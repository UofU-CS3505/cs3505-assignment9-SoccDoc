#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QLabel>
#include "QtWidgets/qradiobutton.h"
#include "mapmodel.h"
#include <QQueue>
#include <QMessageBox>
#include <QHBoxLayout>

/**
 * @brief The MainWindow class
 * @authors Benjamin Sidwell, Alex Fraser, Jason Lopex, Andy Liu, and Ryan Nip
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MapModel *map;

    // Docks for managing train and station details
    QDockWidget* trainDetailsDock;
    QDockWidget* stationDetailsDock;

    QRadioButton* greenButton;
    QRadioButton* blueButton;
    QRadioButton* redButton;

    //data variables to be updated.
    QLabel* throughput;
    QLabel* waitTime;
    QLabel* numberOfPassengers;
    QPixmap image;
    QPixmap shapeImage;

    QHBoxLayout* tipLayout = new QHBoxLayout();
    int tipNum = 1;

    void createLeftDockWindow();
    void createRightDockWindow();
    void createBottomDockWindow();
    void createDockWindows();
    void createTipPopups();
    void deleteTrain(QLabel* image);
    void drawStationPassenger(Station* station);
    void redrawStation(Station* station);

    QProgressBar* progressBar;
    /**
     * @brief action to tell the user about the application
     */
    QAction *aboutAct;
    /**
     * @brief action to exit the application
     */
    QAction *exitAct;

    // Tips
    QQueue<QMessageBox*> tipMessageBoxQueue;
    QMessageBox* firstTip;
    QMessageBox* secondTip;
    QMessageBox* thirdTip;
    QMessageBox* fourthTip;
    QMessageBox* fifthTip;

public slots:
    void updateData(double, double, double);
    void updateTrainData(QString);
    void updateStationData(QString);
    void showTip();
    void updateProgressBar(int);
    void fillProgressBar();
    void resetProgressBar();
    void redrawTracks();
    void enableTrackButtons();

signals:
    void firstTipSignal();
    void secondTipSignal();
    void thirdTipSignal();
    void fourthTipSignal();
    void fifthTipSignal();
};
#endif // MAINWINDOW_H
