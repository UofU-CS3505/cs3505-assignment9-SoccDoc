#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include "mapmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected:
    /**
     * @brief closeEvent is a method that gets invoked when user closes the mainwindow
     * @param event - captures when the user clicks close
     */
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    MapModel *map;
    void createActions();

    void createLeftDockWindow();
    void createRightDockWindow();
    void createBottomDockWindow();
    void createDockWindows();

    QProgressBar* progressBar;
    /**
     * @brief action to tell the user about the application
     */
    QAction *aboutAct;
    /**
     * @brief action to exit the application
     */
    QAction *exitAct;




public slots:
    /**
     * @brief gives a short description of this application
     */
    void about();
};
#endif // MAINWINDOW_H
