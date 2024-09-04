#include "mapobject.h"

MapObject::MapObject(QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    setRect(0, 0, 800, 600); // Set the area covered by the map

    // Define the map layout
    mapData = {
               {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
               {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
               {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
               {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
               {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
               {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
               };
}

void MapObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::blue);
    for (int y = 0; y < mapData.size(); ++y) {
        for (int x = 0; x < mapData[y].size(); ++x) {
            if (mapData[y][x] == 1) {
                painter->drawRect(x * 50, y * 50, 50, 50);
            }
        }
    }
}

QRectF MapObject::boundingRect() const {
    return rect();
}
