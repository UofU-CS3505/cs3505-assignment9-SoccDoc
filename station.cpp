#include "station.h"
#include "train.h"

Station::Station(QObject *parent, QPoint _location, Passenger passenger) :
    QObject(parent), location(_location)
{
    rand = QRandomGenerator::securelySeeded();
    stationType = passenger;
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

bool Station::update(){
    //generatePassengers if the correct probability was hit.
    int randNum = rand.bounded(1000);
    if(randNum < GENERATE_PASSENGER_PROBABILITY && waitingPassengers.size() < 8){
        generatePassenger();
        return true;
    }
    return false;
}

void Station::generatePassenger(){
    Passenger passengerToAdd = Passenger(rand() % Last);

    //regenerates the pasenger while its he same type as the station
    while(passengerToAdd == stationType){
        passengerToAdd = Passenger(rand() % Last);
    }

    waitingPassengers.append(passengerToAdd);
}

void Station::updateTrainPassengers(Train* trainToLoad){
    if(!elapsedTimer.isValid()){
        elapsedTimer.start();
    }else{
        waitTime = (elapsedTimer.elapsed());
        elapsedTimer.restart();
    }
    numberOfPassengerOffloaded = 0;

    //unload all the passengers that have the same type as the station
    emit passengerDelivered(trainToLoad->removePassengers(stationType));

    //load all the passengers that can go to their correct destination on the train.
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
    int passengersToBoard = waitingPassengers.removeAll(type);

    // Load the passengers onto the train
    for (int i = 0; i < passengersToBoard; i++)
        // Check if we succeed to load a passenger
        if (!trainToLoad->boardPassenger(type))
            return i; // we failed, stop loading

    // All passengers successfully loaded
    return passengersToBoard;
}

Passenger Station::getStationType(){
    return stationType;
}

QPoint Station::getLocation() {
    return location;
}

double Station::getThroughput(){
    if (!waitTime || waitTime == 0){
        return 0;
    }
    throughput = (numberOfPassengerOffloaded/(waitTime)/1000);
    return throughput;
}

double Station::getWaitTime(){
    return (waitTime / 1000);
}

QList<Passenger> Station::getPassengers(){
    return waitingPassengers;
}
