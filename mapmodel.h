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
    Station* getStation(QPoint point);

private:
    QTimer updateTimer;
    QList<Train*> trains;
    QList<Station*> stations;
    Station* selectedStation = nullptr;
    TrainDrawer *drawer;
    QList<QPoint> stationPoints;
    QRandomGenerator rand;
    QPixmap trainImage;
    const int MILISECONDS_TO_UPDATE = (1000);
    const int STATION_DISTANCE = 120;
    const int STATION_EDGE_BUFFER = 40;

    QList<Station*> trainStations;


    /**
     * @brief spawns a new station and draws it
     */
    void spawnStation();

    /**
     * @brief Checks if a given point is too close to current stations
     * @param the point to check
     * @return false if station is too close, true otherwise
     */
    bool stationLocationIsGood(QPoint newStationLocation);

public slots:
    /**
     * @brief updates data
     */
    void updateFrame();
    void trainButtonClicked(int);
    void stationButtonClicked(int);
    void confetti();
    void checkForStations(QList<QPoint>);

    void checkProgressBar(int);

signals:
    void updateData(int, int);
    void updateTrainDetails(QString);
    void updateStationDetails(QString);
    void showNewTip();
    void restartProgressBar();
};

#endif // MAPMODEL_H
