#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QLabel>
#include "mapmodel.h"
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

    // Docks for managing train and station details
    QDockWidget* trainDetailsDock;
    QDockWidget* stationDetailsDock;

    //data variables to be updated.
    QLabel* throughput;
    QLabel* waitTime;

    void createLeftDockWindow();
    void createRightDockWindow();
    void createBottomDockWindow();
    void createDockWindows();

    QProgressBar* progressBar;
    /**
     * @brief action to tell the user about the application
     */
    QAction *aboutAct;
    /**
     * @brief action to exit the application
     */
    QAction *exitAct;

public slots:
    /**
     * @brief gives a short description of this application
     */
    void about();
    void updateData(int, int);
    void updateTrainDetailsDock(QString);
    void updateStationDetailsDock(QString);
};
#endif // MAINWINDOW_H
