#include "mapmodel.h"
#include <QWidget>

MapModel::MapModel(QWidget *parent) :
    QWidget(parent), updateTimer(this),
    drawer(new TrainDrawer(this))
{
    // Setup and start update timer
    updateTimer.setInterval(MILISECONDS_TO_UPDATE);
    //updateTimer.callOnTimeout(&MapModel::updateFrame);
    connect(&updateTimer, &QTimer::timeout, this, &MapModel::updateFrame);
    updateTimer.start();
}

void MapModel::updateFrame() {

}

TrainDrawer* MapModel::getDrawer(){
    return drawer;
}
