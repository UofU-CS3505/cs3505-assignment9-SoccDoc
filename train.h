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

    /**
     * @brief Replaces the current stations with the given stations.
     * @param stations the new list of stations
     */
    void changeStations(QList<Station*> stations);

    QList<Station*> getConnectedStations();
    void boardPassenger(Passenger passenger);
    void removePassengers(Passenger passengerType);
    void update();

private:
    Station* nextStation;
    QPoint location;
    QList<Passenger> passengers;
    QList<Station*> connectedStations;

    double getDistance(QPoint p1, QPoint p2);

    const double SPEED = 2.0;
};

#endif // TRAIN_H
