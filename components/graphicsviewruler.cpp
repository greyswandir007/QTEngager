#include "graphicsviewruler.h"

GraphicsViewRuler::GraphicsViewRuler(QWidget* parent, Qt::WindowFlags f)
    : QFrame(parent, f) {
}

GraphicsViewRuler::~GraphicsViewRuler(){
}

void GraphicsViewRuler::changeScale(double scale) {
    updateScale(scale);
    repaint();
}

void GraphicsViewRuler::mouseMoveEvent(QMouseEvent *event) {
    QFrame::mouseMoveEvent(event);//stub
}

void GraphicsViewRuler::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::MidButton) {
        currentScale = 1;
        rulerStep = defaultRulerStep;
        numberStep = 1;
        currentScaller = 0;
        emit scaleChanged(currentScale);
        repaint();
        event->accept();
    }
}

void GraphicsViewRuler::mouseReleaseEvent(QMouseEvent *event) {
    QFrame::mouseMoveEvent(event);//stub
}

void GraphicsViewRuler::wheelEvent(QWheelEvent *event) {
    double f = event->delta() / 120;
    double power = pow(1.1, fabs(f));
    if (f > 0) {
        currentScale *= power;
    } else {
        currentScale /= power;
    }
    updateScale(currentScale);
    emit scaleChanged(currentScale);
    repaint();
    event->accept();
}

void GraphicsViewRuler::paintEvent(QPaintEvent *event) {
    rulerOrientation = rect().width() > rect().height() ? Qt::Horizontal : Qt::Vertical;
    rulerPainter.begin(this);
    int size = rulerOrientation == Qt::Horizontal ? rect().width() : rect().height();
    double offset = 0;
    double i = 0;
    bool first = true;
    while(offset < size) {
        QString mark = QString("%1").arg(i);
        int textOffset = static_cast<int>(first ? offset : offset - 3 * mark.length());
        if (rulerOrientation == Qt::Horizontal) {
            rulerPainter.drawText(textOffset, 10, mark);
        } else {
            rulerPainter.save();
            rulerPainter.rotate(-90);
            rulerPainter.drawText(-5-textOffset, 10, mark);
            rulerPainter.restore();
        }
        drawRulerPattern(offset);
        offset += rulerStep;
        i += numberStep;
        first = false;
    }
    rulerPainter.end();
    event->accept();
}

void GraphicsViewRuler::drawRulerPattern(double offset) {
    double smallStep = rulerStep / 10.0;
    double ofs = offset;
    if (rulerOrientation == Qt::Horizontal) {
        for(int len : rulerPattern) {
            rulerPainter.drawLine(static_cast<int>(ofs), len, static_cast<int>(ofs), rect().height()-1);
            ofs += smallStep;
        }
    } else {
        for(int len : rulerPattern) {
            rulerPainter.drawLine(len, static_cast<int>(ofs), rect().width()-1, static_cast<int>(ofs));
            ofs += smallStep;
        }
    }
}

void GraphicsViewRuler::updateScale(double scale) {
    double newStep = defaultRulerStep * scale;
    numberStep = 1;
    currentScaller = 0;
    if (newStep > 20) {
        while (newStep > 100) {
            newStep /= scallers[!currentScaller];
            numberStep /= scallers[!currentScaller];
            currentScaller = !currentScaller;
        }
    } else {
        while (newStep < 20) {
            newStep *= scallers[currentScaller];
            numberStep *= scallers[currentScaller];
            currentScaller = !currentScaller;
        }
    }
    rulerStep = newStep;
}
