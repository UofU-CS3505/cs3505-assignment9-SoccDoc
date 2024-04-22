#include "mapmodel.h"
#include <QWidget>

MapModel::MapModel(QWidget *parent) :
    QWidget(parent), updateTimer(this),
    drawer(new TrainDrawer(this))
{
    // Get the securely seeded generator
    rand = QRandomGenerator::securelySeeded();

    // Setup and start update timer
    updateTimer.setInterval(MILISECONDS_TO_UPDATE);
    connect(&updateTimer, &QTimer::timeout, this, &MapModel::updateFrame);
    connect(drawer, &TrainDrawer::checkForStations, this, &MapModel::drawStationsOnLine);
    updateTimer.start();

    // Spawn default stations
    spawnCircleStation();
    spawnSquareStation();
    spawnTriangleStation();
    spawnStation();

    // Set initial line color
    currentLine = Qt::green;

    // Add goals to queue
    passengerGoals.enqueue(5);
    passengerGoals.enqueue(15);
    passengerGoals.enqueue(30);
    passengerGoals.enqueue(50);
    passengerGoals.enqueue(100);

    // Set first goal
    numberOfPassengersDelivered = 0;
    currentPassengerGoal = passengerGoals.dequeue();
}

void MapModel::updateFrame() {
    // Update all of the stations
    foreach (Station* station, stations)
    {
        // If the station has added a passenger, update how many are drawn
        if (station->update()){
            int index = station->returnWaitingSize() - 1;
            emit drawStationPassenger(station, station->getPassengers().at(index));
        }
    }

    // Update the canvas
    drawer->updateImage();

    // Update the current data and highlight the selected station
    if(selectedStation != nullptr){
        emit updateData(selectedStation->getThroughput(), selectedStation->getWaitTime(), (selectedStation->getPassengers().size()));
        drawer->selectStation(selectedStation);
    }
}

void MapModel::trainButtonClicked(int id) {
    // Update details for selected radio button
    switch(id) {
    case 0:
        emit updateTrainDetails("Green Train Details");
        currentLine = Qt::green;
        break;
    case 1:
        emit updateTrainDetails("Blue Train Details");
        currentLine = Qt::blue;
        break;
    case 2:
        emit updateTrainDetails("Red Train Details");
        currentLine = Qt::red;
        break;
    }

    // Set the color of the drawer based on the radio button selected
    drawer->setPenColor(currentLine);
}

void MapModel::stationButtonClicked(int id) {
    // Update details for selected radio button
    switch(id) {
    case 0:
        emit updateStationDetails("Square Station Details");
        break;
    case 1:
        emit updateStationDetails("Circle Station Details");
        break;
    }
}

TrainDrawer* MapModel::getDrawer(){
    return drawer;
}
void MapModel::confetti() {
    drawer->confetti();
}

void MapModel::spawnStation() {
    // Get canvas dimensions
    int canvasWidth = drawer->size().width();
    int canvasHeight = drawer->size().height();

    // Create initial random location
    int x = rand.bounded(STATION_EDGE_BUFFER, canvasWidth - STATION_EDGE_BUFFER);
    int y = rand.bounded(STATION_EDGE_BUFFER, canvasHeight - STATION_EDGE_BUFFER);

    QPoint newStationLocation;
    newStationLocation.setX(x);
    newStationLocation.setY(y);

    // Check if new station location is too close to other stations
    while(!stationLocationIsGood(newStationLocation)) {
        x = rand.bounded(STATION_EDGE_BUFFER, canvasWidth - STATION_EDGE_BUFFER);
        y = rand.bounded(STATION_EDGE_BUFFER, canvasHeight - STATION_EDGE_BUFFER);

        newStationLocation.setX(x);
        newStationLocation.setY(y);
    }

    // Create station and add it to station list
    Station* newStation = new Station(this, newStationLocation, Passenger(rand.bounded(3)));
    stations.append(newStation);
    drawer->drawStations(newStation);
    selectedStation = newStation;

    connect(newStation, &Station::passengerDelivered, this, &MapModel::passengerDelivered);
}

void MapModel::spawnSquareStation() {
    // Get canvas dimensions
    int canvasWidth = drawer->size().width();
    int canvasHeight = drawer->size().height();

    // Create initial random location
    int x = rand.bounded(STATION_EDGE_BUFFER, canvasWidth - STATION_EDGE_BUFFER);
    int y = rand.bounded(STATION_EDGE_BUFFER, canvasHeight - STATION_EDGE_BUFFER);

    QPoint newStationLocation;
    newStationLocation.setX(x);
    newStationLocation.setY(y);

    // Check if new station location is too close to other stations
    while(!stationLocationIsGood(newStationLocation)) {
        x = rand.bounded(STATION_EDGE_BUFFER, canvasWidth - STATION_EDGE_BUFFER);
        y = rand.bounded(STATION_EDGE_BUFFER, canvasHeight - STATION_EDGE_BUFFER);

        newStationLocation.setX(x);
        newStationLocation.setY(y);
    }

    // Create station and add it to station list
    Station* newStation = new Station(this, newStationLocation, Passenger(0));
    stations.append(newStation);
    drawer->drawStations(newStation);
    selectedStation = newStation;

    connect(newStation, &Station::passengerDelivered, this, &MapModel::passengerDelivered);
}

void MapModel::spawnTriangleStation() {
    // Get canvas dimensions
    int canvasWidth = drawer->size().width();
    int canvasHeight = drawer->size().height();

    // Create initial random location
    int x = rand.bounded(STATION_EDGE_BUFFER, canvasWidth - STATION_EDGE_BUFFER);
    int y = rand.bounded(STATION_EDGE_BUFFER, canvasHeight - STATION_EDGE_BUFFER);

    QPoint newStationLocation;
    newStationLocation.setX(x);
    newStationLocation.setY(y);

    // Check if new station location is too close to other stations
    while(!stationLocationIsGood(newStationLocation)) {
        x = rand.bounded(STATION_EDGE_BUFFER, canvasWidth - STATION_EDGE_BUFFER);
        y = rand.bounded(STATION_EDGE_BUFFER, canvasHeight - STATION_EDGE_BUFFER);

        newStationLocation.setX(x);
        newStationLocation.setY(y);
    }

    // Create station and add it to station list
    Station* newStation = new Station(this, newStationLocation, Passenger(2));
    stations.append(newStation);
    drawer->drawStations(newStation);
    selectedStation = newStation;

    connect(newStation, &Station::passengerDelivered, this, &MapModel::passengerDelivered);
}

void MapModel::spawnCircleStation() {
    // Get canvas dimensions
    int canvasWidth = drawer->size().width();
    int canvasHeight = drawer->size().height();

    // Create initial random location
    int x = rand.bounded(STATION_EDGE_BUFFER, canvasWidth - STATION_EDGE_BUFFER);
    int y = rand.bounded(STATION_EDGE_BUFFER, canvasHeight - STATION_EDGE_BUFFER);

    QPoint newStationLocation;
    newStationLocation.setX(x);
    newStationLocation.setY(y);

    // Check if new station location is too close to other stations
    while(!stationLocationIsGood(newStationLocation)) {
        x = rand.bounded(STATION_EDGE_BUFFER, canvasWidth - STATION_EDGE_BUFFER);
        y = rand.bounded(STATION_EDGE_BUFFER, canvasHeight - STATION_EDGE_BUFFER);

        newStationLocation.setX(x);
        newStationLocation.setY(y);
    }

    // Create station and add it to station list
    Station* newStation = new Station(this, newStationLocation, Passenger(1));
    stations.append(newStation);
    drawer->drawStations(newStation);
    selectedStation = newStation;

    connect(newStation, &Station::passengerDelivered, this, &MapModel::passengerDelivered);
}

bool MapModel::stationLocationIsGood(QPoint newStationLocation) {
    int width = drawer->getWidth();
    foreach(Station* station, stations) {
        // Calculate the distance between the station and the possible new station
        double xCord = std::pow(newStationLocation.x() + width - station->getLocation().x() + width, 2);
        double yCord = std::pow(newStationLocation.y() + width - station->getLocation().y() + width, 2);

        double distance = std::sqrt(xCord + yCord);

        // Check if this station is too close
        if(distance < STATION_DISTANCE)
            return false;
    }

    return true;
}

void MapModel::checkProgressBar(int progressValue) {
    // Don't do anything if progress bar isn't full
    if (progressValue < 100)
        return;

    // Reset the number of delivered passengers and update the goal
    numberOfPassengersDelivered = 0;
    if (!passengerGoals.isEmpty())
        currentPassengerGoal = passengerGoals.dequeue();

    // Give user an extra train and spawn another station
    numberOfUnusedTrains += 1;
    spawnStation();

    // Show a new tip and reset the progress bar
    emit showNewTip();
    emit restartProgressBar();
    confetti();
}

void MapModel::drawStationsOnLine(QList<QPoint> testPoints) {
    // Make a list of all stations touching the drawn line
    QList<Station*> selectedStations{};
    foreach(QPoint point, testPoints){
        if(!selectedStations.contains(getStation(point))){
            selectedStations.append(getStation(point));
        }
    }

    // Draw lines between each of those stations
    for(int i = 0; i < selectedStations.length() - 1; i++){
        QPoint startPoint = selectedStations.at(i)->getLocation();
        QPoint endPoint = selectedStations.at(i + 1)->getLocation();

        drawer->drawLineBetweenStations(startPoint, endPoint);
        drawer->drawStations(selectedStations.at(i));
        drawer->drawStations(selectedStations.at(i+1));
    }

    // Add a train to follow that line
    addTrainToLine(selectedStations);
}

Station* MapModel::getStation(QPoint point) {
    // Check if the given point is on a station
    foreach (Station* station, stations) {
        // Check x axis
        if(station->getLocation().x() > point.x() || ((station->getLocation().x() + drawer->STATION_WIDTH) < point.x()))
            continue;

        // Check y axis
        if(station->getLocation().y() > point.y() || (station->getLocation().y() + drawer->STATION_WIDTH) < point.y())
            continue;

        // Point is on this station
        selectedStation = station;
        return station;
    }

    // Point is not on any station, don't change selection
    return selectedStation;
}

void MapModel::passengerDelivered(int passengersDelivered) {
    numberOfPassengersDelivered += passengersDelivered;
    int progressBarPercent = ((double)numberOfPassengersDelivered / currentPassengerGoal) * 100;

    // Cap the progress bar percent at 100
    if (progressBarPercent > 100)
        progressBarPercent = 100;

    emit updateProgressBar(progressBarPercent);
}


void MapModel::addTrainToLine(QList<Station*> trainLine){
    // Check if there are at least two stations
    if (trainLine.size() < 2)
        return;

    if(numberOfUnusedTrains <= 0){
        return; //no trains available to add to a line.
    }

    // Setup train animation object
    QPixmap image;

    // Load corresponding train image color
    if (currentLine == Qt::blue)
        image.load(":/images/images/blueTrain.png");
    else if (currentLine == Qt::red)
        image.load(":/images/images/redTrain.png");
    else if (currentLine == Qt::green)
        image.load(":/images/images/greenTrain.png");

    image = image.scaled(50, 25, Qt::KeepAspectRatio);

    QLabel* trainImage;
    trainImage = new QLabel(drawer);
    trainImage->setPixmap(image);
    trainImage->show();
    QPropertyAnimation* animation = new QPropertyAnimation(trainImage, "pos");

    // Put everything into a train
    Train* train = new Train(this, animation, currentLine);
    train->changeStations(trainLine);
    trains.append(train);
    numberOfUnusedTrains -= 1;
}


