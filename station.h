#ifndef STATION_H
#define STATION_H
#include <QObject>
#include <QPoint>
#include <QRandomGenerator>
#include "passenger.h"
class Train;

/**
 * @brief The Station class
 * @authors Benjamin Sidwell, Alex Fraser, Jason Lopex, Andy Liu, and Ryan Nip
 */
class Station : public QObject
{
    Q_OBJECT

public:
    Station(QObject *parent = nullptr);
    Passenger stationType;
    void update();
    int generatePassengerProbability = 8;
    void updateTrainPassengers(Train trainToLoad);

private:
    QList<Passenger> waitingPassengers;
    QPoint location;
    QRandomGenerator rand;
    void generatePassenger();
};

#endif // STATION_H
