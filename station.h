#ifndef STATION_H
#define STATION_H
#include <QObject>
#include <QPoint>
#include <QRandomGenerator>
#include "passenger.h"
#include <QElapsedTimer>
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

    Passenger stationType;
    void update();
    Passenger getStationType();
    QPoint getLocation();
    double getThroughput();
    double getWaitTime();
    int generatePassengerProbability = 8;
    void updateTrainPassengers(Train* trainToLoad);

private:
    QList<Passenger> waitingPassengers;
    QPoint location;
    QRandomGenerator rand;
    void generatePassenger();
    int amountOfUnloadedPassengers;

    QElapsedTimer elapsedTimer;
    int numberOfPassengerOffloaded;
    double waitTime;
    double throughput;
};

#endif // STATION_H
