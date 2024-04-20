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
    Train(QObject *parent = nullptr, QPropertyAnimation* anim = nullptr);

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
    QColor line;

private:
    Station* pastStation; // station train is departing from
    Station* nextStation; // station train is heading towards
    int nextStationIndex; // where the train is in its station list

    QList<Passenger> passengers; // list of current passengers on this train
    QList<Station*> connectedStations; // list of all stations train is connected to
    bool trainIsGoingBackwards; // Whether the train is going backwards or not

    QPropertyAnimation* animation; // animates the train moving between stations
    const double SPEED = 0.15; // speed of the train
    const int CAPACITY = 10; // Max number of passengers the train can hold

    /**
     * @brief gets the distance between two given points
     */
    double getDistance(QPoint p1, QPoint p2);

public slots:
    /**
     * @brief on/offboards passengers at the arrived station then calls startTravel()
     */
    void endTravel();
};

#endif // TRAIN_H
