#ifndef TRAINDRAWER_H
#define TRAINDRAWER_H
#include <QWidget>

class TrainDrawer: public QWidget
{
    Q_OBJECT
public:
    TrainDrawer(QWidget *parent = nullptr);
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
};

#endif // TRAINDRAWER_H
