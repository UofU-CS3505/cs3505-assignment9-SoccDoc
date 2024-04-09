#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), control(new Control(this))
{
    ui->setupUi(this);

    // Set our canvas as the central widget
    setCentralWidget(control);





    // Add grouped dragging to dock options (the rest are default)
    //setDockOptions(GroupedDragging|AnimatedDocks|AllowTabbedDocks);


    // Title and resize the window
    setWindowTitle("Train");
    resize(875, 700);
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
