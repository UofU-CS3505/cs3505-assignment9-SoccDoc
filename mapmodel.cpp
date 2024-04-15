#include "mapmodel.h"
#include <QWidget>

MapModel::MapModel(QWidget *parent) :
    QWidget(parent), updateTimer(this),
    drawer(new TrainDrawer(this))
{
    trains.append(new Train());

    // Get the securely seeded generator
    rand = QRandomGenerator::securelySeeded();

    // Setup and start update timer
    updateTimer.setInterval(MILISECONDS_TO_UPDATE);
    connect(&updateTimer, &QTimer::timeout, this, &MapModel::updateFrame);
    updateTimer.start();

    // Spawn some initial stations
    for (int i = 0; i < 10; i++)
        spawnStation();
}

void MapModel::updateFrame() {
   drawer->updateImage();
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
void MapModel::confetti() {
    drawer->confetti();
}

void MapModel::spawnStation() {
    // Get canvas dimensions
    int canvasWidth = drawer->size().width();
    int canvasHeight = drawer->size().height();

    // Create initial random location
    int x = rand.bounded(STATION_EDGE_BUFFER, canvasWidth - STATION_EDGE_BUFFER);
    int y = rand.bounded(STATION_EDGE_BUFFER, canvasHeight - STATION_EDGE_BUFFER);

    QPoint newStationLocation;
    newStationLocation.setX(x);
    newStationLocation.setY(y);
    qDebug() << x << ", " << y;

    // Check if new station location is too close to other stations
    while(!stationLocationIsGood(newStationLocation)) {
        x = rand.bounded(STATION_EDGE_BUFFER, canvasWidth - STATION_EDGE_BUFFER);
        y = rand.bounded(STATION_EDGE_BUFFER, canvasHeight - STATION_EDGE_BUFFER);

        qDebug() << x << ", " << y;

        newStationLocation.setX(x);
        newStationLocation.setY(y);
    }

    // Create station and add it to station list
    Station* newStation = new Station(this, newStationLocation, rand);
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
    if (progressValue == 20){
        emit showNewTip();
        return;
    }

    if (progressValue == 40){
        emit showNewTip();
        return;
    }

    if (progressValue == 60){
        emit showNewTip();
        return;
    }

    if (progressValue == 80){
        emit showNewTip();
        return;
    }

    if (progressValue == 100){
        emit showNewTip();
        confetti();
        return;
    }
}

// not implemented
void MapModel::checkForStations(QList<QPoint>) {

}
