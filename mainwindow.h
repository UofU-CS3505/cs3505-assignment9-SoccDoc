#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mapmodel.h"
#include <QMainWindow>
#include <QProgressBar>
#include <QLabel>
#include <QQueue>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QButtonGroup>

/**
 * @brief The MainWindow class displays all of the information that MapModel asks it to
 * @authors Benjamin Sidwell, Alex Fraser, Jason Lopex, Andy Liu, and Ryan Nip
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MapModel *mapModel;

    // Docks for managing train details
    QVBoxLayout* trainLayout;
    QButtonGroup* trainButtonGroup;

    QPushButton* RedrawTrack;

    //data variables to be updated.
    QLabel* throughput;
    QLabel* waitTime;
    QLabel* numberOfPassengers;
    QPixmap image;
    QPixmap shapeImage;

    QHBoxLayout* tipLayout = new QHBoxLayout();
    int tipNum = 1;

    const int PASSENGER_ICON_WIDTH = 5; // size of passenger icons

    void createLeftDockWindow();
    void createRightDockWindow();
    void createBottomDockWindow();
    void createDockWindows();
    void createTipPopups();
    void deleteTrain(QLabel* image);

    QProgressBar* progressBar;
    /**
     * @brief action to tell the user about the application
     */
    QAction *aboutAct;
    /**
     * @brief action to exit the application
     */
    QAction *exitAct;

    // Start Message
    QMessageBox* startWindow;

    // Tips
    QQueue<QMessageBox*> tipMessageBoxQueue;
    QMessageBox* firstTip;
    QMessageBox* secondTip;
    QMessageBox* thirdTip;
    QMessageBox* fourthTip;
    QMessageBox* fifthTip;

public slots:
    void drawStationPassenger(Station* station);
    void addTrainButton();
    void updateData(double, double, double);
    void showTip();
    void updateProgressBar(int);
    void fillProgressBar();
    void resetProgressBar();
    void redrawTracks();
    void enableTrackButtons();

signals:
    void startWindowSignal();
    void firstTipSignal();
    void secondTipSignal();
    void thirdTipSignal();
    void fourthTipSignal();
    void fifthTipSignal();
};
#endif // MAINWINDOW_H
