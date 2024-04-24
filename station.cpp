#include "station.h"
#include "train.h"

Station::Station(QObject *parent, QPoint _location, Passenger passenger) :
    QObject(parent), stationType(passenger), location(_location)
{
    rand = QRandomGenerator::securelySeeded();
    waitTime = 0;
    throughput = 0;
    numberOfPassengerOffloaded = 0;
}

Station::Station(const Station& other) : QObject(other.parent()) {
    location = other.location;
    rand = other.rand;
}

Station& Station::operator=(const Station& other) {
    // Copy
    Station copyOther(other);

    // Swap
    this->setParent(other.parent());
    std::swap(location, copyOther.location);
    std::swap(rand, copyOther.rand);

    return *this;
}

int Station::returnWaitingSize(){
    return waitingPassengers.size();
}

void Station::update(){
    //generatePassengers if the correct probability was hit.
    int randNum = rand.bounded(10000);
    if(randNum >= GENERATE_PASSENGER_PROBABILITY)
        return;

    // If we have reached the passenger limit, decrease progress bar
    if (waitingPassengers.size() > PASSENGER_LIMIT) {
        emit passengerDelivered(-1);
        return;
    }

    // Generate a passenger
    generatePassenger();
    passengersUpdated = true;
}

void Station::generatePassenger(){
    Passenger passengerToAdd = Passenger(rand() % Last);

    //regenerates the pasenger while its he same type as the station
    while(passengerToAdd == stationType)
        passengerToAdd = Passenger(rand() % Last);

    waitingPassengers.append(passengerToAdd);
}

void Station::updateTrainPassengers(Train* trainToLoad){
    // Starts a timer to record how long it takes for a train to come
    if(!elapsedTimer.isValid())
        elapsedTimer.start();
    else {
        waitTime = (elapsedTimer.elapsed());
        elapsedTimer.restart();
    }

    //unload all the passengers that have the same type as the station
    emit passengerDelivered(trainToLoad->removePassengers(stationType));

    //load all the passengers that can go to their correct destination on the train.
    numberOfPassengerOffloaded = 0;
    for (int passengerInt = Passenger::Square; passengerInt != Passenger::Last; passengerInt++) {
        QList<Station*> stations = trainToLoad->getConnectedStations();

        // Check if the train has a station with the current passenger type
        foreach (Station* station, stations) {
            if (station->getStationType() == passengerInt) {
                // It does, load all passengers of that type onto the train
                numberOfPassengerOffloaded += loadPassengers(trainToLoad, (Passenger)passengerInt);
                break;
            }
        }
    }
}

int Station::loadPassengers(Train* trainToLoad, Passenger type) {
    int numberOfPassengersLoaded = 0;
    while(true) {
        // Check if there are any more passengers of this type
        if (!waitingPassengers.removeOne(type))
            break;

        // Try to board a passenger onto the train
        if (!trainToLoad->boardPassenger(type))
            break;

        numberOfPassengersLoaded++;
    }

    // Redraw passengers
    passengersUpdated = true;
    return numberOfPassengersLoaded;
}

Passenger Station::getStationType(){
    return stationType;
}

QPoint Station::getLocation() {
    return location;
}

double Station::getThroughput(){
    // Check if throughput needs to be updated
    if (!waitTime || waitTime <= 0)
        return throughput;

    // Re-calculate throughput
    throughput = (numberOfPassengerOffloaded/(waitTime/1000));
    return throughput;
}

double Station::getWaitTime(){
    return (waitTime / 1000);
}

QList<Passenger> Station::getPassengers(){
    return waitingPassengers;
}

int Station::getNumberOfPassengerIcons() {
    return passengerIcons.size();
}

void Station::clearPassengerIcons() {
    foreach (QLabel* label, passengerIcons)
        delete label;

    passengerIcons.clear();
}

void Station::addPassengerIcon(QLabel* icon) {
    passengerIcons.append(icon);
}

bool Station::passengersChanged() {
    return passengersUpdated;
}

void Station::updatedPassengers() {
    passengersUpdated = false;
}
