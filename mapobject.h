#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <QGraphicsRectItem>
#include <QPainter>

class MapObject : public QGraphicsRectItem {
public:
    MapObject(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    QVector<QVector<int>> mapData;
};

#endif // MAPOBJECT_H
