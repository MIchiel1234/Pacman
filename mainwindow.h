#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "pacman.h"
#include "ghost.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateGame();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Pacman *pacman;
    QList<Ghost *> ghosts;
    QList<QGraphicsPixmapItem *> dots;
    QTimer *gameTimer;

    void setupScene();
    void checkCollisions();
    void handlePowerBall(QGraphicsPixmapItem *powerBall);
    void checkGhostCollisionWithPacman();
    void handleGhostEaten(Ghost *ghost);
    void removeDot(QGraphicsPixmapItem *dot);
    void resetGame();
};

#endif // MAINWINDOW_H
