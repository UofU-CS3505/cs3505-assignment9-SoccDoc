#include "station.h"


Station::Station(QObject *parent) : QObject(parent) {


}



void Station::update(){
    int randNum = rand.bounded(100);
    if(randNum < generatePassengerProbability){
        generatePassenger();
    }

}

void Station::generatePassenger(){
    Passenger passengerToAdd = Passenger(rand() % last);

    //TODO: check that the passenger generated is not the same as the station type. IE: a triangle wasnt generated at a triangle station.

    waitingPassengers.append(passengerToAdd);
}

Passenger Station::getStationType(){

}

QPoint Station::getLocation(){

}
