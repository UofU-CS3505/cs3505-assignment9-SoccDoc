#ifndef TRAINDRAWER_H
#define TRAINDRAWER_H
#include <QWidget>
#include <QRandomGenerator>
#include <Box2D/Box2D.h>
#include <QColor>
#include "station.h"

class TrainDrawer: public QWidget
{
    Q_OBJECT
public:
    ///
    /// \brief TrainDrawer - constructor
    /// \param parent - the qwidget that owns traindrawer
    ///
    TrainDrawer(QWidget *parent = nullptr);

    ///
    /// \brief getWidth - returns the width of a station
    /// \return - the width of a station
    ///
    int getWidth();

    ///
    /// \brief confetti - creates confetti using box2D
    ///
    void confetti();

    ///
    /// \brief updateImage - updates the entire canvas
    ///
    void updateImage();

    ///
    /// \brief size - returns the size of the canvas
    /// \return - size of the canvas
    ///
    QSize size();

    ///
    /// \brief STATION_WIDTH - a constant variable that holds the width of stations
    ///
    const int STATION_WIDTH = 30;

    ///
    /// \brief setPenColor - sets the pen color
    /// \param newPenColor - the color the pen will be set to
    ///
    void setPenColor(QColor newPenColor);

    ///
    /// \brief selectStation - highlights the station that is currently selected
    /// \param selectedStation - the station that is currently selected
    ///
    void selectStation(Station* selectedStation);

    ///
    /// \brief drawLineTo - This method draws a line between the startPoint and endPoint given. This is used to draw a straight line between stations.
    /// \param startPoint - The start point to begin drawing the line.
    /// \param endPoint  - The end point to end drawing the line.
    ///
    void drawLineBetweenStations(const QPoint &startPoint, const QPoint &endPoint);

    ///
    /// \brief redrawTrack - erases all the previous track lines of the selected track line and allows the user to draw track lines
    ///
    void redrawTrack();

protected:
    ///
    /// \brief paintEvent - event that gets called every update
    /// \param event - a paint event
    ///
    void paintEvent(QPaintEvent *event) override;

    ///
    /// \brief mousePressEvent - event that gets called when the user clicks their mouse
    /// \param event - a qmouse event
    ///
    void mousePressEvent(QMouseEvent *event) override;

    ///
    /// \brief mouseMoveEvent - event that gets called when the user is moving their mouse
    /// \param event - a qmouse event
    ///
    void mouseMoveEvent(QMouseEvent *event) override;

    ///
    /// \brief mouseReleaseEvent - event that gets called when the user releases the mouse
    /// \param event - a qmouse event
    ///
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    ///
    /// \brief resizeImage - resizes the canvas with the given QSize and fills the canvas with the given color value
    /// \param image - the canvas
    /// \param newSize - the size of the canvas
    /// \param colorValue - the color of the canvas
    ///
    void resizeImage(QImage *image, const QSize &newSize, int colorValue);

    ///
    /// \brief drawLineTo - This method draws a line between the last point recieved and the current point. It is used for drawing a line.
    /// \param endPoint - The current point to be drawn to.
    ///
    void drawLineTo(const QPoint &endPoint);

    ///
    /// \brief distanceXIsGreater - checks if the distance of the x value between two points is greater than the distance of the y value between two points
    /// \param startPoint - the first point
    /// \param endPoint - the second point
    /// \return - true if the distance between the x values is greater than the y values
    ///
    bool distanceXIsGreater(const QPoint &startPoint, const QPoint &endPoint);

    ///
    /// \brief overlayImage - the image to be displayed when the user is drawing
    ///
    QImage overlayImage;

    ///
    /// \brief baseImage - the image that holds all the stations and lines, and is the image to be displayed when the user is not drawing
    ///
    QImage baseImage;

    ///
    /// \brief redLine - the image that holds only the red track lines
    ///
    QImage redLine;

    ///
    /// \brief blueLine - the image that holds only the blue track lines
    ///
    QImage blueLine;

    ///
    /// \brief greenLine - the image that holds only the green track lines
    ///
    QImage greenLine;

    ///
    /// \brief stationDrawings - the image that holds only the stations
    ///
    QImage stationDrawings;

    ///
    /// \brief lastPoint - holds the last point of the user's mouse movement
    ///
    QPoint lastPoint;

    ///
    /// \brief points - a list of points where the user's mouse hits a station
    ///
    QList<QPoint> *points;

    ///
    /// \brief scribbling - a boolean value that checks if the user is drawing or not
    ///
    bool scribbling;

    ///
    /// \brief penColor - the color of the pen
    ///
    QColor penColor = Qt::green;

    ///
    /// \brief hitBlack - boolean value that determines if the user's mouse hits a station
    ///
    bool hitBlack = false;

    ///
    /// \brief previousSelectedStation - holds the selected station
    ///
    Station* previousSelectedStation = nullptr;

    ///
    /// \brief rand - a random number generator
    ///
    QRandomGenerator rand;

    struct confetti
    {
        int direction;
        b2Body *body;
        b2Fixture *fixture;
    };

    struct confetti createConfetti(const b2Vec2& pos);
    void drawConfetti(QPainter *p, const struct TrainDrawer::confetti& confetti);
    QVector<struct confetti> allConfetti;
    b2World* _world;

    ///
    /// \brief redrawLine - a boolean value that determines if the user wants to redraw a line
    ///
    bool redrawLine;
public slots:
    ///
    /// \brief drawStations - draw stations
    /// \param station - the station to be drawn
    ///
    void drawStations(Station* station);

signals:
    ///
    /// \brief checkForStations - sends a signal that checks which station was hit during the user's mouse movement
    ///
    void checkForStations(QList<QPoint>);

    ///
    /// \brief enableTrackButtonSignal - sends a signal to enable the track button signals
    ///
    void enableTrackButtonSignal();

    ///
    /// \brief updateSelectStation - signal that sends the point of the selected station
    /// \param point - the point of the selected station
    ///
    void updateSelectStation(QPoint point);
};


#endif // TRAINDRAWER_H
