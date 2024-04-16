#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QLabel>
#include "mapmodel.h"
#include <QQueue>
#include <QMetaMethod>
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

    //data variables to be updated.
    QLabel* throughput;
    QLabel* waitTime;
    QPixmap image;
    QHBoxLayout* tipLayout = new QHBoxLayout();
    int tipNum = 1;

    void createLeftDockWindow();
    void createRightDockWindow();
    void createBottomDockWindow();
    void createDockWindows();
    void createTipPopups();
    void trainKillingSimulator();
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

    // Tips
    QList<QMessageBox*> tipList;
    QMessageBox* firstTip;
    QMessageBox* secondTip;
    QMessageBox* thirdTip;
    QMessageBox* fourthTip;
    QMessageBox* fifthTip;

    // Queue of signals to execute tips
    typedef void (MainWindow::*signalPointer)();
    QQueue<signalPointer> tipQueue; // queue for signals

public slots:
    void updateData(int, int);
    void updateTrainData(QString);
    void updateStationData(QString);
    void showTip();
    void fillProgressBar();
    void resetProgressBar();

signals:
    void firstTipSignal();
    void secondTipSignal();
    void thirdTipSignal();
    void fourthTipSignal();
    void fifthTipSignal();
};
#endif // MAINWINDOW_H
