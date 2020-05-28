#ifndef MAINSVGITEM_H
#define MAINSVGITEM_H

#include <QGraphicsSvgItem>
#include <QGraphicsSceneMouseEvent>

class MainSvgItem : public QGraphicsSvgItem
{
public:
    MainSvgItem(const QString &fileName, QGraphicsItem *parentItem = nullptr);

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    bool mousePressed = false;
    QPoint startScreenPos;
    QPointF startPosition;
};

#endif // MAINSVGITEM_H
