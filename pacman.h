#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QObject>

class Pacman : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Pacman(const QString &imagePath);
    void keyPressEvent(QKeyEvent *event); // Make sure this method is public
    void move();

private:
    int direction; // 0: left, 1: right, 2: up, 3: down
    QPixmap images[4][6]; // Array to hold images for different directions and frames
    int frame; // Current frame for animation
    void updateImage(); // Update the image based on direction and frame
};

#endif // PACMAN_H
