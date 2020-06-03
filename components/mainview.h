#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QGraphicsView>
#include <QObject>

class MainView : public QGraphicsView
{
    Q_OBJECT
public:
    MainView(QWidget *parent);

    void updateSceneRect();
    void addPixmapToScene(QPixmap pixmap);
    void addSvgToScene(QString filename);
    void clearScene();

signals:
    void scaleChanged(double scale);

public slots:
    void changeScale(double scale);


protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

private:
    double mainScale = 1;
    double mainScaleFactor = 0.4;

    void addItemToScene(QGraphicsItem *item);
};

#endif // MAINVIEW_H
