#include "mainwindow.h"
#include <QRadioButton>
#include <QPushButton>
#include <QButtonGroup>
#include <QDockWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), map(new MapModel(this))
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
    startPrompt->setText("This Train Education App is to help people get a basic sense on how train systems need to function"
                         " and learn a bit more about trains.\n\n This app will have users create tracks between stations and try to "
                         "maintain a passenger flow between stations.\n More stations will be added as the user is able to get passengers "
                         "to their destinations.");
    startPrompt->setStyleSheet("QLabel{min-width: 500px; min-height: 300px;}");
    startPrompt->setStandardButtons(QMessageBox::Ok);

    // Execute the popup for the start window.
    startPrompt->exec();


    // Set our canvas as the central widget
    setCentralWidget(map->getDrawer());

    // Create the window format
    createDockWindows();

    // Create the tip library
    createTipPopups();

    // Title and resize the window
    setWindowTitle("Train");
    resize(1150, 700);

    // Connect updating detail dock widgets
    connect(map, &MapModel::updateStationDetails, this, &MainWindow::updateStationData);
    connect(map, &MapModel::updateData, this, &MainWindow::updateData);
    connect(progressBar, &QProgressBar::valueChanged, map, &MapModel::checkProgressBar);
    connect(map, &MapModel::showNewTip, this, &MainWindow::showTip);
    connect(map, &MapModel::restartProgressBar, this, &MainWindow::resetProgressBar);
    connect(map, &MapModel::restartProgressBar, this, &MainWindow::resetProgressBar);
    connect(map, &MapModel::drawStationPassenger, this, &MainWindow::drawStationPassenger);
    connect(map, &MapModel::updateProgressBar, this, &MainWindow::updateProgressBar);
    connect(map, &MapModel::enableTrackButtonsSignal, this, &MainWindow::enableTrackButtons);
    connect(map, &MapModel::redraw, this, &MainWindow::redrawStation);

}

MainWindow::~MainWindow() {}

void MainWindow::redrawStation(Station* station){
    foreach (QLabel* ql, station->passengerIcons){
        delete ql;
    }
    station->passengerIcons.clear();

    foreach (Passenger p, station->getPassengers()){
        QPixmap newImage;
        QLabel* label = new QLabel(map->getDrawer()); // Create a new QLabel object for each passenger

        if (p == Circle){
            shapeImage.load(":/images/images/Circle.png");
            newImage = shapeImage.scaled(5, 5, Qt::KeepAspectRatio);
        }
        else if (p == Square){
            shapeImage.load(":/images/images/Square.png");
            newImage = shapeImage.scaled(5, 5, Qt::KeepAspectRatio);
        }
        else if (p == Triangle){
            shapeImage.load(":/images/images/Triangle.png");
            newImage = shapeImage.scaled(5, 5, Qt::KeepAspectRatio);
        }

        label->setPixmap(newImage);
        label->setGeometry(station->getLocation().x() + (station->passengerIcons.size() * 10) - 30, station->getLocation().y() - 10, 10, 5);
        label->show();
        station->passengerIcons.append(label);
    }
}
void MainWindow::drawStationPassenger(Station* station){
    QPixmap newImage;
    QLabel* label = new QLabel(map->getDrawer());

    foreach (Passenger p, station->getPassengers()){
        if (p == Circle){
            shapeImage.load(":/images/images/Circle.png") ;
            newImage = shapeImage.scaled(5, 5, Qt::KeepAspectRatio);
        }
        else if (p == Square){
            shapeImage.load(":/images/images/Square.png") ;
            newImage = shapeImage.scaled(5, 5, Qt::KeepAspectRatio);

        }
        else if (p == Triangle){
            shapeImage.load(":/images/images/Triangle.png") ;
            newImage = shapeImage.scaled(5, 5, Qt::KeepAspectRatio);
        }
    }

    label->setPixmap(newImage);
    label->setGeometry(station->getLocation().x() + (station->returnWaitingSize() * 10) - 30, station->getLocation().y() - 10, 10, 5);
    label->show();
    station->passengerIcons.append(label);
}

void MainWindow::createDockWindows() {
    createLeftDockWindow();
    createRightDockWindow();
    createBottomDockWindow();
}


void MainWindow::createLeftDockWindow() {
    QPixmap LineImage;
    // Create train layout and add radio buttons
    greenButton = new QRadioButton("");
    LineImage.load(":/images/images/greenLine.png") ;
    //LineImage.fill(Qt::green);
    QIcon newImage = QIcon(LineImage.scaled(120, 30, Qt::IgnoreAspectRatio));
    greenButton->setIcon(newImage);
    greenButton->setIconSize(QSize(120, 30));
    greenButton->setBaseSize(120, 40);
    // label->setPixmap(newImage);
    //  label.setMask(newImage.mask());
    // //label->setGeometry(station->getLocation().x() + (station->returnWaitingSize() * 10) - 30, station->getLocation().y() - 10, 10, 5);
    // label->show();

    blueButton = new QRadioButton("");
    LineImage.load(":/images/images/blueLine.png") ;
    //LineImage.fill(Qt::blue);
    newImage = QIcon(LineImage.scaled(120, 30, Qt::IgnoreAspectRatio));
    blueButton->setIcon(newImage);
    blueButton->setIconSize(QSize(120, 30));
    blueButton->setBaseSize(120, 40);


    redButton = new QRadioButton("");
    LineImage.load(":/images/images/redLine.png") ;
    newImage = QIcon(LineImage.scaled(120, 30, Qt::IgnoreAspectRatio));
    redButton->setIcon(newImage);
    redButton->setIconSize(QSize(120, 30));
    redButton->setBaseSize(120, 40);

    greenButton->setChecked(true); // Check default option

    // Add train buttons to button group
    QButtonGroup* trainButtonGroup = new QButtonGroup();
    trainButtonGroup->addButton(greenButton, 0);
    trainButtonGroup->addButton(blueButton, 1);
    trainButtonGroup->addButton(redButton, 2);
    connect(trainButtonGroup, &QButtonGroup::idClicked, map, &MapModel::trainButtonClicked);

    QPushButton* RedrawTrack = new QPushButton("Redraw Track");
    connect(RedrawTrack, &QPushButton::clicked, this, &MainWindow::redrawTracks);

    // Put train buttons in a vertical layout
    QVBoxLayout* trainLayout = new QVBoxLayout();
    trainLayout->setAlignment(Qt::AlignTop);
    trainLayout->addWidget(greenButton);
    trainLayout->addWidget(blueButton);
    trainLayout->addWidget(redButton);
    trainLayout->addWidget(RedrawTrack);

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

    // Setup progress bar
    progressBar = new QProgressBar();
    progressBar->setMaximum(100);
    progressBar->setValue(0);

    // Setup data
    throughput = new QLabel("Passengers/Second: ");
    waitTime = new QLabel("WaitTime: ");
    numberOfPassengers = new QLabel("Number Of Passengers Waiting: ");

    // Put the data into a layout
    QVBoxLayout* dataLayout = new QVBoxLayout();
    QHBoxLayout* throughputLayout = new QHBoxLayout();
    throughputLayout->setAlignment(Qt::AlignLeft);


    QLabel* throughputImg = new QLabel();
    throughputImg->setPixmap(QPixmap(":/images/images/throughput.png").scaled(30,30));
    throughputLayout->addWidget(throughputImg);
    throughputLayout->addWidget(throughput);

    QHBoxLayout* waitTimeLayout = new QHBoxLayout();
    waitTimeLayout->setAlignment(Qt::AlignLeft);

    QLabel* waitTimeImg = new QLabel();
    waitTimeImg->setPixmap(QPixmap(":/images/images/WaitTime.png").scaled(30, 30));
    waitTimeLayout->addWidget(waitTimeImg);
    waitTimeLayout->addWidget(waitTime);

    QHBoxLayout* passengersWaitingLayout = new QHBoxLayout();
    passengersWaitingLayout->setAlignment(Qt::AlignLeft);

    QLabel* passengerWaiting = new QLabel();
    passengerWaiting->setPixmap(QPixmap(":/images/images/waitingPassengers.png").scaled(30, 30));
    passengersWaitingLayout->addWidget(passengerWaiting);
    passengersWaitingLayout->addWidget(numberOfPassengers);


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
    // Setup a tip popup
    QPushButton* tip = new QPushButton("Fill progress bar");
    connect(tip, &QPushButton::clicked, this, &MainWindow::fillProgressBar);

    QVBoxLayout* tipOrganizer = new QVBoxLayout();
    // Put the tips into a layout
    tipLayout->addWidget(tip);

    // Put the tips into a widget
    QWidget* tipWidget = new QWidget();
    tipOrganizer->addWidget(progressBar);
    tipOrganizer->addLayout(tipLayout);

    tipWidget->setLayout(tipOrganizer);


    // Dock the tab widget
    QDockWidget* bottomDock = new QDockWidget("Library", this);
    bottomDock->setWidget(tipWidget);

    // Dock the frame buttons on the left side and set alignment
    tipLayout->setAlignment(Qt::AlignLeft);
    this->addDockWidget(Qt::BottomDockWidgetArea, bottomDock);
    bottomDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
}

void MainWindow::createTipPopups() {
    // Make the pop up for a tip
    firstTip = new QMessageBox(this);

    // Define the message box details
    firstTip->setWindowTitle("Drawing Tracks");
    firstTip->setText("Select a train then click the 'Redraw Track' "
                      "button to draw a new track for the selected train.");
    firstTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    firstTip->setStandardButtons(QMessageBox::Ok);

    // Connect signals for popups
    connect(this, &MainWindow::firstTipSignal, firstTip, &QMessageBox::exec);

    // Enqueue signals for popups
    tipMessageBoxQueue.enqueue(firstTip);

    // Setup second tip
    secondTip = new QMessageBox(this);
    secondTip->setWindowTitle("Passenger Needs");
    secondTip->setText("Each passenger has a different request and "
                       "needs to go to a specific station. Be sure each station "
                       "types can get to every\n type of station! This will increase the efficiency of each line!");
    secondTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    secondTip->setStandardButtons(QMessageBox::Ok);
    connect(this, &MainWindow::secondTipSignal, secondTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(secondTip);

    // Setup third tip
    thirdTip = new QMessageBox(this);
    thirdTip->setWindowTitle("Tracking Data");
    thirdTip->setText("Each station tracks data that can be seen "
                      "when that train is selected. Try optimising your Train Lines "
                      "to minimise these numbers!");
    thirdTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    thirdTip->setStandardButtons(QMessageBox::Ok);
    connect(this, &MainWindow::thirdTipSignal, thirdTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(thirdTip);

    // Setup fourth tip
    fourthTip = new QMessageBox(this);
    fourthTip->setWindowTitle("As Stations get Added");
    fourthTip->setText("As more stations get added, it is"
                       " important to maintain the efficiency of each line, "
                       "try redrawing lines to allow more people to get to their destiniation.");
    fourthTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    fourthTip->setStandardButtons(QMessageBox::Ok);
    connect(this, &MainWindow::fourthTipSignal, fourthTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(fourthTip);

    // Setup fifth tip
    fifthTip = new QMessageBox(this);
    fifthTip->setWindowTitle("Max Capacity");
    fifthTip->setText("Just like in real life, these trains can only hold a certain "
                      "amount of people. be sure passengers "
                      "dont get too backed up! nobody likes an over flowing train!");
    fifthTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    fifthTip->setStandardButtons(QMessageBox::Ok);
    connect(this, &MainWindow::fifthTipSignal, fifthTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(fifthTip);
}

void MainWindow::updateStationData(QString newData) {
    stationDetailsDock->setWindowTitle(newData);
}

void MainWindow::updateData(double newThroughput, double newWaitTime, double numOfPassengers){
    QString throughputStr = QString::number(newThroughput, 'f', 3);
    QString waitTimeStr = QString::number(newWaitTime, 'f', 3);
     QString numOfPassengersStr = QString::number(numOfPassengers, 'f', 1);

    throughput->setText(("Passengers/Second: " + throughputStr ));
    waitTime->setText(("WaitTime: " + waitTimeStr + " Seconds"));
    //waitTime->setPixmap(QPixmap(":/images/images/WaitTime.png").scaled(30,30));
    numberOfPassengers->setText(("Passengers Waiting: " + numOfPassengersStr));
    throughput->update();
    waitTime->update();

    numberOfPassengers->update();
}

void MainWindow::showTip() {
    // Check if there are more tip signals
    if (tipMessageBoxQueue.isEmpty()) {
        qDebug() << "Out of tips!";
        return;
    }

    // Get the next tip message box and make a button for it
    QMessageBox* tipMessageBox = tipMessageBoxQueue.dequeue();;
    QPushButton* tipButton = new QPushButton(tipMessageBox->windowTitle());

    // Create connection to the popup for this new button
    connect(tipButton, &QPushButton::clicked, tipMessageBox, &QMessageBox::exec);
    tipLayout->addWidget(tipButton);
    tipMessageBox->exec();

    // C++ at its best (outdated but I worked too hard to remove this entirely)
    // ((*this).*(signalPointer))();
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
    greenButton->setEnabled(false);
    blueButton->setEnabled(false);
    redButton->setEnabled(false);
    map->redrawTrack();
}

void MainWindow::enableTrackButtons(){
    greenButton->setEnabled(true);
    blueButton->setEnabled(true);
    redButton->setEnabled(true);
}
