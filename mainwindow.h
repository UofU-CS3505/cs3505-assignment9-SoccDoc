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
 * @brief The MainWindow class displays all of the information that MapModel asks it to on a window
 * @authors Benjamin Sidwell, Alex Fraser, Jason Lopex, Andy Liu, and Ryan Nip
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MapModel *mapModel; // The model that computes the logic for the central widget

    QVBoxLayout* trainLayout; // Layout for adding train buttons
    QButtonGroup* trainButtonGroup; // Radio button group for train butons
    QPushButton* RedrawTrack; // Button for redrawing train tracks

    QLabel* throughput; // Average delivered passengers per second for the selected station
    QLabel* waitTime; // Average wait time between train arrivals for the selected station
    QLabel* numberOfPassengers; // number of passengers waiting at the selected station

    QProgressBar* progressBar; // Shows user how much of the goal they have completed
    QHBoxLayout* tipLayout; // Layout for storing tips for building a train network

    const int PASSENGER_ICON_WIDTH = 5; // size of passenger icons

    void createDockWindows(); // Sets up all dock windows
    void createLeftDockWindow(); // Sets up dock windows on the left
    void createRightDockWindow(); // Sets up dock windows on the right
    void createBottomDockWindow(); // Sets up dock windows on the bottom
    void createTipPopups(); // Sets up queue of tip popups

    QMessageBox* startWindow; // Introductory message shown to user on startup

    QQueue<QMessageBox*> tipMessageBoxQueue; // The queue for tips
    // Various tip popups
    QMessageBox* firstTip;
    QMessageBox* secondTip;
    QMessageBox* thirdTip;
    QMessageBox* fourthTip;
    QMessageBox* fifthTip;
    QMessageBox* sixthTip;
    QMessageBox* seventhTip;

public slots:
    /**
     * @brief Draws the passenger icons for the given station
     * @param station to draw passenger icons for
     */
    void drawStationPassenger(Station* station);

    /**
     * @brief Draws the passenger icons for the given station
     * @param station to draw passenger icons for
     */
    void drawTrainPassenger(Train* train);

    /**
     * @brief Adds a train button to the left dock
     */
    void addTrainButton();

    /**
     * @brief Updates data points on the right dock
     */
    void updateData(double, double, double);

    /**
     * @brief Shows the next tip to the user
     */
    void showTip();

    /**
     * @brief Updates the progress bar to the given value
     */
    void updateProgressBar(int);

    /**
     * @brief Fills the progress bar
     */
    void fillProgressBar();

    /**
     * @brief Empties the progress bar
     */
    void resetProgressBar();

    /**
     * @brief Redraws the track for the selected train
     */
    void redrawTracks();

    /**
     * @brief Enables the buttons to change the selected track
     */
    void enableTrackButtons();

signals:
    // Signals for each of the tips to show their popup
    void firstTipSignal();
    void secondTipSignal();
    void thirdTipSignal();
    void fourthTipSignal();
    void fifthTipSignal();
    void sixthTipSignal();
    void seventhTipSignal();
};
#endif // MAINWINDOW_H
