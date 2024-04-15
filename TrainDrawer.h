#ifndef TRAINDRAWER_H
#define TRAINDRAWER_H
#include <QWidget>
#include <QRandomGenerator>
#include <Box2D/Box2D.h>

class TrainDrawer: public QWidget
{
    Q_OBJECT
public:
    TrainDrawer(QWidget *parent = nullptr);
    void confetti();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    void resizeImage(QImage *image, const QSize &newSize);
    void drawLineTo(const QPoint &endPoint);
    QImage image;
    QPoint lastPoint;
    bool scribbling;



    QRandomGenerator rand;

    struct confetti
    {
        b2Body *body;
        b2Fixture *fixture;
    };

    struct confetti createConfetti(const b2Vec2& pos);
    void drawConfetti(QPainter *p, const struct TrainDrawer::confetti& confetti);
    QVector<struct confetti> allConfetti;
    b2World* _world;


};


#endif // TRAINDRAWER_H
