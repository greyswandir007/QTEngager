#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QImageReader>
#include "components/graphicitems/maingraphicsitem.h"
#include <components/graphicitems/mainsvgitem.h>
#include <engager/gcodecommands.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setupEngageController();
    connectEvents();
    addStatusBarWidgets();
}

MainWindow::~MainWindow() {
    engagerController->disconnect();
    delete ui;
    delete logDialog;
    if (mainProgram) {
        delete mainProgram;
    }
}

void MainWindow::on_rectangleButton_clicked(){
    int power2 = 0;
    if (ui->rectanglePower->value() > 10) {
        power2 = 2800;
    }
    CommandQueue sequence = creator->rectangleQueue(ui->rectangleX->value(), ui->rectangleY->value(),
                                                    ui->rectangleW->value(), ui->rectangleH->value(),
                                                    ui->rectanglePower->value(), power2, ui->rectangleSpeed->value());
    runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_circleButton_clicked(){
    CommandQueue sequence = creator->circleQueue(ui->circleX->value(), ui->circleY->value(), ui->circleR->value(),
                                                 ui->circlePower->value(), ui->circleSpeed->value());
    runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_connectToEngager() {
    ui->comPortSelect->setCurrentIndex(engagerController->connectedPortIndex());
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
    for (QString item : engagerController->getComPortList()) {
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
    if (engagerController->isConnected()) {
        engagerController->disconnectedFromEngager();
    } else {
        engagerController->engagerConnect(ui->comPortSelect->currentIndex());
    }
}

void MainWindow::on_actionAuto_Connect_triggered() {
    engagerController->setAutoConnect(ui->actionAuto_Connect->isChecked());
}

void MainWindow::on_actionLog_triggered() {
    if (logDialog->isVisible()) {
        logDialog->hide();
    } else {
        logDialog->show();
    }
}

void MainWindow::on_actionMove_left_triggered() {
    CommandQueue sequence = creator->moveXQueue(10);
    runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_actionMove_right_triggered() {
    CommandQueue sequence = creator->moveXQueue(-10);
    runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_actionMove_forward_triggered() {
    CommandQueue sequence = creator->moveYQueue(10);
    runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_actionMove_backward_triggered() {
    CommandQueue sequence = creator->moveYQueue(-10);
    runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_actionLaser_off_triggered() {
    engagerController->sendCommand(creator->gcodeCommands()->laserPowerOff());
}

void MainWindow::on_actionLaser_low_triggered() {
    engagerController->sendCommand(creator->gcodeCommands()->laserPowerLow());
}

void MainWindow::on_actionLaser_middle_triggered() {
    engagerController->sendCommand(creator->gcodeCommands()->laserPowerMedium());
}

void MainWindow::on_actionLaser_high_triggered() {
    engagerController->sendCommand(creator->gcodeCommands()->laserPowerHigh());
}

void MainWindow::on_actionEngage_rect_triggered() {
    int power2 = 0;
    if (ui->rectanglePower->value() > 10) {
        power2 = 2800;
    }
    CommandQueue sequence = creator->rectangleQueue(ui->rectangleX->value(), ui->rectangleY->value(),
                                                    ui->rectangleW->value(), ui->rectangleH->value(),
                                                    ui->rectanglePower->value(), power2, ui->rectangleSpeed->value());
    runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_actionStart_engage_triggered() {
    CommandQueue sequence = creator->circleQueue(ui->circleSpeed->value());
    runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::on_actionStop_engage_triggered() {
    engagerController->clearSequence();
}

void MainWindow::on_moveButton_clicked() {
    CommandQueue sequence = creator->moveToQueue(ui->moveX->value(), ui->moveY->value(), ui->moveSpeed->value());
    runEngagerProgram(new EngagerProgram(sequence));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    ui->mainView->updateSceneRect();
}

void MainWindow::on_sendCommand_clicked() {
    if (!ui->commandEdit->text().isEmpty()) {
        engagerController->sendCommand(ui->commandEdit->text() + "\x0a");
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
    if (mainProgram) {
        runEngagerProgram(mainProgram);
    } else {
        runEngagerProgram(new EngagerProgram(ui->mainView, creator));
    }
}

void MainWindow::on_actionClear_triggered() {
    ui->mainView->clearScene();
    if (mainProgram) {
        delete mainProgram;
    }
    mainProgram = nullptr;
}

void MainWindow::on_actionSave_2_triggered() {
    QString filename = QFileDialog::getSaveFileName();
    if(!filename.isEmpty()) {
        EngagerProgram *engagerProgram = new EngagerProgram(ui->mainView, creator);
        engagerProgram->saveProgram(filename);
    }
}

void MainWindow::on_actionOpen_2_triggered() {
    QString filename = QFileDialog::getOpenFileName();
    if(!filename.isEmpty()) {
        if (mainProgram) {
            delete mainProgram;
        }
        mainProgram = new EngagerProgram(filename);

    }
}

void MainWindow::connectEvents() {
    connect(engagerController, &EngagerController::connectedToEngager, this, &MainWindow::on_connectToEngager);
    connect(engagerController, &EngagerController::disconnectedFromEngager, this, &MainWindow::on_disconnectFromEngager);
    connect(engagerController, &EngagerController::comPortListUpdate, this, &MainWindow::on_comPortListUpdate);

    connect(ui->horizontalRuler, SIGNAL(scaleChanged(double)), ui->verticalRuler, SLOT(changeScale(double)));
    connect(ui->horizontalRuler, SIGNAL(scaleChanged(double)), ui->mainView, SLOT(changeScale(double)));

    connect(ui->verticalRuler, SIGNAL(scaleChanged(double)), ui->horizontalRuler, SLOT(changeScale(double)));
    connect(ui->verticalRuler, SIGNAL(scaleChanged(double)), ui->mainView, SLOT(changeScale(double)));

    connect(ui->mainView, SIGNAL(scaleChanged(double)), ui->horizontalRuler, SLOT(changeScale(double)));
    connect(ui->mainView, SIGNAL(scaleChanged(double)), ui->verticalRuler, SLOT(changeScale(double)));

    connect(ui->horizontalGCodeRuler, SIGNAL(scaleChanged(double)), ui->verticalGCodeRuler, SLOT(changeScale(double)));
    connect(ui->horizontalGCodeRuler, SIGNAL(scaleChanged(double)), ui->gcodeView, SLOT(changeScale(double)));

    connect(ui->verticalGCodeRuler, SIGNAL(scaleChanged(double)), ui->horizontalGCodeRuler, SLOT(changeScale(double)));
    connect(ui->verticalGCodeRuler, SIGNAL(scaleChanged(double)), ui->gcodeView, SLOT(changeScale(double)));

    connect(ui->gcodeView, SIGNAL(scaleChanged(double)), ui->verticalGCodeRuler, SLOT(changeScale(double)));
    connect(ui->gcodeView, SIGNAL(scaleChanged(double)), ui->horizontalGCodeRuler, SLOT(changeScale(double)));
}

void MainWindow::addStatusBarWidgets() {
    QLabel *timePassedLabel = new QLabel();
    QLabel *timeLeftLabel = new QLabel();
    timePassedLabel->setText(tr("Time passed: "));
    timeLeftLabel->setText(tr("Time left: "));
    timePassedLabel->setMaximumWidth(70);
    timePassedLabel->setMinimumWidth(70);
    timeLeftLabel->setMaximumWidth(70);
    timeLeftLabel->setMinimumWidth(70);
    timePassed->setMaximumWidth(70);
    timePassed->setMinimumWidth(70);
    timeLeft->setMaximumWidth(50);
    timeLeft->setMinimumWidth(50);
    timePassed->setText(" ");
    timeLeft->setText(" ");
    ui->statusBar->addWidget(timePassedLabel);
    ui->statusBar->addWidget(timePassed);
    ui->statusBar->addWidget(timeLeftLabel);
    ui->statusBar->addWidget(timeLeft);
}

void MainWindow::setupEngageController() {
    creator = new CommandCreator(new GCodeCommands());
    logDialog = new LogDialog();
    engagerController = new EngagerController(creator);
    engagerController->setTextLog(logDialog->logTextEdit());
    engagerController->setEngageProgressBar(ui->engageProgress);
    engagerController->setPassedTimeLabel(timePassed);
    engagerController->setLeftTimeLabel(timeLeft);
    for (QString item : engagerController->getComPortList()) {
        ui->comPortSelect->addItem(item);
    }
}

void MainWindow::runEngagerProgram(EngagerProgram *program) {
    showGCode(program);
    engagerController->runEngagerProgram(program);
}

void MainWindow::showGCode(EngagerProgram *program) {
    ui->gcodeView->clearScene();
    ui->gcodeView->updateSceneRect();
    ui->gcodeView->addPixmapToScene(creator->gcodeCommands()->createPixmap(program->currentProgram()));
    ui->gcodeText->setText(program->programText());
}

void MainWindow::on_actionCreate_GCode_triggered() {
    if (mainProgram) {
        runEngagerProgram(mainProgram);
    } else {
        runEngagerProgram(new EngagerProgram(ui->mainView, creator));
    }
}
