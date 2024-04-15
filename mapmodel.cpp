#include "mapmodel.h"
#include <QWidget>

MapModel::MapModel(QWidget *parent) :
    QWidget(parent), updateTimer(this),
    drawer(new TrainDrawer(this))
{
    trains.append(new Train());

    connect(this, &MapModel::drawNewStation, drawer, &TrainDrawer::drawStations);

    // Setup and start update timer
    updateTimer.setInterval(MILISECONDS_TO_UPDATE);
    connect(&updateTimer, &QTimer::timeout, this, &MapModel::updateFrame);
    updateTimer.start();

    spawnStation();
}

void MapModel::updateFrame() {

}

void MapModel::trainButtonClicked(int id) {
    switch(id) {
    case 0:
        emit updateTrainDetails("Orange Train Details");
        break;
    case 1:
        emit updateTrainDetails("Blue Train Details");
        break;
    case 2:
        emit updateTrainDetails("Red Train Details");
        break;
    }
}

void MapModel::stationButtonClicked(int id) {
    switch(id) {
    case 0:
        emit updateStationDetails("Square Station Details");
        break;
    case 1:
        emit updateStationDetails("Circle Station Details");
        break;
    }
}

TrainDrawer* MapModel::getDrawer(){
    return drawer;
}

void MapModel::spawnStation() {
    QPoint newStationLocation;
    QRandomGenerator random;

    // Create initial random location
    newStationLocation.setX(random.bounded(drawer->size().width()));
    newStationLocation.setY(random.bounded(drawer->size().height()));

    // // Check if new station location is too close to other stations
    // while(checkStationProximity(newStationLocation)) {
    //     newStationLocation.setX(random.bounded(drawer->size().width()));
    //     newStationLocation.setY(random.bounded(drawer->size().width()));
    // }

    // Create station and add it to station list
    Station* newStation = new Station(this, QPoint(50, 50));
    stations.append(newStation);
    drawer->drawStations(newStation);
}

bool MapModel::checkStationProximity(QPoint newStationLocation) {
    foreach(Station station, stations) {
        // Calculate the distance between the station and the possible new station
        double xCord = std::pow(newStationLocation.x() - station.getLocation().x(), 2);
        double yCord = std::pow(newStationLocation.y() - station.getLocation().y(), 2);

        double distance = std::sqrt(xCord + yCord);

        // Check if this station is too close
        if(distance < 20)
            return false;
    }

    return true;
}
