#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QDockWidget>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), map(new MapModel(this))
{
    ui->setupUi(this);

    // Set our canvas as the central widget
    setCentralWidget(map->getDrawer());





    // Add grouped dragging to dock options (the rest are default)
    //setDockOptions(GroupedDragging|AnimatedDocks|AllowTabbedDocks);

    createDockWindows();
    //createTipPopups();

    // Title and resize the window
    setWindowTitle("Train");
    resize(875, 700);

    // Connect updating detail dock widgets
    connect(map, &MapModel::updateTrainDetails, this, &MainWindow::updateTrainDetailsDock);
    connect(map, &MapModel::updateStationDetails, this, &MainWindow::updateStationDetailsDock);
    connect(map, &MapModel::updateData, this, &MainWindow::updateData);
}

MainWindow::~MainWindow(){

}

 void MainWindow::closeEvent(QCloseEvent *event) {
//     // Check if the user has unsaved progress before closing and prompt them to save
// //     if (maybeSave())
        event->accept();
// //     else
// //         event->ignore(); // they chose to cancel closing
 }



void MainWindow::about() {
    QMessageBox::about(this, "About Sprite Editor",
                       "<p>This <b>Sprite Editor</b> enables users to create sprites"
                       "in a multitude of colors! Additionally, users can erase pixels"
                       "and use the paint bucket tool to fill areas of their sprite. "
                       "They can also save sprites as an ssp file and load ssp files"
                       "regardless of number of pixels into the editor. There is a preview "
                       "in the top left corner that has a changable frame rate. Users can "
                       "use the buttons on the left to add and remove frames. There are "
                       "also buttons that appear as the user add frames that will jump "
                       "the user to the pictured frame! The user can only set the number "
                       "of pixels upon entering the sprite editor.</p>");
}

void MainWindow::createActions() {


    // Connect the exit action to closing the application
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);

    // Connect the about act to a popup that describes the application to the user
    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createDockWindows() {
   createLeftDockWindow();
   createRightDockWindow();
   createBottomDockWindow();
}

void MainWindow::createLeftDockWindow() {
    // Create train layout and add radio buttons
    QRadioButton* orange = new QRadioButton("Orange Line");
    QRadioButton* blue = new QRadioButton("Blue Line");
    QRadioButton* red = new QRadioButton("Red Line");

    orange->setChecked(true); // Check default option

    // Add train buttons to button group
    QButtonGroup* trainButtonGroup = new QButtonGroup();
    trainButtonGroup->addButton(orange, 0);
    trainButtonGroup->addButton(blue, 1);
    trainButtonGroup->addButton(red, 2);
    connect(trainButtonGroup, &QButtonGroup::idClicked, map, &MapModel::trainButtonClicked);

    // Put train buttons in a vertical layout
    QVBoxLayout* trainLayout = new QVBoxLayout();
    trainLayout->setAlignment(Qt::AlignTop);
    trainLayout->addWidget(orange);
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
    trainDetailsDock = new QDockWidget("Orange Train Details", this);     // Default train
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
    QPushButton* tip = new QPushButton("Tip 1");
    connect(tip, &QPushButton::clicked, this, &MainWindow::showTip);

    // Put the tips into a layout
    QHBoxLayout* tipLayout = new QHBoxLayout();
    tipLayout->addWidget(tip);

    // Put the tips into a widget
    QWidget* tipWidget = new QWidget();
    tipWidget->setLayout(tipLayout);

    // Setup progress bar
    progressBar = new QProgressBar();
    progressBar->setMaximum(100);
    progressBar->setValue(50);

    // Setup data
    throughput = new QLabel("Throughput: ");
    waitTime = new QLabel("WaitTime: ");

    // Put the data into a layout
    QVBoxLayout* dataLayout = new QVBoxLayout();
    tipLayout->addItem(dataLayout);
    dataLayout->addWidget(progressBar);
    dataLayout->addWidget(throughput);
    dataLayout->addWidget(waitTime);

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
    QMessageBox* tip = new QMessageBox(this);

    // Define the message box details
    tip->setWindowTitle("Tip 1");
    tip->setText("Some info");
    tip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    tip->setStandardButtons(QMessageBox::Ok);

    // Execute the popup
    tip->exec();
}

void MainWindow::updateTrainDetailsDock(QString newDetails) {
    trainDetailsDock->setWindowTitle(newDetails);
}

void MainWindow::updateStationDetailsDock(QString newDetails) {
    stationDetailsDock->setWindowTitle(newDetails);
}

void MainWindow::updateData(int newThroughput, int newWaitTime){
    throughput->setText(&"Throughput: " [newThroughput]);
    waitTime->setText(&"WaitTime: " [newWaitTime]);
}

void MainWindow::showTip() {
    // Make the pop up for a tip
    QMessageBox* tip = new QMessageBox(this);

    // Define the message box details
    tip->setWindowTitle("Tip 1");
    tip->setText("Some info");
    tip->setStyleSheet("QLabel{min-width: 400px; min-height: 300px;}");
    tip->setStandardButtons(QMessageBox::Ok);

    // Execute the popup
    tip->exec();
}
