#ifndef CONTROL_H
#define CONTROL_H
#include <QWidget>

class Control: public QWidget
{
    Q_OBJECT
public:
    Control(QWidget *parent = nullptr);
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

#endif // CONTROL_H
