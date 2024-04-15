#include "TrainDrawer.h"
#include "passenger.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>



TrainDrawer::TrainDrawer(QWidget *parent) : QWidget(parent) {
    //create the world with correct gravity
    b2Vec2 gravity(0.0f, 10.0f);
    _world = new b2World(gravity);

    setAttribute(Qt::WA_StaticContents);
    resizeImage(&image, QSize(750, 500));
    resizeImage(&baseImage, QSize(750, 500));
    redrawLine = false;

}

void TrainDrawer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();

    if(!redrawLine){
        painter.drawImage(dirtyRect, baseImage, dirtyRect);
    }
    else
    {
        QPixmap base = QPixmap::fromImage(baseImage);
        QPixmap overlay = QPixmap::fromImage(image);
        QPixmap result(base.width(), base.height());
        result.fill(Qt::transparent);
        //we don't know if result is the one being shown
        QPainter painter(&result);
        painter.drawPixmap(0, 0, base);
        painter.drawPixmap(0, 0, overlay);
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
    if (event->button() == Qt::LeftButton && redrawLine) {
        lastPoint = event->position().toPoint();
        scribbling = true;
    }
}

void TrainDrawer::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling && redrawLine)
        drawLineTo(event->position().toPoint());
}

void TrainDrawer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling && redrawLine) {
        drawLineTo(event->position().toPoint());
        scribbling = false;
    }
}

void TrainDrawer::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    int rad = (10 / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
               .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}


void TrainDrawer::confetti(){
    //clear all the confetti in the array (assuming they've fallen off the screen)
    allConfetti.clear();
    //add new confetti at the correct location on the screen, position will need adjusting.
    for (int i = 0; i < 500; ++i) {
        allConfetti.append(createConfetti(b2Vec2(rand.bounded(0, baseImage.width()), rand.bounded(-30, 30))));
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
    shape.SetAsBox(2, 5);


    // fixture defines its movement/interactions as related to physics
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.friction = 10.0f;
    fd.restitution = 0.6f;
    o.fixture = o.body->CreateFixture(&fd);
    return o;
}

void TrainDrawer::drawConfetti(QPainter *painter, const struct TrainDrawer::confetti& confetti) {
    //get the position and angle of the coffetti
    float32 x = confetti.body->GetPosition().x + confetti.direction*500;
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
    if(station->getStationType() == Passenger::Circle){
        painter.drawEllipse(station->getLocation().x(), station->getLocation().y(), 60, 60);
    }
    else if(station->getStationType() == Passenger::Square){
        painter.drawRect(station->getLocation().x(), station->getLocation().y(), 60, 60);
    }
}
