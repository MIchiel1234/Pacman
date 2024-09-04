#include "ghost.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>

Ghost::Ghost(const QString &imagePath) :
    QGraphicsPixmapItem(QPixmap(imagePath)),
    exitMode(false),
    exited(false),
    frightened(false),
    direction(QRandomGenerator::global()->bounded(4)), // Random initial direction
    originalImagePath(imagePath)
{
    frightenedTimer = new QTimer(this);
    connect(frightenedTimer, &QTimer::timeout, this, &Ghost::stopBeingFrightened);
}

void Ghost::setExitMode(bool mode)
{
    if (!exited) {
        exitMode = mode;
        if (exitMode) {
            // Align directly with the gate
            setX(280);
        }
    }
}

bool Ghost::hasExited() const
{
    return exited;
}

void Ghost::setExited(bool exitedStatus)
{
    exited = exitedStatus;
}

void Ghost::move()
{
    if (exitMode) {
        // Move towards the gate
        setY(y() - 10); // Move upwards through the gate
        if (y() <= 180) { // Assuming the gate ends at y = 180
            exitMode = false;
            exited = true; // Mark the ghost as exited
            qDebug() << "Ghost exited at position:" << x() << y(); // Debug statement
        }
        return; // Skip normal movement if in exit mode
    }

    // Normal random movement logic
    qreal dx = 0, dy = 0;
    switch (direction) {
    case 0: dx = -10; break; // Move left
    case 1: dx = 10; break;  // Move right
    case 2: dy = -10; break; // Move up
    case 3: dy = 10; break;  // Move down
    }

    // Calculate new position
    qreal newX = x() + dx;
    qreal newY = y() + dy;

    // Check for collision with walls
    QRectF newRect(newX, newY, boundingRect().width(), boundingRect().height());
    QList<QGraphicsItem *> collidingItems = scene()->items(newRect);
    bool collision = false;

    for (QGraphicsItem *item : collidingItems) {
        if (item != this && item->data(0) == "wall") {
            collision = true;
            break;
        }
    }

    // Handle collision
    if (collision) {
        direction = QRandomGenerator::global()->bounded(4);  // Change direction if collision occurs
    } else {
        setPos(newX, newY);
    }

    // Ensure ghost doesn't go out of bounds
    if (x() < 0 || x() > 540 || y() < 0 || y() > 460) {
        direction = QRandomGenerator::global()->bounded(4);  // Change direction if out of bounds
        setPos(x() - dx, y() - dy);  // Move back
    }
}

void Ghost::becomeFrightened() {
    frightened = true;
    updateImage(":/images/ghosts/bl1.png");
    frightenedTimer->start(10000); // Frightened for 10 seconds
}

void Ghost::stopBeingFrightened() {
    frightened = false;
    frightenedTimer->stop();
    updateImage(originalImagePath); // Reset to original image
}

bool Ghost::isFrightened() const {
    return frightened;
}

void Ghost::updateImage(const QString &imagePath) {
    setPixmap(QPixmap(imagePath));
}
