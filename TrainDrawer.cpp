#include "TrainDrawer.h"
#include "passenger.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

TrainDrawer::TrainDrawer(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    resizeImage(&baseImage, QSize(750, 500));
    overlayImage = baseImage;
    redrawLine = false;
}

void TrainDrawer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    if(!redrawLine){
        qDebug() << "hello";
        painter.drawImage(dirtyRect, baseImage, dirtyRect);
    }else{
        qDebug() << "hi";
        painter.drawImage(dirtyRect, overlayImage, dirtyRect);
    }
}

// void TrainDrawer::resizeEvent(QResizeEvent *event)
// {
//     if (width() > image.width() || height() > image.height()) {
//         resizeImage(&image, QSize(width(), height()));
//         update();
//     }
//     QWidget::resizeEvent(event);
// }

void TrainDrawer::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void TrainDrawer::mousePressEvent(QMouseEvent *event)
{
    redrawLine = true;
    if (event->button() == Qt::LeftButton && redrawLine) {
        points.append(event->position().toPoint());
        lastPoint = event->position().toPoint();
        scribbling = true;
    }
}

void TrainDrawer::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling && redrawLine){
        points.append(event->position().toPoint());
        drawLineTo(event->position().toPoint());
    }
}

void TrainDrawer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling && redrawLine) {
        points.append(event->position().toPoint());
        drawLineTo(event->position().toPoint());
        scribbling = false;
        redrawLine = false;
        update();
        overlayImage = baseImage;
        //emit checkForStations(points);
    }
}

void TrainDrawer::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&overlayImage);
    painter.setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    int rad = (10 / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
               .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void TrainDrawer::drawStations(Station* station){
    QPainter painter(&baseImage);
    if(station->getStationType() == Passenger::Circle){
        painter.drawEllipse(station->getLocation().x(), station->getLocation().y(), 60, 60);
    }
    else if(station->getStationType() == Passenger::Square){
        painter.drawRect(station->getLocation().x(), station->getLocation().y(), 60, 60);
    }
}
