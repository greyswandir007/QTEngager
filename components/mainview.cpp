#include "mainview.h"

#include <QGraphicsItem>
#include <QWheelEvent>

MainView::MainView(QWidget *parent) : QGraphicsView(parent) {

}

void MainView::updateSceneRect() {
    QRect rec(geometry());
    scene()->setSceneRect(0, 0, rec.width() - 3, rec.height() - 3);
}

void MainView::addPixmapToScene(QPixmap pixmap, int scale) {
    QGraphicsPixmapItem *item = scene()->addPixmap(pixmap);
    item->setScale(mainScaleFactor * mainScale * scale);
    item->setData(0, scale);
}

void MainView::changeScale(double scale) {
    mainScale = scale;
    for(QGraphicsItem *item : scene()->items()) {
        double scale = item->data(0).toDouble();
        item->setScale(mainScaleFactor * mainScale * scale);
    }
}

void MainView::mouseMoveEvent(QMouseEvent *event) {

}

void MainView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::MidButton) {
        mainScale = 1;
        changeScale(mainScale);
        emit scaleChanged(mainScale);
        event->accept();
    }
}

void MainView::mouseReleaseEvent(QMouseEvent *event) {

}

void MainView::wheelEvent(QWheelEvent *event) {
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
