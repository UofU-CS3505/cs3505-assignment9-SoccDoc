#ifndef TRAINDRAWER_H
#define TRAINDRAWER_H
#include <QWidget>
#include <QRandomGenerator>
#include <Box2D/Box2D.h>
#include "station.h"

class TrainDrawer: public QWidget
{
    Q_OBJECT
public:
    TrainDrawer(QWidget *parent = nullptr);
    int getWidth();
    void confetti();
    void updateImage();
    QSize size();
    const int STATION_WIDTH = 30;

protected:
    void paintEvent(QPaintEvent *event) override;
    //commented out because we don't want the canvas to be resized
    // void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    void resizeImage(QImage *image, const QSize &newSize);
    void drawLineTo(const QPoint &endPoint);
    QImage overlayImage;
    QImage baseImage;
    QPoint lastPoint;
    QList<QPoint> points;
    bool scribbling;





    QRandomGenerator rand;

    struct confetti
    {
        int direction;
        b2Body *body;
        b2Fixture *fixture;
    };

    struct confetti createConfetti(const b2Vec2& pos);
    void drawConfetti(QPainter *p, const struct TrainDrawer::confetti& confetti);
    QVector<struct confetti> allConfetti;
    b2World* _world;


    bool redrawLine;
public slots:
    void drawStations(Station* station);
signals:
    void checkForStations(QList<QPoint>);
};


#endif // TRAINDRAWER_H
