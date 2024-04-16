#include "train.h"

Train::Train(QObject *parent) : QObject(parent) {}

void Train::boardPassenger(Passenger passenger) {
    cargo.append(passenger);
}

QList<Station> Train::getConnectedStations() {
    return connectedStations;
}

void Train::removePassengers(Passenger passengerType) {
    cargo.removeAll(passengerType);
}
