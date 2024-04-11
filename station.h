#ifndef STATION_H
#define STATION_H
#include <QObject>
#include <QPoint>
#include "passenger.h"

/**
 * @brief The Station class
 * @authors Benjamin Sidwell, Alex Fraser, Jason Lopex, Andy Liu, and Ryan Nip
 */
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
