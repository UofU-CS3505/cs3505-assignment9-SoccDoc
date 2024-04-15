#include "mapmodel.h"
#include <QWidget>

MapModel::MapModel(QWidget *parent) :
    QWidget(parent), updateTimer(this),
    drawer(new TrainDrawer(this))
{
    trains.append(new Train());
    stations.append(new Station());

    connect(this, &MapModel::drawNewStation, drawer, &TrainDrawer::drawStations);

    // Setup and start update timer
    updateTimer.setInterval(MILISECONDS_TO_UPDATE);
    connect(&updateTimer, &QTimer::timeout, this, &MapModel::updateFrame);
    updateTimer.start();
}

void MapModel::updateFrame() {
    update();
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
    update();
}
