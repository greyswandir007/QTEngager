#include "mainsvgitem.h"
#include "maingraphicsitem.h"
#include "../itempropertiesdialog.h"
#include <QDebug>
#include <QDomDocument>
#include <QDomNode>
#include <QFile>
#include <QPainter>

MainSvgItem::MainSvgItem(const QString &fileName, QGraphicsItem *parentItem) :
    QGraphicsSvgItem(parentItem) {
    setMainData();
    createRootElement(fileName);
}

MainSvgItem::~MainSvgItem() {
    delete renderer;
}

QPixmap MainSvgItem::renderPixmap() {
    QRectF rect = renderer->boundsOnElement("qtEngagerMainGraphicElement123");
    qreal scale = data(SCALE).toDouble() * data(SCALE2).toDouble();
    QPixmap pix(static_cast<int>(rect.width() * scale), static_cast<int>(rect.height()*scale));
    pix.fill(QColor::fromRgb(255, 255, 255));
    QPainter painter(&pix);
    renderer->render(&painter, "qtEngagerMainGraphicElement123");
    painter.end();
    return pix;
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

void MainSvgItem::setMainData() {
    setData(SCALE, 1.0);
    setData(SCALE2, 1.0);
    setData(POSITION_X, 0.0);
    setData(POSITION_Y, 0.0);
    setData(MAX_INTENSITY_VALUE, 255);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void MainSvgItem::createRootElement(const QString &fileName) {
    QDomDocument doc;
    QDomElement svg;
    QDomElement svg2;
    QDomElement g;
    QFile file;
    file.setFileName(fileName);
    if (file.open(QFile::ReadOnly)) {
        doc.setContent(file.readAll());
        svg = doc.firstChildElement("svg");
        svg2 = doc.createElement("svg");
        for (int i = 0; i < svg.attributes().size(); i++) {
            QDomNode attr = svg.attributes().item(i);
            svg2.setAttribute(attr.nodeName(), attr.nodeValue());
        }
        g = doc.createElement("g");
        g.setAttribute("id", "qtEngagerMainGraphicElement123");
        QDomNode element = svg.firstChild();
        qDebug() << svg.childNodes().count();
        int nodeCount = svg.childNodes().count();
        for (int i = 0; i < nodeCount; i++) {
            QDomNode node = svg.childNodes().at(0);
            if (node.nodeName() == "title" || node.nodeName() == "desc"
                    || (node.childNodes().count() == 0 && node.nodeName() == "g")) {
                svg2.appendChild(node);
            } else {
                g.appendChild(node);
            }
        }
        svg2.appendChild(g);
        doc.removeChild(svg);
        doc.appendChild(svg2);
        renderer = new QSvgRenderer(doc.toByteArray());
        setSharedRenderer(renderer);
        setElementId("qtEngagerMainGraphicElement123");
        updateDimensions(svg.attribute("width"), svg.attribute("height"));
        setData(SCALE2, originalWidthMM / renderer->viewBoxF().width() * 10);
    }
}

void MainSvgItem::updateDimensions(QString width, QString height) {
    if (width.endsWith("px")) {
        setData(SCALE2, 1.0);
        originalWidthMM = width.remove(width.length() - 2, 2).toDouble() * 0.1;
        originalHeightMM = height.remove(width.length() - 2, 2).toDouble() * 0.1;
    } else if (width.endsWith("in")) {
        setData(SCALE2, 7.14375); //96.0
        originalWidthMM = width.remove(width.length() - 2, 2).toDouble() * 25.4;
        originalHeightMM = height.remove(width.length() - 2, 2).toDouble() * 25.4;
    } else if (width.endsWith("cm")) {
        setData(SCALE2, 2.8125); //37.795
        originalWidthMM = width.remove(width.length() - 2, 2).toDouble() * 10;
        originalHeightMM = height.remove(width.length() - 2, 2).toDouble() * 10;
    } else if (width.endsWith("mm")) {
        setData(SCALE2, 0.28125);
        originalWidthMM = width.remove(width.length() - 2, 2).toDouble();
        originalHeightMM = height.remove(width.length() - 2, 2).toDouble();
    } else if (width.endsWith("pt")) {
        setData(SCALE2, 1.333333333);
        originalWidthMM = width.remove(width.length() - 2, 2).toDouble() * 0.1333333;
        originalHeightMM = height.remove(width.length() - 2, 2).toDouble() * 0.1333333;
    } else if (width.endsWith("pc")) {
        setData(SCALE2, 16.0);
        originalWidthMM = width.remove(width.length() - 2, 2).toDouble() * 1.6;
        originalHeightMM = height.remove(width.length() - 2, 2).toDouble() * 1.6;
    } else {
        setData(SCALE2, 1.0);
        originalWidthMM = width.toDouble() * 0.1;
        originalHeightMM = height.toDouble() * 0.1;
        if (originalWidthMM == 0.0) {
            originalWidthMM = width.remove(width.length() - 2, 2).toDouble() * 0.1;
            originalHeightMM = height.remove(width.length() - 2, 2).toDouble() * 0.1;
        }
    }
}
