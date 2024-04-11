#include "mapmodel.h"
#include <QWidget>

MapModel::MapModel(QWidget *parent) :
    QWidget(parent), updateTimer(this),
    drawer(new TrainDrawer(this))
{
    // Setup and start update timer
    updateTimer.setInterval(MILISECONDS_TO_UPDATE);
    //updateTimer.callOnTimeout(&MapModel::updateFrame);                   // this doesn't work for some reason??
    connect(&updateTimer, &QTimer::timeout, this, &MapModel::updateFrame); // but this does
    updateTimer.start();
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
