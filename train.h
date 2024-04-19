#ifndef TRAIN_H
#define TRAIN_H
#include <QObject>
#include <QLabel>
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

    void startTravel();
    void setImage(QString fileName);

    QList<Station*> getConnectedStations();
    void boardPassenger(Passenger passenger);
    int removePassengers(Passenger passengerType);

private:
    Station* currentStation;
    Station* nextStation;
    int stationInList;
    QPoint location;
    QList<Passenger> passengers;
    QList<Station*> connectedStations;

    QLabel* trainImage;

    double getDistance(QPoint p1, QPoint p2);

    const double SPEED = 2.0;

public slots:
    void endTravel();
};

#endif // TRAIN_H
