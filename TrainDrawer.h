#ifndef TRAINDRAWER_H
#define TRAINDRAWER_H
#include <QWidget>
#include "station.h"

class TrainDrawer: public QWidget
{
    Q_OBJECT
public:
    TrainDrawer(QWidget *parent = nullptr);
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
    bool redrawLine;
public slots:
    void drawStations(Station* station);
signals:
    void checkForStations(QList<QPoint>);
};

#endif // TRAINDRAWER_H
