#include "mapmodel.h"
#include <QWidget>

MapModel::MapModel(QWidget *parent) : QWidget(parent), drawer(new TrainDrawer(this)) {

}

TrainDrawer* MapModel::getDrawer(){
    return drawer;
}
