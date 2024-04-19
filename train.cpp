#include "train.h"
#include <QWidget>
#include <QPropertyAnimation>

Train::Train(QObject *parent) : QObject(parent) {}

void Train::boardPassenger(Passenger passenger) {
    passengers.append(passenger);
}

QList<Station*> Train::getConnectedStations() {
    return connectedStations;
}

int Train::removePassengers(Passenger passengerType) {
    return passengers.removeAll(passengerType);
}

void Train::changeStations(QList<Station*> stations) {
    // Connect the new stations and clear the passengers
    connectedStations = stations;
    passengers.clear();

    // Load passengers from first station
    pastStation = connectedStations.first();
    pastStation->updateTrainPassengers(this);

    // Start train towards next station
    stationInList = 1;
    nextStation = connectedStations.at(stationInList);
    startTravel();
}

void Train::setAnimation(QPropertyAnimation* anim) {
    animation = anim;
}

void Train::startTravel() {
    // Calculate the train's travel time
    double stationDistance = getDistance(pastStation->getLocation(), nextStation->getLocation());
    int traveltime = stationDistance / SPEED;

    // Setup animation and start it
    animation->setDuration(traveltime);
    animation->setStartValue(pastStation->getLocation());
    animation->setEndValue(nextStation->getLocation());
    animation->start();

    connect(animation, &QPropertyAnimation::finished, this, &Train::endTravel);
}

void Train::endTravel() {
    // Make the station we are at the current station and update passengers
    pastStation = nextStation;
    pastStation->updateTrainPassengers(this);

    // Set current and next station
    stationInList++;
    if (stationInList == connectedStations.size())
        stationInList = 0;

    // Set the next station and leave station
    nextStation = connectedStations.at(stationInList);
    startTravel();
}

double Train::getDistance(QPoint p1, QPoint p2) {
    double xCord = std::pow(p1.x() - p2.x(), 2);
    double yCord = std::pow(p1.y() - p2.y(), 2);

    return std::sqrt(xCord + yCord);
}
