#include "TrainDrawer.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>



TrainDrawer::TrainDrawer(QWidget *parent) : QWidget(parent) {
    //create the world with correct gravity
    b2Vec2 gravity(0.0f, -10.0f);
    _world = new b2World(gravity);
}

void TrainDrawer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);

    foreach(const struct confetti& o, allConfetti) {
            drawConfetti(&painter, o);
        }




}

void TrainDrawer::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        resizeImage(&image, QSize(width(), height()));
        update();
    }
    QWidget::resizeEvent(event);
}

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
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->position().toPoint();
        scribbling = true;
    }
}

void TrainDrawer::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->position().toPoint());
}

void TrainDrawer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
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
    for (int i = 0; i < 100; ++i) {
        allConfetti.append(createConfetti(b2Vec2(i, 0)));
    }

}
struct TrainDrawer::confetti TrainDrawer::createConfetti(const b2Vec2& pos) {
    struct confetti o;
    // body of the confetti, make dynamic for the effect, will cause them to fall and interact with each other
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = pos;
    o.body = _world->CreateBody(&bd);

    // create the shape of the object
    b2PolygonShape shape;
    shape.SetAsBox(20, 40);

    // fixture defines its movement/interactions as related to physics
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.friction = 1.0f;
    fd.restitution = 0.6f;
    o.fixture = o.body->CreateFixture(&fd);
    return o;
}

void TrainDrawer::drawConfetti(QPainter *p, const struct TrainDrawer::confetti& confetti) {
    //get the position and angle of the coffetti
    float32 x = confetti.body->GetPosition().x;
    float32 y = confetti.body->GetPosition().y;
    float32 angle = confetti.body->GetAngle();

    //create the b2shape as a QObject
    const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(confetti.fixture->GetShape());
    float32 hx = shape->GetVertex(1).x;
    float32 hy = shape->GetVertex(2).y;
    QRectF r(x-hx, y-hy, 2*hx, 2*hy);

    //save the painter, move the rectangle onto the image, and set its correct orientation
    p->save();
    p->translate(r.center());
    p->rotate(angle*180/b2_pi);
    p->translate(-r.center());
    p->drawRect(r);
    p->restore();
}
