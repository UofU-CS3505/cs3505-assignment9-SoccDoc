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

void Train::removePassengers(Passenger passengerType) {
    passengers.removeAll(passengerType);
}

void Train::changeStations(QList<Station*> stations) {
    // Connect the new stations and clear the passengers
    connectedStations = stations;
    passengers.clear();

    // Load passengers from first station
    currentStation = connectedStations.first();
    currentStation->updateTrainPassengers(this);

    // Start train towards next station
    stationInList = 1;
    nextStation = connectedStations.at(stationInList);

    foreach (Station* s, connectedStations)
        qDebug() << s->getLocation();

    qDebug() << "next " << nextStation->getLocation();
}

void Train::setImage(QPixmap image) {
    trainImage = new QLabel();
    trainImage->setPixmap(image);
}

void Train::startTravel() {
    QPropertyAnimation* animation = new QPropertyAnimation(trainImage, "pos");
    animation->setDuration(3000);
    animation->setStartValue(currentStation->getLocation());
    animation->setEndValue(nextStation->getLocation());
    animation->start();

    connect(animation, &QPropertyAnimation::finished, this, &Train::endTravel);
}

void Train::endTravel() {
    currentStation = nextStation;
    currentStation->updateTrainPassengers(this);

    // Set current and next station
    stationInList++;
    currentStation->updateTrainPassengers(this);
    nextStation = connectedStations.at(stationInList);

    startTravel();
}

void Train::update() {
    foreach (Station* s, connectedStations)
        qDebug() << s->getLocation();

    QPoint nextStationLocation = nextStation->getLocation();
    qDebug() << "now next " << nextStationLocation;

    // Find the distance
    double distance = getDistance(location, nextStationLocation);
    double relativeSpeed = SPEED / distance;

    // Set the new train location
    QPoint newLocation = relativeSpeed * location + (1 - relativeSpeed) * nextStationLocation;
    location = newLocation;
    qDebug() << location << " going to " << nextStation;
    qDebug() << location.x() << ", " << location.y();
    qDebug();
}

double Train::getDistance(QPoint p1, QPoint p2) {
    double xCord = std::pow(p1.x() - p2.x(), 2);
    double yCord = std::pow(p1.y() - p2.y(), 2);

    return std::sqrt(xCord + yCord);
}
