#include "pacman.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>

Pacman::Pacman(const QString &imagePath) :
    QGraphicsPixmapItem(QPixmap(imagePath)),
    direction(1), // Start moving right
    frame(0)
{
    for (int i = 0; i < 6; ++i) {
        images[0][i] = QPixmap(QString(":/images/pacman/c%1.png").arg(i + 1)); // Left
        images[1][i] = QPixmap(QString(":/images/pacman/a%1.png").arg(i + 1));// Right
        images[2][i] =  QPixmap(QString(":/images/pacman/b%1.png").arg(i + 1));// Up
        images[3][i] = QPixmap(QString(":/images/pacman/d%1.png").arg(i + 1)); // Down
    }
    updateImage();
}

void Pacman::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Pacman received key press: " << event->key(); // Debug statement
    switch (event->key()) {
    case Qt::Key_Left:
        direction = 0;
        break;
    case Qt::Key_Right:
        direction = 1;
        break;
    case Qt::Key_Up:
        direction = 2;
        break;
    case Qt::Key_Down:
        direction = 3;
        break;
    default:
        return;
    }
    updateImage();
}

void Pacman::move()
{
    qreal dx = 0, dy = 0;
    switch (direction) {
    case 0: dx = -20; break; // Move left
    case 1: dx = 20; break;  // Move right
    case 2: dy = -20; break; // Move up
    case 3: dy = 20; break;  // Move down
    }

    qreal newX = x() + dx;
    qreal newY = y() + dy;

    QRectF newRect(newX, newY, boundingRect().width(), boundingRect().height());
    qDebug() << "New position: (" << newX << "," << newY << ")";
    qDebug() << "Bounding rect: " << newRect;

    QList<QGraphicsItem *> collidingItems = scene()->items(newRect);
    bool collision = false;

    for (QGraphicsItem *item : collidingItems) {
        if (item != this && item->data(0) == "wall") {
            collision = true;
            qDebug() << "Collision with wall at: " << item->boundingRect() << " Wall Pos: (" << item->x() << "," << item->y() << ")";
            break;
        }
    }

    if (!collision) {
        setPos(newX, newY);
        frame = (frame + 1) % 6;
        updateImage();
    } else {
        qDebug() << "Pacman collision detected";
    }
}

void Pacman::updateImage()
{
    setPixmap(images[direction][frame]);
}
