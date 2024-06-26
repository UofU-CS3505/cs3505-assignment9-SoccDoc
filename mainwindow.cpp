#include "mainwindow.h"
#include <QRadioButton>
#include <QPushButton>
#include <QButtonGroup>
#include <QDockWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mapModel(new MapModel(this))
{
    // Make the pop up for introducing the train education app.
    QMessageBox* startPrompt = new QMessageBox(this);
    startPrompt->setWindowTitle("Train Education App");

    // Set the image to appear along with the text.
    QPixmap startIcon;
    startIcon.load(":/images/images/RealTrainImage.png");
    startIcon = startIcon.scaled(400, 200, Qt::KeepAspectRatio);
    startPrompt->setIconPixmap(startIcon);

    // Define the message box text details
    startPrompt->setText(tr("This Train Education App is to help people get a basic sense on how train systems need to function"
                         " and learn a bit more about trains.\n\n This app will have users create tracks between stations and try to "
                         "maintain a passenger flow between stations.\n More stations will be added as the user is able to get passengers "
                            "to their destinations."));
    startPrompt->setStyleSheet("QLabel{min-width: 300px; min-height: 300px;}");
    startPrompt->setStandardButtons(QMessageBox::Ok);

    // Execute the popup for the start window.
    startPrompt->exec();

    // Set our canvas as the central widget
    setCentralWidget(mapModel->getDrawer());

    // Create the window format
    createDockWindows();

    // Create the tip library
    createTipPopups();

    // Title and resize the window
    setWindowTitle("Train Education App");
    resize(1150, 700);

    //Call Default popup
    QMessageBox* tutorialPopup = new QMessageBox(this);
    tutorialPopup->setWindowTitle("Drawing Tracks");
    tutorialPopup->setText("Select a train then click the 'Redraw Track' "
                       "button to \ndraw a new track for the selected train.");
    tutorialPopup->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    tutorialPopup->setStandardButtons(QMessageBox::Ok);

    // Connect updating detail dock widgets
    connect(mapModel, &MapModel::updateData, this, &MainWindow::updateData);
    connect(progressBar, &QProgressBar::valueChanged, mapModel, &MapModel::checkProgressBar);
    connect(mapModel, &MapModel::showNewTip, this, &MainWindow::showTip);
    connect(mapModel, &MapModel::restartProgressBar, this, &MainWindow::resetProgressBar);
    connect(mapModel, &MapModel::restartProgressBar, this, &MainWindow::resetProgressBar);
    connect(mapModel, &MapModel::drawStationPassenger, this, &MainWindow::drawStationPassenger);
    connect(mapModel, &MapModel::updateProgressBar, this, &MainWindow::updateProgressBar);
    connect(mapModel, &MapModel::enableTrackButtonsSignal, this, &MainWindow::enableTrackButtons);
    connect(mapModel, &MapModel::addTrainType, this, &MainWindow::addTrainButton);
    connect(mapModel, &MapModel::drawTrainPassenger, this, &MainWindow::drawTrainPassenger);

    this -> show();

    // Execute the popup for the start window.
    tutorialPopup->exec();
}

MainWindow::~MainWindow() {}

void MainWindow::drawTrainPassenger(Train* train){
    // Check if the train is still alive
    if (!train)
        return;

    // Remove all of the passenger icons
    train->clearPassengerIcons();

    // Iterate through every passenger and draw them
    QPixmap passengerImage;
    foreach (Passenger p, train->getPassengers()) {
        QLabel* label = new QLabel(mapModel->getDrawer()); // Create a new QLabel object for each passenger
        // Load the proper passenger icon
        switch (p) {
        case Square:
            passengerImage.load(":/images/images/Square.png");
            break;
        case Circle:
            passengerImage.load(":/images/images/Circle.png");
            break;
        case Triangle:
            passengerImage.load(":/images/images/Triangle.png");
            break;
        case Last:
            break; // Here to account for all cases
        }

        // Finish setting up the label
        passengerImage = passengerImage.scaled(PASSENGER_ICON_WIDTH, PASSENGER_ICON_WIDTH, Qt::KeepAspectRatio);
        label->setPixmap(passengerImage);
        label->show();
        train->addPassengerIcon(label);

        // Set the start and end points of the animation
        int iconWidthBuffer = train->getNumberOfPassengerIcons() * (PASSENGER_ICON_WIDTH * 2);
        QPoint trainAnimationStartPoint = train->getAnimation()->startValue().toPoint();
        QPoint trainAnimationEndPoint = train->getAnimation()->endValue().toPoint();
        QPoint passengerAnimationStartPoint = QPoint(trainAnimationStartPoint.x() + (iconWidthBuffer),
                                                     trainAnimationStartPoint.y() - (PASSENGER_ICON_WIDTH * 2));
        QPoint passengerAnimationEndPoint = QPoint(trainAnimationEndPoint.x() + (iconWidthBuffer),
                                                   trainAnimationEndPoint.y() - (PASSENGER_ICON_WIDTH * 2));

        // Set up the animation object and start
        QPropertyAnimation* animation = new QPropertyAnimation(label, "pos");
        animation->setDuration(train->getAnimation()->duration());
        animation->setStartValue(passengerAnimationStartPoint);
        animation->setEndValue(passengerAnimationEndPoint);
        animation->start();
    }

}

void MainWindow::drawStationPassenger(Station* station){
    // Remove all of the passenger icons
    station->clearPassengerIcons();

    // Redraw all of the passenger icons
    QPixmap passengerIcon;
    foreach (Passenger p, station->getPassengers()){
        QLabel* label = new QLabel(mapModel->getDrawer());

        // Load the proper passenger icon
        switch (p) {
        case Square:
            passengerIcon.load(":/images/images/Square.png");
            break;
        case Circle:
            passengerIcon.load(":/images/images/Circle.png");
            break;
        case Triangle:
            passengerIcon.load(":/images/images/Triangle.png");
            break;
        case Last:
            break; // Here to account for all cases
        }

        // Finish setting up the label
        passengerIcon = passengerIcon.scaled(PASSENGER_ICON_WIDTH, PASSENGER_ICON_WIDTH, Qt::KeepAspectRatio);
        label->setPixmap(passengerIcon);

        // Set the location of the icon
        int iconWidthBuffer = station->getNumberOfPassengerIcons() * (PASSENGER_ICON_WIDTH * 2);
        label->setGeometry(station->getLocation().x() + iconWidthBuffer - (PASSENGER_ICON_WIDTH * 6), // x coordinate of icon
                           station->getLocation().y() - (PASSENGER_ICON_WIDTH * 2),                   // y coordinate of icon (looks better double)
                           (PASSENGER_ICON_WIDTH * 2),                                                // width of icon (looks better doubled)
                           PASSENGER_ICON_WIDTH);                                                     // height of icon

        // Display the icon
        label->show();
        station->addPassengerIcon(label);
    }
}

void MainWindow::createDockWindows() {
    createLeftDockWindow();
    createRightDockWindow();
    createBottomDockWindow();
}

void MainWindow::createLeftDockWindow() {
    // Create train layout and add initial radio button
    QRadioButton* greenButton = new QRadioButton("");
    QPixmap LineImage;
    LineImage.load(":/images/images/greenLine.png") ;
    QIcon newImage = QIcon(LineImage.scaled(120, 30, Qt::IgnoreAspectRatio));
    greenButton->setIcon(newImage);
    greenButton->setIconSize(QSize(120, 30));
    greenButton->setBaseSize(120, 40);
    greenButton->setChecked(true); // Check default option

    // Add train button to button group
    trainButtonGroup = new QButtonGroup();
    trainButtonGroup->addButton(greenButton, 0);
    connect(trainButtonGroup, &QButtonGroup::idClicked, mapModel, &MapModel::trainButtonClicked);

    // Create button for redrawing the track
    RedrawTrack = new QPushButton("Redraw Green Track"); // Green because that is the default
    connect(RedrawTrack, &QPushButton::clicked, this, &MainWindow::redrawTracks);
    connect(mapModel, &MapModel::changeRedrawTrackText, RedrawTrack, &QPushButton::setText);

    // Put train buttons in a vertical layout
    trainLayout = new QVBoxLayout();
    trainLayout->setAlignment(Qt::AlignTop);
    trainLayout->addWidget(RedrawTrack);
    trainLayout->addWidget(greenButton);

    QWidget* trainWidget = new QWidget();
    trainWidget->setLayout(trainLayout);

    // Set the layout to one multi-widget and the multi widget to frameScrolling and dock it on the window
    QDockWidget* trainDock = new QDockWidget("Trains", this);
    trainDock->setWidget(trainWidget);

    // Dock the frame buttons on the left side
    this->addDockWidget(Qt::LeftDockWidgetArea, trainDock);
    trainDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
}

void MainWindow::createRightDockWindow() {
    // Setup throughput
    QLabel* throughputImg = new QLabel();
    throughputImg->setPixmap(QPixmap(":/images/images/throughput.png").scaled(30,30));
    throughput = new QLabel("Passengers/Second: ");

    QHBoxLayout* throughputLayout = new QHBoxLayout();
    throughputLayout->setAlignment(Qt::AlignLeft);
    throughputLayout->addWidget(throughputImg);
    throughputLayout->addWidget(throughput);

    // Setup wait time
    QLabel* waitTimeImg = new QLabel();
    waitTimeImg->setPixmap(QPixmap(":/images/images/WaitTime.png").scaled(30, 30));
    waitTime = new QLabel("WaitTime: ");

    QHBoxLayout* waitTimeLayout = new QHBoxLayout();
    waitTimeLayout->setAlignment(Qt::AlignLeft);
    waitTimeLayout->addWidget(waitTimeImg);
    waitTimeLayout->addWidget(waitTime);

    // Setup number of passengers waiting
    QLabel* passengerWaiting = new QLabel();
    passengerWaiting->setPixmap(QPixmap(":/images/images/waitingPassengers.png").scaled(30, 30));
    numberOfPassengers = new QLabel("Number Of Passengers Waiting: ");

    QHBoxLayout* passengersWaitingLayout = new QHBoxLayout();
    passengersWaitingLayout->setAlignment(Qt::AlignLeft);
    passengersWaitingLayout->addWidget(passengerWaiting);
    passengersWaitingLayout->addWidget(numberOfPassengers);

    // Put all of the data to a vertical layout
    QVBoxLayout* dataLayout = new QVBoxLayout();
    dataLayout->addLayout(throughputLayout);
    dataLayout->addLayout(waitTimeLayout);
    dataLayout->addLayout(passengersWaitingLayout);

    // Put the data into a widget
    QWidget* dataWidget = new QWidget();
    dataWidget->setLayout(dataLayout);


    // Dock the tab widget
    QDockWidget* rightDock = new QDockWidget("Library", this);
    rightDock->setWidget(dataWidget);

    // Dock the frame buttons on the left side and set alignment
    dataLayout->setAlignment(Qt::AlignTop);
    this->addDockWidget(Qt::RightDockWidgetArea, rightDock);
    rightDock->setFeatures(QDockWidget::NoDockWidgetFeatures);

}

void MainWindow::createBottomDockWindow() {
    // Put the tips into a horizontal layout
    tipLayout = new QHBoxLayout();

    // Setup progress bar
    progressBar = new QProgressBar();
    progressBar->setMaximum(100);
    progressBar->setValue(0);

    // Put the tips and progress bar into a vertical layout
    QVBoxLayout* libraryLayout = new QVBoxLayout();
    libraryLayout->addWidget(progressBar);
    libraryLayout->addLayout(tipLayout);

    // Put the tips into a widget
    QWidget* libraryWidget = new QWidget();
    libraryWidget->setLayout(libraryLayout);

    // Dock the library widget
    QDockWidget* bottomDock = new QDockWidget("Library", this);
    bottomDock->setWidget(libraryWidget);

    // Dock the frame buttons on the left side and set alignment
    tipLayout->setAlignment(Qt::AlignLeft);
    this->addDockWidget(Qt::BottomDockWidgetArea, bottomDock);
    bottomDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    bottomDock->setMinimumSize(1150, 198);
    bottomDock->setMaximumSize(1150, 198);
}

void MainWindow::addTrainButton() {
    // Declare image for the buttons color
    QPixmap LineImage;

    // Decide on color of button based on how many we already have
    int numberOfButtons = trainButtonGroup->buttons().size();
    switch (numberOfButtons) {
    case 1:
        LineImage.load(":/images/images/blueLine.png");
        break;
    case 2:
        LineImage.load(":/images/images/redLine.png");
        break;
    default:
        return;
    }

    // Set up rest of button
    QRadioButton* newButton = new QRadioButton("");;
    QIcon newImage = QIcon(LineImage.scaled(120, 30, Qt::IgnoreAspectRatio));
    newButton->setIcon(newImage);
    newButton->setIconSize(QSize(120, 30));
    newButton->setBaseSize(120, 40);
    trainButtonGroup->addButton(newButton, numberOfButtons); // The next button's id is the same as the previous number of buttons
    trainLayout->addWidget(newButton);
}

void MainWindow::createTipPopups() {
    // Make the pop up for a tip
    firstTip = new QMessageBox(this);

    // Define the message box details
    QPixmap firstTipIcon;
    firstTipIcon.load(":/images/images/Tip1.png");
    firstTipIcon = firstTipIcon.scaled(200, 300, Qt::KeepAspectRatio);

    firstTip->setWindowTitle("Passenger Needs");
    firstTip->setText("Each passenger has a different request and "
                      "needs to go \nto a specific station. Be sure your trains "
                      "can get to every\n type of station! This will increase the efficiency of your network!");
    firstTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    firstTip->setStandardButtons(QMessageBox::Ok);
    firstTip->setIconPixmap(firstTipIcon);

    // Connect signals for popups
    connect(this, &MainWindow::firstTipSignal, firstTip, &QMessageBox::exec);

    // Enqueue signals for popups
    tipMessageBoxQueue.enqueue(firstTip);

    // Setup second tip
    QPixmap secondTipIcon;
    secondTipIcon.load(":/images/images/Tip2.png");
    secondTipIcon = secondTipIcon.scaled(400, 300, Qt::KeepAspectRatio);

    secondTip = new QMessageBox(this);
    secondTip->setWindowTitle("How to Progress");
    secondTip->setText("The more passengers that are transported to their desired stations \n"
                       "the more progress that you will make.\n\n"
                       "If a station overfills with passengers, the progress bar will begin decreasing.");
    secondTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    secondTip->setStandardButtons(QMessageBox::Ok);
    secondTip->setIconPixmap(secondTipIcon);
    connect(this, &MainWindow::secondTipSignal, secondTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(secondTip);

    // Setup third tip
    QPixmap thirdTipIcon;
    thirdTipIcon.load(":/images/images/Tip3.png");
    thirdTipIcon = thirdTipIcon.scaled(400, 300, Qt::KeepAspectRatio);

    thirdTip = new QMessageBox(this);
    thirdTip->setWindowTitle("Tracking Data");
    thirdTip->setText("Clicking on a station will show data about it on the right side of the window.\n"
                      "Try your best to optimize your Train Lines "
                      "to minimise these numbers!");
    thirdTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    thirdTip->setStandardButtons(QMessageBox::Ok);
    thirdTip->setIconPixmap(thirdTipIcon);
    connect(this, &MainWindow::thirdTipSignal, thirdTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(thirdTip);

    // Setup fourth tip
    QPixmap fourthTipIcon;
    fourthTipIcon.load(":/images/images/Tip4.png");
    fourthTipIcon = fourthTipIcon.scaled(400, 300, Qt::KeepAspectRatio);

    fourthTip = new QMessageBox(this);
    fourthTip->setWindowTitle("As Stations get Added");
    fourthTip->setText("As more stations get added, it is"
                       " important to maintain \nthe efficiency of each line, "
                       "try redrawing lines to allow \nmore people to get to their destination.");
    fourthTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    fourthTip->setStandardButtons(QMessageBox::Ok);
    fourthTip->setIconPixmap(fourthTipIcon);
    connect(this, &MainWindow::fourthTipSignal, fourthTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(fourthTip);

    // Setup fifth tip
    QPixmap fifthTipIcon;
    fifthTipIcon.load(":/images/images/Tip5.JPG");
    fifthTipIcon = fifthTipIcon.scaled(600, 300, Qt::KeepAspectRatio);

    fifthTip = new QMessageBox(this);
    fifthTip->setWindowTitle("Max Capacity");
    fifthTip->setText("Just like in real life, these trains can only hold \na certain "
                      "amount of people. Be sure passengers "
                      "don't get too \ncrowded! Nobody likes an over crowded train!");
    fifthTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    fifthTip->setStandardButtons(QMessageBox::Ok);
    fifthTip->setIconPixmap(fifthTipIcon);

    connect(this, &MainWindow::fifthTipSignal, fifthTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(fifthTip);

    // Setup Sixth Tip Image
    QPixmap sixthTipIcon;
    sixthTipIcon.load(":/images/images/FarAwayStationExample.png");
    sixthTipIcon = sixthTipIcon.scaled(600, 300, Qt::KeepAspectRatio);

    // Setup sixth tip
    sixthTip = new QMessageBox(this);
    sixthTip->setWindowTitle("Track Length");
    sixthTip->setText("Just like in real life, the farther away stations are from each other the longer \nit will take the train to arrive to that station.");
    sixthTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    sixthTip->setStandardButtons(QMessageBox::Ok);
    sixthTip->setIconPixmap(sixthTipIcon);

    connect(this, &MainWindow::sixthTipSignal, sixthTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(sixthTip);

    //Setup Seventh tip image
    QPixmap seventhTipIcon;
    seventhTipIcon.load(":/images/images/TrainWaitTimeExample.png");
    seventhTipIcon = seventhTipIcon.scaled(600, 300, Qt::KeepAspectRatio);

    // Setup seventh tip
    seventhTip = new QMessageBox(this);
    seventhTip->setWindowTitle("Wait Time Clarification");
    seventhTip->setText("Wait times represent the amount of time until a train arrival for that station.");
    seventhTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    seventhTip->setStandardButtons(QMessageBox::Ok);
    seventhTip->setIconPixmap(seventhTipIcon);

    connect(this, &MainWindow::seventhTipSignal, seventhTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(seventhTip);

}

void MainWindow::updateData(double newThroughput, double newWaitTime, double numOfPassengers){
    QString throughputStr = QString::number(newThroughput, 'f', 3);
    QString waitTimeStr = QString::number(newWaitTime, 'f', 3);
    QString numOfPassengersStr = QString::number(numOfPassengers, 'f', 1);

    // Display current data
    throughput->setText(("Passengers/Second: " + throughputStr ));
    waitTime->setText(("WaitTime: " + waitTimeStr + " Seconds"));
    numberOfPassengers->setText(("Passengers Waiting: " + numOfPassengersStr));

    // Update data
    throughput->update();
    waitTime->update();
    numberOfPassengers->update();
}

void MainWindow::showTip() {
    // Check if there are more tip signals
    if (tipMessageBoxQueue.isEmpty())
        return;

    // Get the next tip message box and make a button for it
    QMessageBox* tipMessageBox = tipMessageBoxQueue.dequeue();;
    QPushButton* tipButton = new QPushButton(tipMessageBox->windowTitle());

    // Create connection to the popup for this new button
    connect(tipButton, &QPushButton::clicked, tipMessageBox, &QMessageBox::exec);
    tipLayout->addWidget(tipButton);
    tipMessageBox->exec();
}

void MainWindow::fillProgressBar() {
    progressBar->setValue(100);
}

void MainWindow::resetProgressBar() {
    progressBar->setValue(0);
}
void MainWindow::updateProgressBar(int newValue) {
    progressBar->setValue(newValue);
}

void MainWindow::redrawTracks(){
    foreach (QAbstractButton* button, trainButtonGroup->buttons())
        button->setEnabled(false);

    mapModel->redrawTrack();
}

void MainWindow::enableTrackButtons(){
    foreach (QAbstractButton* button, trainButtonGroup->buttons())
        button->setEnabled(true);
}
