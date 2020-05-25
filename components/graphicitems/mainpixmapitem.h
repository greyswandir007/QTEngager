#ifndef MAINPIXMAPITEM_H
#define MAINPIXMAPITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>

class MainPixmapItem : public QGraphicsPixmapItem {

public:
    MainPixmapItem(QGraphicsItem *parent = nullptr);
    MainPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool mousePressed;
    qreal startX;
    qreal startY;
    qreal startPositionX;
    qreal startPositionY;
};

#endif // MAINPIXMAPITEM_H
