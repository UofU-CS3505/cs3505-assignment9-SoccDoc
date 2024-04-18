#ifndef STATION_H
#define STATION_H
#include <QObject>
#include <QPoint>
#include <QRandomGenerator>
#include "passenger.h"
#include <QElapsedTimer>
#include <QtGlobal>
class Train;

/**
 * @brief The Station class
 * @authors Benjamin Sidwell, Alex Fraser, Jason Lopex, Andy Liu, and Ryan Nip
 */
class Station : public QObject
{
    Q_OBJECT

public:
    Station(QObject *parent = nullptr,
            QPoint _location = QPoint(0,0),
            QRandomGenerator randomGenerator = *QRandomGenerator::global());

    Station(const Station& other);
    Station& operator=(const Station& that);

    Passenger stationType;
    void update();
    Passenger getStationType();
    QPoint getLocation();
    const int GENERATE_PASSENGER_PROBABILITY = 8;

    double getThroughput();
    double getWaitTime();
    
    void updateTrainPassengers(Train* trainToLoad);

private:
    QList<Passenger> waitingPassengers;
    QPoint location;
    QRandomGenerator rand;
    void generatePassenger();
    int amountOfUnloadedPassengers;

    QElapsedTimer elapsedTimer;
    int numberOfPassengerOffloaded = 0;
    quint64 waitTime = 0;
    int throughput = 0;
};

#endif // STATION_H
