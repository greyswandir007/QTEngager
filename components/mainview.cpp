#include "mainview.h"
#include "graphicitems/maingraphicsitem.h"

#include <QGraphicsItem>
#include <QWheelEvent>
#include <QDebug>

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
    addItemToScene(new MainSvgItem(filename));
}

void MainView::changeScale(double scale) {
    mainScale = scale;
    for(QGraphicsItem *item : scene()->items()) {
        qreal scale = item->data(SCALE).toDouble();
        qreal x = item->data(POSITION_X).toDouble() * mainScale;
        qreal y = item->data(POSITION_Y).toDouble() * mainScale;
        item->setScale(mainScaleFactor * mainScale * scale);
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
    if (f > 0) {
        mainScale *= power;
    } else {
        mainScale /= power;
    }
    changeScale(mainScale);
    emit scaleChanged(mainScale);
    event->accept();
}

void MainView::addItemToScene(QGraphicsItem *item) {
    item->setScale(mainScaleFactor * mainScale);
    item->setData(MAIN_SCALE_FACTOR, mainScaleFactor);
    item->setData(MAIN_SCALE, mainScale);
    scene()->addItem(item);
}
