#ifndef MAPMODEL_H
#define MAPMODEL_H
#include <QWidget>
#include <QTimer>
#include <QRandomGenerator>
#include "station.h"
#include "train.h"
#include "TrainDrawer.h"

/**
 * @brief The MapModel class
 * @authors Benjamin Sidwell, Alex Fraser, Jason Lopex, Andy Liu, and Ryan Nip
 */
class MapModel: public QWidget
{
    Q_OBJECT

public:
    MapModel(QWidget *parent = nullptr);

    TrainDrawer* getDrawer();

private:
    QTimer updateTimer;
    QList<Train*> trains;
    QList<Station*> stations;
    TrainDrawer *drawer;
    QList<QPoint> stationPoints;
    QRandomGenerator rand;
    const int MILISECONDS_TO_UPDATE = 1000;

public slots:
    /**
     * @brief updates data
     */
    void updateFrame();
    void trainButtonClicked(int);
    void stationButtonClicked(int);
    void checkForStations(QList<QPoint>);

signals:
    void updateData(int, int);
    void updateTrainDetails(QString);
    void updateStationDetails(QString);
    void drawNewStation(Station*);
};

#endif // MAPMODEL_H
