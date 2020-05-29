#include "mainview.h"
#include "graphicitems/maingraphicsitem.h"

#include <QGraphicsItem>
#include <QWheelEvent>
#include <QDebug>
#include <QSvgRenderer>

#include <components/graphicitems/mainpixmapitem.h>
#include <components/graphicitems/mainsvgitem.h>

MainView::MainView(QWidget *parent) : QGraphicsView(parent) {
    setMouseTracking(true);
}

void MainView::updateSceneRect() {
    QRect rec(geometry());
    scene()->setSceneRect(0, 0, rec.width() - 3, rec.height() - 3);
}

void MainView::addPixmapToScene(QPixmap pixmap) {
    addItemToScene(new MainPixmapItem(pixmap));
}

void MainView::addSvgToScene(QString filename) {
    MainSvgItem *item= new MainSvgItem(filename);
    addItemToScene(item);
}

void MainView::changeScale(double scale) {
    mainScale = scale;
    scale = mainScale * mainScaleFactor;
    for(QGraphicsItem *item : scene()->items()) {
        qreal x = item->data(POSITION_X).toDouble() * mainScale;
        qreal y = item->data(POSITION_Y).toDouble() * mainScale;
        item->setScale(scale * item->data(SCALE).toDouble() * item->data(SCALE2).toDouble());
        item->setPos(x, y);
        item->setData(MAIN_SCALE, mainScale);
    }
}

void MainView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
}

void MainView::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);
    if (event->button() == Qt::MidButton) {
        mainScale = 1;
        changeScale(mainScale);
        emit scaleChanged(mainScale);
        event->accept();
    }
}

void MainView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);
}

void MainView::wheelEvent(QWheelEvent *event) {
    QGraphicsView::wheelEvent(event);
    double f = event->delta() / 120;
    double power = pow(1.1, fabs(f));
    mainScale = f > 0 ? mainScale * power : mainScale / power;
    changeScale(mainScale);
    emit scaleChanged(mainScale);
    event->accept();
}

void MainView::addItemToScene(QGraphicsItem *item) {
    item->setData(MAIN_SCALE_FACTOR, mainScaleFactor);
    item->setData(MAIN_SCALE, mainScale);
    item->setData(SCALE, 1.0);
    item->setData(POSITION_X, 0.0);
    item->setData(POSITION_Y, 0.0);
    item->setData(MAX_INTENSITY_VALUE, 255);
    item->setData(INVERT_INTENSITY, false);
    item->setData(MIRROR_X, false);
    item->setData(MIRROR_Y, false);
    item->setData(ROTATION, 0.0);
    item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    item->setScale(mainScale * mainScaleFactor * item->data(SCALE).toDouble() * item->data(SCALE2).toDouble());
    scene()->addItem(item);
}
