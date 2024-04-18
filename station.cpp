#include "station.h"
#include "train.h"



Station::Station(QObject *parent, QPoint _location, QRandomGenerator randomGenerator) :
    QObject(parent), location(_location), rand(randomGenerator)
{
    stationType = Passenger(rand() % last);
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
    int randNum = rand.bounded(3000);
    if(randNum < GENERATE_PASSENGER_PROBABILITY && waitingPassengers.size() < 8){
        generatePassenger();
        return true;
    }

    return false;
}

void Station::generatePassenger(){
    Passenger passengerToAdd = Passenger(rand() % last);

    //regenerates the pasenger while its he same type as the station
    while(passengerToAdd == stationType){
        passengerToAdd = Passenger(rand() % last);
    }

    waitingPassengers.append(passengerToAdd);
}

void Station::updateTrainPassengers(Train* trainToLoad){
    if(!elapsedTimer.isValid()){
        elapsedTimer.start();
    }else{
        waitTime = elapsedTimer.elapsed();
        elapsedTimer.restart();
    }
    numberOfPassengerOffloaded = 0;

    //load all the passengers that can go to their correct destination on the train.
    for(int j = waitingPassengers.size(); j > 0; j--){
        Passenger passengerToAdd = waitingPassengers.at(j);
        QList<Station> stations = trainToLoad->getConnectedStations();

        for(int i = 0; i < trainToLoad->getConnectedStations().size(); i++){
             const Station* connectedStation = &(stations.at(i));

            if(connectedStation->stationType == passengerToAdd){
                 trainToLoad->boardPassenger(passengerToAdd);
                waitingPassengers.removeAt(j);
                 numberOfPassengerOffloaded++;
            }
        }
    }

    //unload all the passengers that have the same type as the station
    trainToLoad->removePassengers(stationType);
}

Passenger Station::getStationType(){
    return stationType;
}

QPoint Station::getLocation() {
    return location;
}

double Station::getThroughput(){
    if (!waitTime || waitTime == 0){
        return -1;
    }
    throughput = (numberOfPassengerOffloaded/waitTime);
    return throughput;
}

double Station::getWaitTime(){
    return waitTime;
}

QList<Passenger> Station::getPassengers(){
    return waitingPassengers;
}
