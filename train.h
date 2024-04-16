#ifndef TRAIN_H
#define TRAIN_H
#include <QObject>
#include "passenger.h"
#include "station.h"

/**
 * @brief The Train class
 * @authors Benjamin Sidwell, Alex Fraser, Jason Lopex, Andy Liu, and Ryan Nip
 */
class Train : public QObject
{
    Q_OBJECT

public:
    Train(QObject *parent = nullptr);

    void addStations(QList<Station> stations);
    void clearStations();
    QList<Station> getConnectedStations();
    void boardPassenger(Passenger passenger);
    void removePassengers(Passenger passengerType);

private:
    Station currentDestination;
    QList<Passenger> cargo;
    QList<Station> connectedStations;

    void deliverPassengers();
    void boardPassengers();
};

#endif // TRAIN_H
