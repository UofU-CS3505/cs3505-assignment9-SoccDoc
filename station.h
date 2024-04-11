#ifndef STATION_H
#define STATION_H
#include <QObject>
#include <QPoint>
#include "passenger.h"

class Station : public QObject
{
    Q_OBJECT

public:
    Station();

private:
    Passenger stationType;
    QList<Passenger> waitingPassengers;
    QPoint location;
};

#endif // STATION_H
