#ifndef STATION_H
#define STATION_H
#include <QObject>
#include <QPoint>
#include <QRandomGenerator>
#include "passenger.h"
#include <QElapsedTimer>
#include <QtGlobal>
#include <QLabel>
class Train;

/**
 * @brief The Station class keeps track of passengers at the station and calculates some data for the station
 * @authors Benjamin Sidwell, Alex Fraser, Jason Lopex, Andy Liu, and Ryan Nip
 */
class Station : public QObject
{
    Q_OBJECT

public:
    /**
     * @param parent this objects parent
     * @param _location the location on the canvas of this station
     * @param passenger the type of passenger that this station accepts
     */
    Station(QObject *parent = nullptr,
            QPoint _location = QPoint(0,0),
            Passenger passenger = Passenger(0));

    /**
     * @brief Copy constructor for Station
     * @param other station to copy
     */
    Station(const Station& other);

    /**
     * @brief Assignment operator for station
     * @param station to copy
     * @return the copied station
     */
    Station& operator=(const Station& that);

    /**
     * @brief Randomly decides whether or not to add a passenger to this station
     */
    void update();

    /**
     * @brief Returns the passenger type this station accepts
     * @return the passenger type this station accepts
     */
    Passenger getStationType();

    /**
     * @brief Returns the location of this station on the canvas
     * @return this station's location
     */
    QPoint getLocation();

    /**
     * @return The current throughput of this station
     */
    double getThroughput();

    /**
     * @return The average wait time of this station
     */
    double getWaitTime();
    
    /**
     * @brief Loads/unloads passengers to/from the given train
     * @param trainToLoad the train to load/unload passengers
     */
    void updateTrainPassengers(Train* trainToLoad);

    /**
     * @brief Helper for updateTrainPassengers that does the loading of passengers
     * @param trainToLoad the train to load passengers onto
     * @param type the type of passenger to load
     * @return the number of passengers loaded
     */
    int loadPassengers(Train* trainToLoad, Passenger type);

    /**
     * @return The number of passengers waiting at this station
     */
    int returnWaitingSize();

    /**
     * @return the number of passenger icons
     */
    int getNumberOfPassengerIcons();

    /**
     * @brief Clears all of the passenger icons
     */
    void clearPassengerIcons();

    /**
     * @brief Adds an icon to the list of passenger icons
     * @param icon the label to add
     */
    void addPassengerIcon(QLabel* icon);

    /**
     * @return List of passengers at this station
     */
    QList<Passenger> getPassengers();

    /**
     * @return true if the passengers have been changed, false otherwise
     */
    bool passengersChanged();

    /**
     * @brief Marks the passenger list as unchanged
     */
    void updatedPassengers();

private:
    QList<QLabel*> passengerIcons; // List of icons to display this stations passengers
    QList<Passenger> waitingPassengers; // List of passengers at this station
    bool passengersUpdated = false; // Whether station's passengers have been edited or not
    Passenger stationType; // The type of passenger this station is accepting
    QPoint location; // The location of this station

    QElapsedTimer elapsedTimer; // Time since last train unloaded passengers
    double numberOfPassengerOffloaded = -1; // The number of passengers off loaded on the last train
    double waitTime = -1; // Average wait time of this statino
    double throughput = 0; // Current throughput of this station
    const int GENERATE_PASSENGER_PROBABILITY = 40; // The chance of generating a passenger
    const int PASSENGER_LIMIT = 8; // The max number of passengers at a station
    QRandomGenerator rand; // This stations random generator (for generating passengers)

    /**
     * @brief Generates a random passenger type at this station
     */
    void generatePassenger();

signals:
    /**
     * @brief Gives the number of passengers delivered to this station
     */
    void passengerDelivered(int);
};

#endif // STATION_H
