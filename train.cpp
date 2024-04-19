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
    currentStation = connectedStations.first();
    currentStation->updateTrainPassengers(this);

    // Start train towards next station
    stationInList = 1;
    nextStation = connectedStations.at(stationInList);


    foreach (Station* s, connectedStations)
        qDebug() << s->getLocation();

    qDebug() << "next " << nextStation->getLocation();

    startTravel();
}

void Train::setImage(QString fileName) {
    // Setup train image
    QPixmap image;
    image.load(fileName);
    image = image.scaled(50, 25, Qt::KeepAspectRatio);

    trainImage = new QLabel();
    trainImage->setPixmap(image);
}

void Train::startTravel() {
    qDebug() << "curr " << currentStation->getLocation();
    qDebug() << "to " << nextStation->getLocation();
    qDebug(); // empty line

    // Setup animation and start it
    QPropertyAnimation* animation = new QPropertyAnimation(trainImage, "pos");
    animation->setDuration(5000);
    animation->setStartValue(currentStation->getLocation());
    animation->setEndValue(nextStation->getLocation());
    animation->start();

    connect(animation, &QPropertyAnimation::finished, this, &Train::endTravel);
}

void Train::endTravel() {
    // Make the station we are at the current station and update passengers
    currentStation = nextStation;
    currentStation->updateTrainPassengers(this);

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
