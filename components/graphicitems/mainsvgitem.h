#ifndef MAINSVGITEM_H
#define MAINSVGITEM_H

#include <QGraphicsSvgItem>
#include <QGraphicsSceneMouseEvent>
#include <QSvgRenderer>

class MainSvgItem : public QGraphicsSvgItem
{
public:
    MainSvgItem(const QString &fileName, QGraphicsItem *parentItem = nullptr);
    ~MainSvgItem();

    QPixmap renderPixmap();

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    void setMainData();
    void createRootElement(const QString &fileName);
    void updateDimensions(QString width, QString height);

    bool mousePressed = false;
    QPoint startScreenPos;
    QPointF startPosition;
    QSvgRenderer *renderer;
    qreal originalWidthMM;
    qreal originalHeightMM;
};

#endif // MAINSVGITEM_H
