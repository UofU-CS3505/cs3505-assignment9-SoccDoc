#ifndef TRAIN_H
#define TRAIN_H
#include <QObject>
#include <QLabel>
#include <QPropertyAnimation>
#include "passenger.h"
#include "station.h"

/**
 * @brief The Train class defines a train object that moves passengers along a list of given stations.
 * @authors Benjamin Sidwell, Alex Fraser, Jason Lopex, Andy Liu, and Ryan Nip
 */
class Train : public QObject
{
    Q_OBJECT

public:
    Train(QObject *parent = nullptr, QLabel* trainLabel = nullptr, QPixmap image = QPixmap(), QColor line = QColor(0, 0, 0));

    /**
     * @brief Replaces the current stations with the given stations.
     * @param stations the new list of stations
     */
    void changeStations(QList<Station*> stations);

    /**
     * @brief starts the train moving towards its next station
     */
    void startTravel();

    /**
     * @brief stops the train animation and deletes its label
     */
    void stopTravel();

    /**
     * @brief Returns a list of all of this train's connected stations
     * @return a list of all of this train's connected stations
     */
    QList<Station*> getConnectedStations();

    /**
     * @brief Boards a passenger onto the train
     * @param passenger the type of passenger to board
     * @return true if all passengers was loaded, false if not
     */
    bool boardPassenger(Passenger passenger);

    /**
     * @brief Removes all passengers of the given type from this train
     * @param passengerType the type of passenger to remove
     * @return number of passengers removed
     */
    int removePassengers(Passenger passengerType);

    /**
     * @brief Gets the color of this trains line
     * @return this trains line color
     */
    QColor getLineColor();

    /**
     * @brief Gets the train's list of passengers
     * @return the passenger list
     */
    QList<Passenger> getPassengers();

    /**
     * @brief Gets the train's animation
     * @return the train animation
     */
    QPropertyAnimation* getAnimation();

    /**
     * @brief Gets the list of train passenger icons
     * @return returns the list containing the passenger icons of the train
     */
    QList<QLabel*> getTrainPassengerIcons();

    QList<QLabel*> trainPassengerIcons; //List of Qlabels with images on them representing the train's passengers

private:
    Station* pastStation; // station train is departing from
    Station* nextStation; // station train is heading towards
    int nextStationIndex; // where the train is in its station list

    QColor lineColor; // The color of this trains line

    QList<Passenger> passengers; // list of current passengers on this train
    const int CAPACITY = 6; // Max number of passengers the train can hold

    QList<Station*> connectedStations; // list of all stations train is connected to
    bool trainIsGoingBackwards; // Whether the train is going backwards or not

    QPropertyAnimation* animation; // animates the train moving between stations
    QLabel* trainImage; // The image for the train
    const double SPEED = 0.15; // speed of the train
    const int STATION_DELAY = 1000; // Ms delay for train when it reaches a station

    /**
     * @brief gets the distance between two given points
     */
    double getDistance(QPoint p1, QPoint p2);

    /**
     * @brief Emits the signal to draw and animate the train passengers and resumes the trains animation
     */
    void resumeAnimations();

public slots:
    /**
     * @brief on/offboards passengers at the arrived station then calls startTravel()
     */
    void endTravel();

signals:
    // Sents out a signal that contains this train object
    void passengersBoarding(Train* train);
};

#endif // TRAIN_H
