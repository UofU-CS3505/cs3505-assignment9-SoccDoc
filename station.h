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

    bool update();
    Passenger getStationType();
    QPoint getLocation();
    const int GENERATE_PASSENGER_PROBABILITY = 8;

    double getThroughput();
    double getWaitTime();
    
    void updateTrainPassengers(Train* trainToLoad);
    int loadPassengers(Train* trainToLoad, Passenger type);

    int returnWaitingSize();
    QList<Passenger> getPassengers();

private:
    QList<Passenger> waitingPassengers;
    Passenger stationType;
    QPoint location;
    QRandomGenerator rand;
    void generatePassenger();
    int amountOfUnloadedPassengers;

    QElapsedTimer elapsedTimer;
    int numberOfPassengerOffloaded = -1;
    quint64 waitTime = -1;
    int throughput = 0;

signals:
    void passengerDelivered(int);
};

#endif // STATION_H
