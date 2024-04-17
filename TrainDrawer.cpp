#include "TrainDrawer.h"
#include "passenger.h"
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QDebug>



TrainDrawer::TrainDrawer(QWidget *parent) : QWidget(parent) {
    //create the world with correct gravity
    b2Vec2 gravity(0.0f, 10.0f);
    _world = new b2World(gravity);
    _world->SetAutoClearForces(true);

    setAttribute(Qt::WA_StaticContents);
    resizeImage(&baseImage, QSize(750, 500));
    overlayImage = baseImage;
    redrawLine = false;
    points = new QList<QPoint>{};

}

void TrainDrawer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();

    if(!redrawLine){
        painter.drawImage(dirtyRect, baseImage, dirtyRect);
    }else{
        painter.drawImage(dirtyRect, overlayImage, dirtyRect);
    }

    
    _world->Step(1.0f/10.0f, 8, 3);
    foreach(const struct confetti& o, allConfetti) {
            drawConfetti(&painter, o);
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
    overlayImage = baseImage;
    redrawLine = true;
    if (event->button() == Qt::LeftButton && redrawLine) {
        if(baseImage.pixelColor(event->position().toPoint()) == Qt::black && !hitBlack){
            //emit signal with the point
            hitBlack = true;
            qDebug() << "Pixel sent";
            points->append(event->position().toPoint());
        }else if (baseImage.pixelColor(event->position().toPoint()) != Qt::black){
            hitBlack = false;
        }
        lastPoint = event->position().toPoint();
        scribbling = true;
    }
}

void TrainDrawer::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling && redrawLine){
        if(baseImage.pixelColor(event->position().toPoint()) == Qt::black && !hitBlack){
            //emit signal with the point
            hitBlack = true;
            qDebug() << "Pixel sent";
            qDebug() << "different signal";
            points->append(event->position().toPoint());

        }else if(baseImage.pixelColor(event->position().toPoint()) != Qt::black){
            hitBlack = false;
            qDebug() << "hitBlack was set false";
        }
        drawLineTo(event->position().toPoint());
    }
}

void TrainDrawer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling && redrawLine) {
        if(baseImage.pixelColor(event->position().toPoint()) == Qt::black && !hitBlack){
            //emit signal with the point
            hitBlack = true;
            points->append(event->position().toPoint());
        }else if(baseImage.pixelColor(event->position().toPoint()) != Qt::black){
            hitBlack = false;
        }

        hitBlack = false;
        drawLineTo(event->position().toPoint());
        scribbling = false;
        redrawLine = false;
        update();
        overlayImage = baseImage;
        emit checkForStations(*points);
        points->clear();
    }
}

void TrainDrawer::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&overlayImage);
    painter.setPen(QPen(Qt::blue, 5, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    int rad = (10 / 2) + 2;
    // update(QRect(lastPoint, endPoint).normalized()
    //            .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void TrainDrawer::drawLineBetweenStations(const QPoint &startPoint, const QPoint &endPoint){
    QPainter painter(&baseImage);
    painter.setPen(QPen(Qt::yellow, 3, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(startPoint, endPoint);
    qDebug() << "The line is drawn";
}


void TrainDrawer::confetti(){
    //clear all the confetti in the array (assuming they've fallen off the screen)
    for(int i = 0; i < allConfetti.size(); i++)
    {
        _world->DestroyBody(allConfetti.at(i).body);
        allConfetti.removeAt(i);
    }

    //add new confetti at the correct location on the screen, position will need adjusting. rand.bounded(0, baseImage.width())
    for (int i = 0; i < 300; ++i) {
        allConfetti.append(createConfetti(b2Vec2(baseImage.width()/2, rand.bounded(-50, 50))));
    }

}
struct TrainDrawer::confetti TrainDrawer::createConfetti(const b2Vec2& pos) {
    struct confetti o;
    // body of the confetti, make dynamic for the effect, will cause them to fall and interact with each other
    b2BodyDef bd;
    o.direction = (rand.bounded(100)%2)*rand.bounded(-1, 1);
    bd.type = b2_dynamicBody;
    bd.position = pos;
    o.body = _world->CreateBody(&bd);

    // create the shape of the object
    b2PolygonShape shape;
    shape.SetAsBox(rand.bounded(2,3), rand.bounded(4,6));


    // fixture defines its movement/interactions as related to physics
    b2FixtureDef fd;

    fd.shape = &shape;
    fd.density = 0.01f;
    fd.friction = 0.0f;
    fd.restitution = 2.0f; //how bouncy the body is
    o.body->ApplyTorque((900.0f *rand.bounded(-2, 2)), false); // apply forces to the body here to adjust confetti
    o.fixture = o.body->CreateFixture(&fd);
    return o;
}

void TrainDrawer::drawConfetti(QPainter *painter, const struct TrainDrawer::confetti& confetti) {
    //get the position and angle of the coffetti
    float32 x = confetti.body->GetPosition().x;
    float32 y = confetti.body->GetPosition().y;
    float32 angle = confetti.body->GetAngle();

    //create the b2shape as a QObject
    const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(confetti.fixture->GetShape());
    float32 hx = shape->GetVertex(1).x;
    float32 hy = shape->GetVertex(2).y;
    QRectF rect(x-hx, y-hy, 2*hx, 2*hy);


    //save the painter, move the rectangle onto the image, and set its correct orientation
    painter->save();
    painter->translate(rect.center());
    painter->rotate(angle*180/b2_pi);
    painter->translate(-rect.center());
    painter->fillRect(rect, QColor(rand.bounded(254), rand.bounded(254), rand.bounded(254), 255) );
    painter->restore();
}

void TrainDrawer::updateImage(){
    update(0, 0, baseImage.width(), baseImage.height());
}

void TrainDrawer::drawStations(Station* station){
    QPainter painter(&baseImage);
    painter.setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    if(station->getStationType() == Passenger::Circle){
        painter.setBrush(Qt::black);
        QRectF rectangle(station->getLocation().x(), station->getLocation().y(), STATION_WIDTH, STATION_WIDTH);
        painter.drawEllipse(rectangle);

    }
    else if(station->getStationType() == Passenger::Square){
        QRectF rectangle(station->getLocation().x(), station->getLocation().y(), STATION_WIDTH, STATION_WIDTH);
        painter.fillRect(rectangle, QBrush(Qt::black));

    }else if(station->getStationType() == Passenger::Triangle){
        // Start point of bottom line
        qreal startPointX1 = station->getLocation().x();
        qreal startPointY1 = station->getLocation().y();

        // End point of bottom line
        qreal endPointX1   = station->getLocation().x()+ STATION_WIDTH;
        qreal endPointY1   = station->getLocation().y();


        // End point of top line
        qreal endPointX2   = station->getLocation().x() + STATION_WIDTH/2;
        qreal endPointY2   = station->getLocation().y() + STATION_WIDTH;

        QPainterPath path;
        // Set pen to this point.
        path.moveTo(startPointX1, startPointY1);
        // Draw line from pen point to this point.
        path.lineTo(endPointX1, endPointY1);

        //path.moveTo (endPointX1, endPointY1); // <- no need to move
        path.lineTo(endPointX2,   endPointY2);

        //path.moveTo (endPointX2,   endPointY2); // <- no need to move
        path.lineTo(startPointX1, startPointY1);

        // painter.drawPath(path);
        painter.fillPath(path, QBrush (Qt::black));
    }
}

QSize TrainDrawer::size() {
    return overlayImage.size();
}

int TrainDrawer::getWidth(){
    return STATION_WIDTH;
}


