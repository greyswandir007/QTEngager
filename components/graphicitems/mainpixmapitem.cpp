#include "mainpixmapitem.h"
#include "maingraphicsitem.h"
#include <QDebug>

MainPixmapItem::MainPixmapItem(QGraphicsItem *parent) :
    QGraphicsPixmapItem (parent) {
    mousePressed = false;
}

MainPixmapItem::MainPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent) :
    QGraphicsPixmapItem (pixmap, parent) {
    mousePressed = false;
}

void MainPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->buttons() | Qt::LeftButton && mousePressed) {
        qreal x = (event->screenPos().x() - startX) + startPositionX;
        qreal y = (event->screenPos().y() - startY) + startPositionY;
        setPos(x, y);
        qreal scale = data(SCALE).toDouble();
        setData(POSITION_X, x / scale);
        setData(POSITION_Y, y / scale);
    } else {
        mousePressed = false;
    }
}

void MainPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "Pressed";
    if (event->button() == Qt::LeftButton) {
        mousePressed = true;
        startX = event->screenPos().x();
        startY = event->screenPos().y();
        startPositionX = pos().x();
        startPositionY = pos().y();
        qDebug() << startX << startY;
        qDebug() << startPositionX << startPositionY;
        setSelected(true);
    }
}

void MainPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mousePressed = false;
    }
}
