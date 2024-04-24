#include "train.h"
#include <QTimer>
#include <QWidget>
#include <QPropertyAnimation>

Train::Train(QObject *parent, QLabel* trainLabel, QPixmap image, QColor color) :
    QObject(parent), lineColor(color),  trainImage(trainLabel)
{
    // Set up animation object
    trainImage->setPixmap(image);
    trainImage->show();
    animation = new QPropertyAnimation(trainImage, "pos");

    // Setup animation connection
    connect(animation, &QPropertyAnimation::finished, this, &Train::endTravel);
}

bool Train::boardPassenger(Passenger passenger) {
    // Check if the train is full
    if (passengers.size() == CAPACITY)
        return false;

    // Board a passenger
    passengers.append(passenger);
    return true;
}

QList<Station*> Train::getConnectedStations() {
    return connectedStations;
}

QColor Train::getLineColor() {
    return lineColor;
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
    trainIsGoingBackwards = false;
    nextStationIndex = 1;
    nextStation = connectedStations.at(nextStationIndex);
    startTravel();
}

void Train::startTravel() {
    // Calculate the train's travel time
    double stationDistance = getDistance(pastStation->getLocation(), nextStation->getLocation());
    int traveltime = stationDistance / SPEED;

    // Setup animation and start it
    animation->setDuration(traveltime);
    animation->setStartValue(pastStation->getLocation());
    animation->setEndValue(nextStation->getLocation());

    // Setup animation delay
    animation->start();
    animation->pause();
    QTimer::singleShot(STATION_DELAY, this, &Train::resumeAnimations);
}

void Train::endTravel() {
    // Make the station we are at the past station and update passengers
    pastStation = nextStation;
    pastStation->updateTrainPassengers(this);

    // Check if the train has reached the end of the line, turn around if it has
    if (nextStationIndex == connectedStations.size() - 1)
        trainIsGoingBackwards = true;
    if (nextStationIndex == 0)
        trainIsGoingBackwards = false;

    // Move the train forward
    if (!trainIsGoingBackwards)
        nextStationIndex++;
    if (trainIsGoingBackwards)
        nextStationIndex--;

    // Set the next station and leave station
    nextStation = connectedStations.at(nextStationIndex);
    startTravel();
}

void Train::stopTravel() {
    clearPassengerIcons();
    animation->stop();
    delete trainImage;
}

void Train::resumeAnimations() {
    animation->resume();
    emit passengersBoarding(this);
}

double Train::getDistance(QPoint p1, QPoint p2) {
    double xCord = std::pow(p1.x() - p2.x(), 2);
    double yCord = std::pow(p1.y() - p2.y(), 2);

    return std::sqrt(xCord + yCord);
}

QList<Passenger> Train::getPassengers(){
    return passengers;
}

QPropertyAnimation* Train::getAnimation(){
    return animation;
}

void Train::clearPassengerIcons() {
    foreach (QLabel* label, passengerIcons)
        delete label;

    passengerIcons.clear();
}

void Train::addPassengerIcon(QLabel* icon) {
    passengerIcons.append(icon);
}

int Train::getNumberOfPassengerIcons() {
    return passengerIcons.size();
}
