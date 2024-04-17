#include "train.h"
#include <QWidget>

Train::Train(QObject *parent) : QObject(parent) {}

void Train::boardPassenger(Passenger passenger) {
    passengers.append(passenger);
}

QList<Station> Train::getConnectedStations() {
    return connectedStations;
}

void Train::removePassengers(Passenger passengerType) {
    passengers.removeAll(passengerType);
}

void Train::changeStations(QList<Station> stations) {
    // Connect the new stations and clear the passengers
    connectedStations = stations;
    passengers.clear();

    // Load passengers from first station
    connectedStations.first().updateTrainPassengers(this);

    // Start train towards next station
    previousStation = connectedStations.first().getLocation();
    nextStation = connectedStations[1].getLocation();
    location = previousStation;
}

void Train::update() {
    QPoint previousStationLocation = previousStation;
    QPoint nextStationLocation = nextStation;

    double distance = getDistance(previousStationLocation, nextStationLocation);
    double relativeSpeed = SPEED / distance;

    QPoint newLocation = relativeSpeed * location + (1 - relativeSpeed) * nextStationLocation;
    location = newLocation;
    qDebug() << location.x() << ", " << location.y();
}

double Train::getDistance(QPoint p1, QPoint p2) {
    double xCord = std::pow(p1.x() - p2.x(), 2);
    double yCord = std::pow(p1.y() - p2.y(), 2);

    return std::sqrt(xCord + yCord);
}
