#include "mapmodel.h"
#include <QWidget>

MapModel::MapModel(QWidget *parent) :
    QWidget(parent), updateTimer(this),
    drawer(new TrainDrawer(this))
{
    trains.append(new Train());

    // Setup and start update timer
    updateTimer.setInterval(MILISECONDS_TO_UPDATE);
    connect(&updateTimer, &QTimer::timeout, this, &MapModel::updateFrame);
    updateTimer.start();

    // Spawn some initial stations
    for (int i = 0; i < 10; i++)
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
    // Create initial random location
    QRandomGenerator random;
    int x = random.bounded(drawer->size().width());
    int y = random.bounded(drawer->size().height());

    QPoint newStationLocation;
    newStationLocation.setX(x);
    newStationLocation.setY(y);

    // Check if new station location is too close to other stations
    while(!stationLocationIsGood(newStationLocation)) {
        x = random.bounded(drawer->size().width());
        y = random.bounded(drawer->size().height());

        newStationLocation.setX(x);
        newStationLocation.setY(y);
    }

    // Create station and add it to station list
    Station* newStation = new Station(this, newStationLocation);
    stations.append(newStation);
    drawer->drawStations(newStation);
}

bool MapModel::stationLocationIsGood(QPoint newStationLocation) {
    foreach(Station station, stations) {
        // Calculate the distance between the station and the possible new station
        double xCord = std::pow(newStationLocation.x() - station.getLocation().x(), 2);
        double yCord = std::pow(newStationLocation.y() - station.getLocation().y(), 2);

        double distance = std::sqrt(xCord + yCord);

        // Check if this station is too close
        if(distance < STATION_DISTANCE)
            return false;
    }

    return true;
}

void MapModel::checkProgressBar(int progressValue) {
    // Check if progress is full
    if (progressValue != 100)
        return;

    emit restartProgressBar();
    emit showNewTip();
}

// not implemented
void MapModel::checkForStations(QList<QPoint>) {

}
