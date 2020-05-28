#include "mainsvgitem.h"
#include "maingraphicsitem.h"
#include "../itempropertiesdialog.h"

MainSvgItem::MainSvgItem(const QString &fileName, QGraphicsItem *parentItem) :
    QGraphicsSvgItem(fileName, parentItem) {
    setData(SCALE, 1.0);
    setData(POSITION_X, 0.0);
    setData(POSITION_Y, 0.0);
    setData(MAX_INTENSITY_VALUE, 255);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void MainSvgItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->buttons() | Qt::LeftButton && mousePressed) {
        qreal x = (event->screenPos().x() - startScreenPos.x()) + startPosition.x();
        qreal y = (event->screenPos().y() - startScreenPos.y()) + startPosition.y();
        setPos(x, y);
        qreal mainScale = data(MAIN_SCALE).toDouble();
        setData(POSITION_X, x / mainScale);
        setData(POSITION_Y, y / mainScale);
    } else {
        mousePressed = false;
    }
    QGraphicsSvgItem::mouseMoveEvent(event);
}

void MainSvgItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mousePressed = true;
        startScreenPos = event->screenPos();
        startPosition = pos();
        setSelected(true);
    }
    QGraphicsSvgItem::mousePressEvent(event);
}

void MainSvgItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mousePressed = false;
    }
    QGraphicsSvgItem::mouseReleaseEvent(event);
}

void MainSvgItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        ItemPropertiesDialog itemPropertiesDialog;
        itemPropertiesDialog.setItem(this);
        if (itemPropertiesDialog.exec() == QDialog::Accepted) {
            itemPropertiesDialog.updateItem();
        }
    }
    QGraphicsSvgItem::mouseDoubleClickEvent(event);
}
