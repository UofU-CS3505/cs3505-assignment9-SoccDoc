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
    // void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    void resizeImage(QImage *image, const QSize &newSize);
    void drawLineTo(const QPoint &endPoint);
    QImage image;
    QImage baseImage;
    QPoint lastPoint;
    bool scribbling;
    bool redrawLine;
public slots:
    void drawStations(Station* station);
};

#endif // TRAINDRAWER_H
