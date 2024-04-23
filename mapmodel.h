#ifndef MAPMODEL_H
#define MAPMODEL_H
#include <QWidget>
#include <QTimer>
#include <QRandomGenerator>
#include <QColor>
#include <QQueue>
#include "station.h"
#include "train.h"
#include "TrainDrawer.h"

/**
 * @brief The MapModel class manages all of the logic of where to draw and what data to display
 * @authors Benjamin Sidwell, Alex Fraser, Jason Lopex, Andy Liu, and Ryan Nip
 */
class MapModel: public QWidget
{
    Q_OBJECT

public:
    MapModel(QWidget *parent = nullptr);

    /**
     * @brief Returns the TrainDrawer object
     * @return the TrainDrawer object
     */
    TrainDrawer* getDrawer();

    /**
     * @brief Returns the station at the given point
     * @param point the point to find a station at
     * @return the station at the given point
     */
    Station* getStation(QPoint point);

private:
    QTimer updateTimer; // Timer to update the canvas and data
    int numberOfUnusedTrains = 3; // Number of trains the user can still place
    QList<Train*> trains; // All of the placed trains
    QList<Station*> stations; // All of the spawned stations
    Station* selectedStation = nullptr; // The current station the user has selected
    TrainDrawer *drawer; // The drawer object that draws tracks
    QList<QPoint> stationPoints; // A list of all the station locations
    QRandomGenerator rand; // A random generator for spawning stations and passengers

    int numberOfPassengersDelivered; // The number of passengers the user has delivered since the last completed goal
    int currentPassengerGoal; // The current passenger goal to fill the progress bar
    QQueue<int> passengerGoals; // A queue of the passenger goals to fill the progress bar

    const int MILISECONDS_TO_UPDATE = (1000/60); // How often the timer updates the canvas
    const int STATION_DISTANCE = 120; // Minimum distance between stations
    const int STATION_EDGE_BUFFER = 40; // Minimum distance between stations and the edge of the canvas
    QColor currentLine; // The color of the current train line

    /**
     * @brief Adds a train to ride the given list of stations
     * @param trainStations the stations for the train to ride between
     */
    void addTrainToLine(QList<Station*> trainStations);

    /**
     * @brief spawns a new station and draws it
     */
    void spawnStation();
    void spawnTriangleStation();
    void spawnCircleStation();
    void spawnSquareStation();

    /**
     * @brief Checks if a given point is too close to current stations
     * @param the point to check
     * @return false if station is too close, true otherwise
     */
    bool stationLocationIsGood(QPoint newStationLocation);

public slots:

    void selectStation(QPoint point);
    /**
     * @brief Updates the passengers in stations
     */
    void updateFrame();

    /**
     * @brief Detects which train radio button was clicked and updates train details
     */
    void trainButtonClicked(int);

    /**
     * @brief Detects which station radio button was clicked and updates station details
     */
    void stationButtonClicked(int);

    /**
     * @brief Displays a blast of confetti on the screen
     */
    void confetti();

    /**
     * @brief Checks for stations at the given points
     */
    void drawStationsOnLine(QList<QPoint>);

    /**
     * @brief Checks if the progress bar is full yet
     */
    void checkProgressBar(int);

    /**
     * @brief Updates how many passengers have been delivered
     */
    void passengerDelivered(int);

    void redrawTrack();

signals:
    /**
     * @brief updates data tab
     */
    void updateData(double, double, double);

    /**
     * @brief Updates the details for the given train
     */
    void updateTrainDetails(QString);

    /**
     * @brief Updates the details for the given station
     */
    void updateStationDetails(QString);

    /**
     * @brief Displays a new tip for the user
     */
    void showNewTip();

    /**
     * @brief Resets the progress bar for the next goal
     */
    void restartProgressBar();

    /**
     * @brief Updates how much of the users goal is complete
     */
    void updateProgressBar(int);

    /**
     * @brief Draws the given station with the given passenger type
     * @param station The station to draw
     * @param passengerType The type of the station to draw
     */
    void drawStationPassenger(Station* station);
    void redraw(Station* station);

    void enableTrackButtonsSignal();
};

#endif // MAPMODEL_H
