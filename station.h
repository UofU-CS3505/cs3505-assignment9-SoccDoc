#ifndef STATION_H
#define STATION_H
#include <QObject>
#include <QPoint>
#include <QRandomGenerator>
#include "passenger.h"

/**
 * @brief The Station class
 * @authors Benjamin Sidwell, Alex Fraser, Jason Lopex, Andy Liu, and Ryan Nip
 */
class Station : public QObject
{
    Q_OBJECT

public:
    Station(QObject *parent = nullptr);
    void update();
    int generatePassengerProbability = 8;

private:
    Passenger stationType;
    QList<Passenger> waitingPassengers;
    QPoint location;
    QRandomGenerator rand;
    void generatePassenger();
};

#endif // STATION_H
