#include "station.h"
#include "train.h"


Station::Station(QObject *parent, QPoint _location, QRandomGenerator randomGenerator) :
    QObject(parent), location(_location), rand(randomGenerator)
{
    stationType = Passenger(rand() % last);
}



void Station::update(){
    //generatePassengers if the correct probability was hit.
    int randNum = rand.bounded(100);
    if(randNum < generatePassengerProbability){
        generatePassenger();
    }

}

void Station::generatePassenger(){
    Passenger passengerToAdd = Passenger(rand() % last);

    //regenerates the pasenger while its he same type as the station
    while(passengerToAdd == stationType){
        passengerToAdd = Passenger(rand() % last);
    }


    waitingPassengers.append(passengerToAdd);
}

void Station::updateTrainPassengers(Train trainToLoad){


    //load all the passengers that can go to their correct destination on the train.
    foreach(Passenger passengerToAdd, waitingPassengers){
         QList<Station> stations = trainToLoad.getConnectedStations();

        for(int i = 0; i < trainToLoad.getConnectedStations().size(); i++){
             const Station* connectedStation = &(stations.at(i));

            if(connectedStation->stationType == passengerToAdd){
                trainToLoad.boardPassenger(passengerToAdd);
            }
        }
    }

    //unload all the passengers that have the same type as the station
    trainToLoad.removePassengers(stationType);
}

Passenger Station::getStationType(){
    return stationType;
}

QPoint Station::getLocation(){
    return location;
}
