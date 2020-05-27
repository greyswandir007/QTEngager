#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "engager/engagercontroller.h"
#include "engager/logdialog.h"

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_rectangleButton_clicked();
    void on_circleButton_clicked();
    void on_connectToEngager();
    void on_disconnectFromEngager();
    void on_comPortListUpdate();
    void on_actionConnect_triggered();
    void on_actionAuto_Connect_triggered();
    void on_actionLog_triggered();
    void on_actionMove_left_triggered();
    void on_actionMove_right_triggered();
    void on_actionMove_forward_triggered();
    void on_actionMove_backward_triggered();
    void on_actionLaser_off_triggered();
    void on_actionLaser_low_triggered();
    void on_actionLaser_middle_triggered();
    void on_actionLaser_high_triggered();
    void on_actionEngage_rect_triggered();
    void on_actionStart_engage_triggered();
    void on_actionStop_engage_triggered();
    void on_moveButton_clicked();
    void on_sendCommand_clicked();
    void on_actionAdd_image_triggered();
    void on_actionEngage_triggered();

private:
    Ui::MainWindow *ui;
    EngagerController engagerController;
    LogDialog *logDialog;

protected:
    virtual void resizeEvent(QResizeEvent *event);
};

#endif // MAINWINDOW_H
