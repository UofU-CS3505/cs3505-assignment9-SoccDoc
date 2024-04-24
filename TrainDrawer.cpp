#include "TrainDrawer.h"
#include "passenger.h"
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QtMath>

TrainDrawer::TrainDrawer(QWidget *parent) : QWidget(parent) {
    //create the world with correct gravity
    b2Vec2 gravity(0.0f, 10.0f);
    _world = new b2World(gravity);
    _world->SetAutoClearForces(true);

    //makes sure each overlay fram is correctly sized with the correct transperency.
    setAttribute(Qt::WA_StaticContents);
    resizeImage(&baseImage, QSize(750, 500), 255);
    resizeImage(&redLine, QSize(750, 500), 0);
    resizeImage(&blueLine, QSize(750, 500), 0);
    resizeImage(&greenLine, QSize(750, 500), 0);
    resizeImage(&stationDrawings, QSize(750, 500), 0);
    overlayImage = baseImage;
    redrawLine = false;
    points = new QList<QPoint>{};
}

void TrainDrawer::paintEvent(QPaintEvent *event)
{
    //if the object needs to be redrawn we redraw the overlay image into the painter, otherwise geet redrawing the base image
    QPainter painter(this);
    QRect dirtyRect = event->rect();

    if(!redrawLine){
        painter.drawImage(dirtyRect, baseImage, dirtyRect);
    }else{
        painter.drawImage(dirtyRect, overlayImage, dirtyRect);
    }
    
    //steps the world so the physics can move and redraw all the confetti from the list of confetti objects
    _world->Step(1.0f/10.0f, 8, 3);
    foreach(const struct confetti& o, allConfetti) {
        drawConfetti(&painter, o);
    }
}

void TrainDrawer::resizeImage(QImage *image, const QSize &newSize, int colorValue)
{
    if (image->size() == newSize)
        return;

    //create a new image of a different size, and set the old image into that image with th painter.
    QImage newImage(newSize, QImage::Format_ARGB32);
    newImage.fill(qRgba(colorValue, colorValue, colorValue, colorValue));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void TrainDrawer::mousePressEvent(QMouseEvent *event)
{
    //check if the mouse click hit a black pixel, if it did, i was over a station and set the hitBlack marker to true
    if (event->button() == Qt::LeftButton && redrawLine) {
        if(stationDrawings.pixelColor(event->position().toPoint()) == Qt::black && !hitBlack){
            //emit signal with the point
            hitBlack = true;
            points->append(event->position().toPoint());
        }else if (stationDrawings.pixelColor(event->position().toPoint()) != Qt::black){
            hitBlack = false;
        }
        lastPoint = event->position().toPoint();
        scribbling = true;

    }
    //update the selected station to the point where the mouse was clicked
    emit updateSelectStation(event->position().toPoint());
}

void TrainDrawer::mouseMoveEvent(QMouseEvent *event)
{
    //detects if the mouse is moving while buttons are pressed, if the left button is pressed and the redraw button has been pressed the user is drawing a new path
    if ((event->buttons() & Qt::LeftButton) && scribbling && redrawLine){
        if(stationDrawings.pixelColor(event->position().toPoint()) == Qt::black && !hitBlack){
            //emit signal with the point
            hitBlack = true;
            points->append(event->position().toPoint());

        }else if(stationDrawings.pixelColor(event->position().toPoint()) != Qt::black){
            hitBlack = false;
        }
        //draw the line traced by the mouse
        drawLineTo(event->position().toPoint());
    }
}

void TrainDrawer::mouseReleaseEvent(QMouseEvent *event)
{
    //detects when the mouse was released and rsets all the variables that mightve been changed to their default values
    if (event->button() == Qt::LeftButton && scribbling && redrawLine) {
        if(stationDrawings.pixelColor(event->position().toPoint()) == Qt::black && !hitBlack){
            //emit signal with the point
            hitBlack = true;
            points->append(event->position().toPoint());
        }else if(stationDrawings.pixelColor(event->position().toPoint()) != Qt::black){
            hitBlack = false;
        }

        hitBlack = false;
        drawLineTo(event->position().toPoint());
        scribbling = false;
        redrawLine = false;
        update();
        overlayImage = baseImage;
        emit checkForStations(*points);
        emit enableTrackButtonSignal();
        points->clear();
    }
}

void TrainDrawer::drawLineTo(const QPoint &endPoint)
{
    //draw a line on the overlay image with the selected pen color
    QPainter painter(&overlayImage);
    painter.setPen(QPen(penColor, 10, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    lastPoint = endPoint;
}

void TrainDrawer::redrawTrack(){
    //fill all thelvines of the same color as the selected lines with white, wich clears the line created
    if(penColor == Qt::green){
        greenLine.fill(qRgba(0, 0, 0, 0));
    }else if(penColor == Qt::blue){
        blueLine.fill(qRgba(0, 0, 0, 0));
    }else if(penColor == Qt::red){
        redLine.fill(qRgba(0, 0, 0, 0));
    }

    //redraw all the other lines that were not deleted
    baseImage.fill(qRgba(255, 255, 255, 255));
    QPainter painter(&baseImage);
    painter.drawImage(0, 0, redLine);
    painter.drawImage(0, 0, greenLine);
    painter.drawImage(0, 0, blueLine);
    painter.drawImage(0, 0, stationDrawings);
    overlayImage = baseImage;
    redrawLine = true;
}

void TrainDrawer::drawLineBetweenStations(const QPoint &startPoint, const QPoint &endPoint){
    //start drawimg the line just outside of the station, and finish just outside of the station
    QPoint newStartPoint(startPoint.x()+(STATION_WIDTH/2), startPoint.y()+(STATION_WIDTH/2));
    QPoint newEndPoint(endPoint.x()+(STATION_WIDTH/2), endPoint.y()+(STATION_WIDTH/2));

    //code to draw multiple lines to a station and make them all appear nicely
    //offsets each color by a certain amount of pixels to make sure they dont overlap
    if(distanceXIsGreater(startPoint, endPoint) && penColor != Qt::green){
        if(penColor == Qt::blue){
            newStartPoint.setY(newStartPoint.y() + 6);
            newEndPoint.setY(newEndPoint.y() + 6);
            QPainter painter(&blueLine);
            painter.setPen(QPen(penColor, 3, Qt::SolidLine, Qt::RoundCap,
                                Qt::RoundJoin));
            painter.drawLine(newStartPoint, newEndPoint);
        } else if(penColor == Qt::red){
            newStartPoint.setY(newStartPoint.y() - 6);
            newEndPoint.setY(newEndPoint.y() - 6);
            QPainter painter(&redLine);
            painter.setPen(QPen(penColor, 3, Qt::SolidLine, Qt::RoundCap,
                                Qt::RoundJoin));
            painter.drawLine(newStartPoint, newEndPoint);
        }
    }else if(!distanceXIsGreater(startPoint, endPoint) && penColor != Qt::green){
        if(penColor == Qt::blue){
            newStartPoint.setX(newStartPoint.x() + 6);
            newEndPoint.setX(newEndPoint.x() + 6);
            QPainter painter(&blueLine);
            painter.setPen(QPen(penColor, 3, Qt::SolidLine, Qt::RoundCap,
                                Qt::RoundJoin));
            painter.drawLine(newStartPoint, newEndPoint);
        } else if(penColor == Qt::red){
            newStartPoint.setX(newStartPoint.x() - 6);
            newEndPoint.setX(newEndPoint.x() - 6);
            QPainter painter(&redLine);
            painter.setPen(QPen(penColor, 3, Qt::SolidLine, Qt::RoundCap,
                                Qt::RoundJoin));
            painter.drawLine(newStartPoint, newEndPoint);
        }
        //green does not need to be offset, since we assume its the primary color
    }else{
        QPainter painter(&greenLine);
        painter.setPen(QPen(penColor, 3, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.drawLine(newStartPoint, newEndPoint);
    }
    QPainter painter(&baseImage);
    painter.drawImage(0, 0, redLine);
    painter.drawImage(0, 0, greenLine);
    painter.drawImage(0, 0, blueLine);
    painter.drawImage(0, 0, stationDrawings);
}

bool TrainDrawer::distanceXIsGreater(const QPoint &startPoint, const QPoint &endPoint){
    //determins if the X distance between two points is greater than or less than the distance between the Y points
    int distanceX = qFabs(startPoint.x() - endPoint.x());
    int distanceY = qFabs(startPoint.y() - endPoint.y());

    if(distanceX > distanceY){
        return true;
    }else{
        return false;
    }
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
    //create painter or the station overlay
    QPainter painter(&stationDrawings);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    //check what type of station needs to be drawn and draw that shape on the image.
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

    QPainter baseImagePainter(&baseImage);
    baseImagePainter.drawImage(0, 0, stationDrawings);
}

QSize TrainDrawer::size() {
    return overlayImage.size();
}

int TrainDrawer::getWidth(){
    return STATION_WIDTH;
}

void TrainDrawer::setPenColor(QColor newPenColor){
    penColor = newPenColor;
}

void TrainDrawer::selectStation(Station* selectedStation){
    //draw a square around the station and set it to the selected station so the data can be updated.
    QPainter painter(&baseImage);
    painter.save();
    if(previousSelectedStation != nullptr){
        QRectF rectangle(previousSelectedStation->getLocation().x()-5, previousSelectedStation->getLocation().y()-5, STATION_WIDTH+10, STATION_WIDTH+10);
        painter.setPen(Qt::white);
        painter.drawRect(rectangle);
    }

    QRectF rectangle(selectedStation->getLocation().x()-5, selectedStation->getLocation().y()-5, STATION_WIDTH+10, STATION_WIDTH+10);
    painter.setPen(Qt::cyan);
    painter.drawRect(rectangle);
    painter.restore();

    previousSelectedStation = selectedStation;
}
