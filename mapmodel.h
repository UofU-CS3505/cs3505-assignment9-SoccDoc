#ifndef MAPMODEL_H
#define MAPMODEL_H
#include <QWidget>
#include <QTimer>
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
    const int MILISECONDS_TO_UPDATE = 1000;

public slots:
    /**
     * @brief updates data
     */
    void updateFrame();

    void trainButtonClicked(int);
    void stationButtonClicked(int);
    void confetti();

signals:
    void updateData(int, int);
    void updateTrainDetails(QString);
    void updateStationDetails(QString);

};

#endif // MAPMODEL_H
