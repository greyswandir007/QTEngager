#include "engager/gcodehelper.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QImageReader>
#include "engager/engagercommand.h"
#include "engager/gcodeconst.h"
#include "components/graphicitems/maingraphicsitem.h"
#include <components/graphicitems/mainsvgitem.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    logDialog = new LogDialog();
    engagerController.setTextLog(logDialog->logTextEdit());
    engagerController.setEngageProgressBar(ui->engageProgress);
    engagerController.setPassedTimeLabel(ui->timePassedValue);
    engagerController.setLeftTimeLabel(ui->timeLeftValue);
    for (QString item : engagerController.getComPortList()) {
        ui->comPortSelect->addItem(item);
    }
    connect(&engagerController, &EngagerController::connectedToEngager, this, &MainWindow::on_connectToEngager);
    connect(&engagerController, &EngagerController::disconnectedFromEngager, this, &MainWindow::on_disconnectFromEngager);
    connect(&engagerController, &EngagerController::comPortListUpdate, this, &MainWindow::on_comPortListUpdate);
    ui->mainView->setScene(new QGraphicsScene());
    QRect rec(ui->mainView->geometry());
    ui->mainView->scene()->setSceneRect(0, 0, rec.width() - 3, rec.height() - 3);

    connect(ui->horizontalRuler, SIGNAL(scaleChanged(double)), ui->verticalRuler, SLOT(changeScale(double)));
    connect(ui->horizontalRuler, SIGNAL(scaleChanged(double)), ui->mainView, SLOT(changeScale(double)));

    connect(ui->verticalRuler, SIGNAL(scaleChanged(double)), ui->horizontalRuler, SLOT(changeScale(double)));
    connect(ui->verticalRuler, SIGNAL(scaleChanged(double)), ui->mainView, SLOT(changeScale(double)));

    connect(ui->mainView, SIGNAL(scaleChanged(double)), ui->horizontalRuler, SLOT(changeScale(double)));
    connect(ui->mainView, SIGNAL(scaleChanged(double)), ui->verticalRuler, SLOT(changeScale(double)));
}

MainWindow::~MainWindow() {
    engagerController.disconnect();
    delete ui;
    delete logDialog;
}

void MainWindow::on_rectangleButton_clicked(){
    int power2 = 0;
    if (ui->rectanglePower->value() > 10) {
        power2 = 2800;
    }
    CommandQueue sequence = GCodeHelper::rectangleQueue(ui->rectangleX->value(), ui->rectangleY->value(),
                                                        ui->rectangleW->value(), ui->rectangleH->value(),
                                                        ui->rectanglePower->value(), power2, ui->rectangleSpeed->value());
    engagerController.runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_circleButton_clicked(){
    CommandQueue sequence = GCodeHelper::circleQueue(ui->circleX->value(), ui->circleY->value(), ui->circleR->value(),
                                                     ui->circlePower->value(), ui->circleSpeed->value());
    engagerController.runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_connectToEngager() {
    ui->comPortSelect->setCurrentIndex(engagerController.connectedPortIndex());
    ui->comPortSelect->setEnabled(false);
    ui->actionConnect->setIcon(QIcon(":/qtEngager/images/pict/Disconnected.svg"));
    ui->actionConnect->setText("Disconnect");
}

void MainWindow::on_disconnectFromEngager() {
    ui->comPortSelect->setEnabled(true);
    ui->actionConnect->setIcon(QIcon(":/qtEngager/images/pict/Connected.svg"));
    ui->actionConnect->setText("Connect");
}

void MainWindow::on_comPortListUpdate() {
    QString text = ui->comPortSelect->currentText();
    int index = -1, i = 0;
    ui->comPortSelect->clear();
    for (QString item : engagerController.getComPortList()) {
        ui->comPortSelect->addItem(item);
        if (item == text) {
            index = i;
        }
        i++;
    }
    if (index >= 0) {
        ui->comPortSelect->setCurrentIndex(index);
    }
}

void MainWindow::on_actionConnect_triggered() {
    if (engagerController.isConnected()) {
        engagerController.disconnectedFromEngager();
    } else {
        engagerController.engagerConnect(ui->comPortSelect->currentIndex());
    }
}

void MainWindow::on_actionAuto_Connect_triggered() {
    engagerController.setAutoConnect(ui->actionAuto_Connect->isChecked());
}

void MainWindow::on_actionLog_triggered() {
    if (logDialog->isVisible()) {
        logDialog->hide();
    } else {
        logDialog->show();
    }
}

void MainWindow::on_actionMove_left_triggered() {
    CommandQueue sequence = GCodeHelper::moveXQueue(10);
    engagerController.runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_actionMove_right_triggered() {
    CommandQueue sequence = GCodeHelper::moveXQueue(-10);
    engagerController.runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_actionMove_forward_triggered() {
    CommandQueue sequence = GCodeHelper::moveYQueue(10);
    engagerController.runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_actionMove_backward_triggered() {
    CommandQueue sequence = GCodeHelper::moveYQueue(-10);
    engagerController.runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_actionLaser_off_triggered() {
    engagerController.sendCommand(GCodeConst::laserPowerOff());
}

void MainWindow::on_actionLaser_low_triggered() {
    engagerController.sendCommand(GCodeConst::laserPowerLow());
}

void MainWindow::on_actionLaser_middle_triggered() {
    engagerController.sendCommand(GCodeConst::laserPowerMedium());
}

void MainWindow::on_actionLaser_high_triggered() {
    engagerController.sendCommand(GCodeConst::laserPowerHigh());
}

void MainWindow::on_actionEngage_rect_triggered() {
    int power2 = 0;
    if (ui->rectanglePower->value() > 10) {
        power2 = 2800;
    }
    CommandQueue sequence = GCodeHelper::rectangleQueue(ui->rectangleX->value(), ui->rectangleY->value(),
                                                        ui->rectangleW->value(), ui->rectangleH->value(),
                                                        ui->rectanglePower->value(), power2, ui->rectangleSpeed->value());
    engagerController.runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_actionStart_engage_triggered() {
    CommandQueue sequence = GCodeHelper::circleQueue(ui->circleSpeed->value());
    engagerController.runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_actionStop_engage_triggered() {
    engagerController.clearSequence();
}

void MainWindow::on_moveButton_clicked() {
    CommandQueue sequence = GCodeHelper::moveToQueue(ui->moveX->value(), ui->moveY->value(), ui->moveSpeed->value());
    engagerController.runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    ui->mainView->updateSceneRect();
}

void MainWindow::on_sendCommand_clicked() {
    if (!ui->commandEdit->text().isEmpty()) {
        engagerController.sendCommand(ui->commandEdit->text() + "\x0a");
        ui->commandEdit->setText("");
    }
}

void MainWindow::on_actionAdd_image_triggered() {
    QString filename = QFileDialog::getOpenFileName();
    QImage image;
    if (filename.endsWith(".svg")) {
        ui->mainView->addSvgToScene(filename);
    } else if (image.load(filename)) {
        ui->mainView->addPixmapToScene(QPixmap::fromImage(image));
    }
    ui->mainView->updateSceneRect();
}

void MainWindow::on_actionEngage_triggered() {
    CommandQueue sequence;
    for(QGraphicsItem *item : ui->mainView->scene()->items()) {
        qreal multiply = 0.1 / item->data(MAIN_SCALE_FACTOR).toDouble();
        qreal scale = item->data(SCALE).toDouble();
        qreal x = item->data(POSITION_X).toDouble() * multiply;
        qreal y = item->data(POSITION_Y).toDouble() * multiply;
        int maxIndensity = item->data(MAX_INTENSITY_VALUE).toInt();
        const QGraphicsPixmapItem *pixmapItem = dynamic_cast<const QGraphicsPixmapItem*>(item);
        if (pixmapItem != nullptr) {
            sequence.append(GCodeHelper::engageImageQueue(pixmapItem->pixmap().toImage(), x, y, scale, scale, maxIndensity));
        } else {
            MainSvgItem *svgItem = dynamic_cast<MainSvgItem*>(item);
            if (svgItem != nullptr) {
                scale = 1;
                QPixmap pixmap = svgItem->renderPixmap();
                qDebug() << pixmap;
                sequence.append(GCodeHelper::engageImageQueue(pixmap.toImage(), x, y, scale, scale, maxIndensity));
            }
        }
    }
    engagerController.runEngagerProgram(new EngagerProgram(sequence));
}
