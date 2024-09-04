#ifndef GHOST_H
#define GHOST_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>

class Ghost : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Ghost(const QString &imagePath);
    void move();
    void setExitMode(bool mode);
    bool hasExited() const;
    void setExited(bool exited);

    void becomeFrightened();
    void stopBeingFrightened();
    bool isFrightened() const;

private:
    bool exitMode;
    bool exited;
    bool frightened;
    int direction; // 0: left, 1: right, 2: up, 3: down
    QTimer *frightenedTimer;
    QString originalImagePath; // Store the original image path

    void updateImage(const QString &imagePath);
};

#endif // GHOST_H
