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
    // Set our canvas as the central widget
    setCentralWidget(map->getDrawer());

    // Create the window format
    createDockWindows();

    // Create the tip library
    createTipPopups();

    // Title and resize the window
    setWindowTitle("Train");
    resize(875, 700);

    // Connect updating detail dock widgets
    connect(map, &MapModel::updateTrainDetails, this, &MainWindow::updateTrainData);
    connect(map, &MapModel::updateStationDetails, this, &MainWindow::updateStationData);
    connect(map, &MapModel::updateData, this, &MainWindow::updateData);
    connect(progressBar, &QProgressBar::valueChanged, map, &MapModel::checkProgressBar);
    connect(map, &MapModel::showNewTip, this, &MainWindow::showTip);
    connect(map, &MapModel::restartProgressBar, this, &MainWindow::resetProgressBar);
    connect(map, &MapModel::restartProgressBar, this, &MainWindow::resetProgressBar);
    connect(map, &MapModel::drawStationPassenger, this, &MainWindow::drawStationPassenger);
    connect(map, &MapModel::updateProgressBar, this, &MainWindow::updateProgressBar);

}

MainWindow::~MainWindow() {}

void MainWindow::drawStationPassenger(Station* station, Passenger passenger){
    QLabel* label = new QLabel(map->getDrawer());
    QPixmap newImage;
    if (passenger == Circle){
        shapeImage.load(":/images/images/Circle.png") ;
        newImage = shapeImage.scaled(5, 5, Qt::KeepAspectRatio);
    }
    else if (passenger == Square){
        shapeImage.load(":/images/images/Square.png") ;
        newImage = shapeImage.scaled(5, 5, Qt::KeepAspectRatio);
    }
    else if (passenger == Triangle){
        shapeImage.load(":/images/images/Triangle.png") ;
        newImage = shapeImage.scaled(5, 5, Qt::KeepAspectRatio);
    }
    label->setPixmap(newImage);
    label->setGeometry(station->getLocation().x() + (station->returnWaitingSize() * 10) - 30, station->getLocation().y() - 10, 10, 5);
    label->show();
}

void MainWindow::createDockWindows() {
    createLeftDockWindow();
    createRightDockWindow();
    createBottomDockWindow();
}


void MainWindow::createLeftDockWindow() {
    QPixmap LineImage;
    // Create train layout and add radio buttons
    QRadioButton* green = new QRadioButton("");
    LineImage.load(":/images/images/greenLine.png") ;
    //LineImage.fill(Qt::green);
    QIcon newImage = QIcon(LineImage.scaled(120, 30, Qt::IgnoreAspectRatio));
    green->setIcon(newImage);
    green->setIconSize(QSize(120, 30));
    green->setBaseSize(120, 40);
    // label->setPixmap(newImage);
    //  label.setMask(newImage.mask());
    // //label->setGeometry(station->getLocation().x() + (station->returnWaitingSize() * 10) - 30, station->getLocation().y() - 10, 10, 5);
    // label->show();

    QRadioButton* blue = new QRadioButton("");
    LineImage.load(":/images/images/blueLine.png") ;
    //LineImage.fill(Qt::blue);
    newImage = QIcon(LineImage.scaled(120, 30, Qt::IgnoreAspectRatio));
    blue->setIcon(newImage);
    blue->setIconSize(QSize(120, 30));
    blue->setBaseSize(120, 40);


    QRadioButton* red = new QRadioButton("");
    LineImage.load(":/images/images/redLine.png") ;
    newImage = QIcon(LineImage.scaled(120, 30, Qt::IgnoreAspectRatio));
    red->setIcon(newImage);
    red->setIconSize(QSize(120, 30));
    red->setBaseSize(120, 40);

    green->setChecked(true); // Check default option

    // Add train buttons to button group
    QButtonGroup* trainButtonGroup = new QButtonGroup();
    trainButtonGroup->addButton(green, 0);
    trainButtonGroup->addButton(blue, 1);
    trainButtonGroup->addButton(red, 2);
    connect(trainButtonGroup, &QButtonGroup::idClicked, map, &MapModel::trainButtonClicked);

    // Put train buttons in a vertical layout
    QVBoxLayout* trainLayout = new QVBoxLayout();
    trainLayout->setAlignment(Qt::AlignTop);
    trainLayout->addWidget(green);
    trainLayout->addWidget(blue);
    trainLayout->addWidget(red);

    QWidget* trainWidget = new QWidget();
    trainWidget->setLayout(trainLayout);

    // Create station layout and add radio buttons ---------------------------------
    QRadioButton* square = new QRadioButton("Square station");
    QRadioButton* circle = new QRadioButton("Circle station");

    square->setChecked(true); // Check default option

    // Add station buttons to button group
    QButtonGroup* stationButtonGroup = new QButtonGroup();
    stationButtonGroup->addButton(square, 0);
    stationButtonGroup->addButton(circle, 1);
    connect(stationButtonGroup, &QButtonGroup::idClicked, map, &MapModel::stationButtonClicked);

    // Put station buttons in a vertical layout
    QVBoxLayout* stationLayout = new QVBoxLayout();
    stationLayout->setAlignment(Qt::AlignTop);
    stationLayout->addWidget(square);
    stationLayout->addWidget(circle);

    QWidget* stationWidget = new QWidget();
    stationWidget->setLayout(stationLayout);

    // Set the layout to one multi-widget and the multi widget to frameScrolling and dock it on the window
    QDockWidget* trainDock = new QDockWidget("Trains", this);
    QDockWidget* stationDock = new QDockWidget("Stations", this);

    trainDock->setWidget(trainWidget);
    stationDock->setWidget(stationWidget);

    // Dock the frame buttons on the left side
    this->addDockWidget(Qt::LeftDockWidgetArea, trainDock);
    this->addDockWidget(Qt::LeftDockWidgetArea, stationDock);

    trainDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    stationDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
}

void MainWindow::createRightDockWindow() {
    // Create train details labels
    QLabel* trainThroughput = new QLabel("throughput: ");
    QLabel* trainStations = new QLabel("stations: ");

    // Put train buttons in a vertical layout
    QVBoxLayout* trainLayout = new QVBoxLayout();
    trainLayout->setAlignment(Qt::AlignTop);
    trainLayout->addWidget(trainThroughput);
    trainLayout->addWidget(trainStations);

    QWidget* trainWidget = new QWidget();
    trainWidget->setLayout(trainLayout);

    // Create station details labels --------------------------------
    QLabel* stationThroughput = new QLabel("throughput: ");
    QLabel* stationTrains = new QLabel("trains: ");

    // Put station buttons in a vertical layout
    QVBoxLayout* stationLayout = new QVBoxLayout();
    stationLayout->setAlignment(Qt::AlignTop);
    stationLayout->addWidget(stationThroughput);
    stationLayout->addWidget(stationTrains);

    QWidget* stationWidget = new QWidget();
    stationWidget->setLayout(stationLayout);

    // Set the layout to one multi-widget and the multi widget to frameScrolling and dock it on the window
    trainDetailsDock = new QDockWidget("Green Train Details", this);     // Default train
    stationDetailsDock = new QDockWidget("Square Station Details", this); // Default station

    trainDetailsDock->setWidget(trainWidget);
    stationDetailsDock->setWidget(stationWidget);

    // Dock the frame buttons on the left side
    this->addDockWidget(Qt::RightDockWidgetArea, trainDetailsDock);
    this->addDockWidget(Qt::RightDockWidgetArea, stationDetailsDock);

    trainDetailsDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    stationDetailsDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
}

void MainWindow::createBottomDockWindow() {
    // Setup a tip popup
    QPushButton* tip = new QPushButton("Fill progress bar");
    connect(tip, &QPushButton::clicked, this, &MainWindow::fillProgressBar);

    // Put the tips into a layout
    tipLayout->addWidget(tip);

    // Put the tips into a widget
    QWidget* tipWidget = new QWidget();
    tipWidget->setLayout(tipLayout);

    // Setup progress bar
    progressBar = new QProgressBar();
    progressBar->setMaximum(100);
    progressBar->setValue(0);

    // Setup data
    throughput = new QLabel("Throughput: ");
    waitTime = new QLabel("WaitTime: ");
    numberOfPassengers = new QLabel("Number Of Passengers Waiting: ");

    // Put the data into a layout
    QVBoxLayout* dataLayout = new QVBoxLayout();
    //tipLayout->addItem(dataLayout);
    dataLayout->addWidget(progressBar);
    dataLayout->addWidget(throughput);
    dataLayout->addWidget(waitTime);
    dataLayout->addWidget(numberOfPassengers);

    // Put the data into a widget
    QWidget* dataWidget = new QWidget();
    dataWidget->setLayout(dataLayout);

    // Add the data and tip widgets to the tab widget
    QTabWidget* bottomTabWidget = new QTabWidget();
    bottomTabWidget->addTab(dataWidget, "Data");
    bottomTabWidget->addTab(tipWidget, "Tips");

    // Dock the tab widget
    QDockWidget* bottomDock = new QDockWidget("Library", this);
    bottomDock->setWidget(bottomTabWidget);

    // Dock the frame buttons on the left side and set alignment
    dataLayout->setAlignment(Qt::AlignTop);
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
                      "button to draw a new\n track for the selected train.");
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
                       "needs to go to\n a specific station. Be sure each station "
                       "types can get to every\n type of station!");
    secondTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    secondTip->setStandardButtons(QMessageBox::Ok);
    connect(this, &MainWindow::secondTipSignal, secondTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(secondTip);

    // Setup third tip
    thirdTip = new QMessageBox(this);
    thirdTip->setWindowTitle("Tracking Data");
    thirdTip->setText("Each train and station tracks data that can be seen "
                      "when\n that train is selected. Additionally, there is "
                      "average data\n for all trains and stations in the 'Data' "
                      "tab at the bottom.");
    thirdTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    thirdTip->setStandardButtons(QMessageBox::Ok);
    connect(this, &MainWindow::thirdTipSignal, thirdTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(thirdTip);

    // Setup fourth tip
    fourthTip = new QMessageBox(this);
    fourthTip->setWindowTitle("Consider This! (4)");
    fourthTip->setText("Part 4 Info:");
    fourthTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    fourthTip->setStandardButtons(QMessageBox::Ok);
    connect(this, &MainWindow::fourthTipSignal, fourthTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(fourthTip);

    // Setup fifth tip
    fifthTip = new QMessageBox(this);
    fifthTip->setWindowTitle("Consider This! (5)");
    fifthTip->setText("Part 5 Info:");
    fifthTip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    fifthTip->setStandardButtons(QMessageBox::Ok);
    connect(this, &MainWindow::fifthTipSignal, fifthTip, &QMessageBox::exec);
    tipMessageBoxQueue.enqueue(fifthTip);
}

void MainWindow::updateTrainData(QString newData) {
    trainDetailsDock->setWindowTitle(newData);
}

void MainWindow::updateStationData(QString newData) {
    stationDetailsDock->setWindowTitle(newData);
}

void MainWindow::updateData(double newThroughput, double newWaitTime, double numOfPassengers){
    QString throughputStr = QString::number(newThroughput, 'f', 3);
    QString waitTimeStr = QString::number(newWaitTime, 'f', 3);
     QString numOfPassengersStr = QString::number(numOfPassengers, 'f', 1);

    throughput->setText(("Throughput: " + throughputStr ));
    waitTime->setText(("WaitTime: " + waitTimeStr + " Seconds"));
    numberOfPassengers->setText(("Numer Of Passengers Waiting: " + numOfPassengersStr));
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
