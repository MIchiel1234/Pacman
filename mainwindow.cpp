#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>

const int TILE_SIZE = 20;
const int BORDER_GAP = 40; // Increase the gap size as needed

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    int sceneWidth = 560 + 2 * BORDER_GAP;
    int sceneHeight = 480 + 2 * BORDER_GAP;
    scene->setSceneRect(0, 0, sceneWidth, sceneHeight);
    scene->setBackgroundBrush(Qt::black);

    setupScene();

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::updateGame);
    gameTimer->start(100);

    // Set focus on the view to capture key events
    ui->graphicsView->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupScene() {
    scene->clear();
    dots.clear();
    ghosts.clear();

    QFile file(":/images/map.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open map file";
        return;
    }

    QTextStream in(&file);
    int y = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << "Read line:" << line;
        for (int x = 0; x < line.length(); ++x) {
            QGraphicsPixmapItem *item = nullptr;
            int posX = x * TILE_SIZE + BORDER_GAP;
            int posY = y * TILE_SIZE + BORDER_GAP;
            switch (line[x].toLatin1()) {
            case '0':
                item = new QGraphicsPixmapItem(QPixmap(":/images/dot.png"));
                item->setPos(posX, posY);
                item->setData(0, "dot");
                dots.append(item);
                qDebug() << "Added dot at (" << posX << "," << posY << ")";
                break;
            case '1':
                item = new QGraphicsPixmapItem(QPixmap(":/images/wall.png"));
                item->setData(0, "wall");
                item->setPos(posX, posY);
                qDebug() << "Added wall at (" << posX << "," << posY << ")";
                break;
            case '2':
                item = new QGraphicsPixmapItem(QPixmap(":/images/gate.png"));
                item->setData(0, "gate");
                item->setPos(posX, posY);
                qDebug() << "Added gate at (" << posX << "," << posY << ")";
                break;
            case '4':
                item = new QGraphicsPixmapItem(QPixmap(":/images/power_ball.png"));
                item->setData(0, "power_ball");
                item->setPos(posX, posY);
                dots.append(item);
                qDebug() << "Added power ball at (" << posX << "," << posY << ")";
                break;
            case 'p':
                pacman = new Pacman(":/images/pacman/a1.png");
                pacman->setPos(posX, posY);
                pacman->setFlag(QGraphicsItem::ItemIsFocusable);
                pacman->setFocus();
                scene->addItem(pacman);
                qDebug() << "Added Pacman at (" << posX << "," << posY << ")";
                break;
            case 'g':
                if (ghosts.size() < 4) {
                    QString ghostImagePath;
                    switch (ghosts.size()) {
                    case 0: ghostImagePath = ":/images/ghosts/redup1.png"; break;
                    case 1: ghostImagePath = ":/images/ghosts/yellowdown1.png"; break;
                    case 2: ghostImagePath = ":/images/ghosts/yellowleft1.png"; break;
                    case 3: ghostImagePath = ":/images/ghosts/yellowup1.png"; break;
                    }
                    Ghost *ghost = new Ghost(ghostImagePath);
                    ghost->setPos(posX, posY);
                    ghost->setFlag(QGraphicsItem::ItemIsFocusable);
                    scene->addItem(ghost);
                    ghosts.append(ghost);
                    QTimer::singleShot(ghosts.size() * 1000, this, [=]() {
                        ghost->setExitMode(true); // Delayed exit mode to stagger ghost exits
                    });
                    qDebug() << "Added ghost at (" << posX << "," << posY << ")";
                }
                break;
            default:
                qDebug() << "Unknown map character:" << line[x];
                break;
            }
            if (item) {
                scene->addItem(item);
            }
        }
        ++y;
    }
    file.close();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    qDebug() << "MainWindow received key press: " << event->key();
    if (event->key() == Qt::Key_Space) {
        int delay = 0;
        for (int i = 0; i < ghosts.size(); ++i) {
            if (!ghosts[i]->hasExited()) {
                QTimer::singleShot(delay, this, [=]() {
                    ghosts[i]->setExitMode(true);
                });
                delay += 1000;
            }
        }
    } else {
        pacman->keyPressEvent(event); // Forward other key events to Pacman
    }
}

void MainWindow::updateGame()
{
    pacman->move();
    for (auto ghost : ghosts) {
        ghost->move();
    }
    checkCollisions();
    checkGhostCollisionWithPacman();
}

void MainWindow::checkCollisions() {
    QRectF pacmanRect = pacman->boundingRect().translated(pacman->pos());

    // Check collision with dots
    for (QGraphicsPixmapItem *dot : dots) {
        if (pacmanRect.intersects(dot->boundingRect().translated(dot->pos()))) {
            if (dot->data(0) == "power_ball") {
                handlePowerBall(dot);
            } else {
                removeDot(dot);
            }
            break;
        }
    }

    // Check collision with ghosts
    checkGhostCollisionWithPacman();
}

void MainWindow::handlePowerBall(QGraphicsPixmapItem* powerBall) {
    scene->removeItem(powerBall);
    dots.removeOne(powerBall);
    delete powerBall;

    // Make all ghosts frightened
    for (auto ghost : ghosts) {
        ghost->becomeFrightened();
    }

    // Update score or any other game logic
}

void MainWindow::checkGhostCollisionWithPacman() {
    QList<QGraphicsItem *> collidingItems = pacman->collidingItems();
    for (QGraphicsItem *item : collidingItems) {
        Ghost *ghost = dynamic_cast<Ghost *>(item);
        if (ghost && ghost->hasExited()) {
            if (ghost->isFrightened()) {
                handleGhostEaten(ghost);
            } else {
                qDebug() << "Collision with ghost detected. Game resetting...";
                resetGame(); // Call the reset method if Pacman collides with a ghost
                break; // Ensure we don't call resetGame multiple times
            }
        }
    }
}

void MainWindow::handleGhostEaten(Ghost* ghost) {
    ghost->stopBeingFrightened();
    ghost->setPos(280 + BORDER_GAP, 220 + BORDER_GAP); // respawn position
    ghost->setExitMode(false);
    ghost->setExited(false);
    QTimer::singleShot(1000, this, [=]() {
        ghost->setExitMode(true); // Delayed exit mode to stagger ghost exits
    });

    // Update score or any other game logic
}

void MainWindow::removeDot(QGraphicsPixmapItem* dot) {
    scene->removeItem(dot);
    dots.removeOne(dot);
    delete dot;

    // Update score or any other game logic
}

void MainWindow::resetGame() {
    // Stop the game timer to prevent further updates during reset
    gameTimer->stop();

    // Disconnect the updateGame slot to ensure no pending operations
    disconnect(gameTimer, &QTimer::timeout, this, &MainWindow::updateGame);

    // Reset the scene, which includes Pacman, ghosts, and dots
    setupScene();

    // Reconnect the updateGame slot and restart the game timer after resetting
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::updateGame);
    gameTimer->start();
}
