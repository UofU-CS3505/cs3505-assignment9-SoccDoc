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
#include "TrainDrawer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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

protected:
    /**
     * @brief closeEvent is a method that gets invoked when user closes the mainwindow
     * @param event - captures when the user clicks close
     */
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    MapModel *map;
    void createActions();

    typedef void (MainWindow::*signalPointer)();
    QQueue<signalPointer> tipQueue; // queue for signals

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
    QMessageBox* starterTip;
    QMessageBox* secondTip;
    QMessageBox* thirdTip;
    QMessageBox* fourthTip;
    QMessageBox* fifthTip;

public slots:
    /**
     * @brief gives a short description of this application
     */
    void about();
    void updateData(int, int);
    void updateTrainDetailsDock(QString);
    void updateStationDetailsDock(QString);
    void showTip();
    void fillProgressBar();
    void resetProgressBar();

signals:
    void starterTipSignal();
    void secondTipSignal();
    void thirdTipSignal();
    void fourthTipSignal();
    void fifthTipSignal();
};
#endif // MAINWINDOW_H
