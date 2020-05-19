#ifndef GRAPHICSVIEWRULER_H
#define GRAPHICSVIEWRULER_H

#include <QFrame>
#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPaintEvent>
#include <QObject>
#include <QPainter>

class GraphicsViewRuler : public QFrame
{
    Q_OBJECT
public:
    explicit GraphicsViewRuler(QWidget *parent, Qt::WindowFlags f = Qt::WindowFlags());
    ~GraphicsViewRuler();

signals:
    void scaleChanged(double scale);

public slots:
    void changeScale(double scale);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void paintEvent(QPaintEvent *event);

    virtual void drawRulerPattern(double x);
    virtual void updateScale(double scale);

private:
    QPainter rulerPainter;
    Qt::Orientation rulerOrientation;

    int defaultRulerStep = 40;

    int rulerPattern[10] = {15, 25, 25, 25, 25, 20, 25, 25, 25, 25};
    int scallers[2] = {5, 2};

    double rulerStep = 40;
    double numberStep = 1;
    double currentScale = 1;

    int currentScaller = 0;
};

#endif // GRAPHICSVIEWRULER_H
