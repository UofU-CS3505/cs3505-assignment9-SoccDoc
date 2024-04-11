#ifndef MAPMODEL_H
#define MAPMODEL_H
#include <QWidget>
#include <QTimer>
#include "station.h"
#include "train.h"

/**
 * @brief The MapModel class
 * @authors Benjamin Sidwell, Alex Fraser, Jason Lopex, Andy Liu, and Ryan Nip
 */
class MapModel: public QWidget
{
    Q_OBJECT

public:
    MapModel(QWidget *parent = nullptr);

private:
    QTimer updateTimer;
    QList<Train> trains;
    QList<Station> stations;

};

#endif // MAPMODEL_H