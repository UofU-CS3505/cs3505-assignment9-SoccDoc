#ifndef TRAINDRAWER_H
#define TRAINDRAWER_H
#include <QWidget>
#include "station.h"

class TrainDrawer: public QWidget
{
    Q_OBJECT
public:
    TrainDrawer(QWidget *parent = nullptr);

    QSize size();

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
    QImage image;
    QImage baseImage;
    QPoint lastPoint;
    bool scribbling;
    bool redrawLine;
public slots:
    void drawStations(Station* station);
};

#endif // TRAINDRAWER_H
