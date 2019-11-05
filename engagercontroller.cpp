#include "engagercontroller.h"
#include <QDebug>

EngagerController::EngagerController() {
    connectedFlag = false;
    commandSend = false;
    connectingFlag = false;
    zeroCoordSet = false;
    connectedFlag2 = false;
    autoConnectFlag = false;
    connect(&serialPort, &QSerialPort::readyRead, this, &EngagerController::on_serialPortRead);
    connect(&mainTimer, &QTimer::timeout, this, &EngagerController::on_timerEvent);
    updateComPortList();
    mainTimer.start(500);
    connectionPortIndex = 0;
    passedCommandCount = 0;
    timePassed = 0;
    totalCommandCount = 0;
    sequenceTimeStart = QDateTime::currentDateTime();
}

void EngagerController::sendCommand(QString command) {
    if (connectedFlag2) {
        if (!commandSend) {
            sendCommandFromSequence(command);
        } else {
            commandList.append(command);
        }
    }
}

void EngagerController::addCommand(QString command) {
    if (totalCommandCount == 0) {
        sequenceTimeStart = QDateTime::currentDateTime();
        passedCommandCount = 0;
    }
    commandList.append(command);
    totalCommandCount++;
}

void EngagerController::addCommandList(QStringList commands){
    if (totalCommandCount == 0) {
        sequenceTimeStart = QDateTime::currentDateTime();
        passedCommandCount = 0;
    }
    commandList.append(commands);
    totalCommandCount += commands.count();
}

void EngagerController::updateComPortList() {
    comPortList.clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        comPortList.append(info.portName());
    }
    emit comPortListUpdate();
}

QStringList EngagerController::getComPortList() {
    return comPortList;
}

void EngagerController::setTextLog(QTextEdit *textLog) {
    this->textLog = textLog;
}

void EngagerController::engagerConnect(int index) {
    if (index >= 0 && index < comPortList.count()) {
        serialPort.close();
        serialPort.setPortName(comPortList.at(index));
        serialPort.setBaudRate(115200);
        if (serialPort.open(QIODevice::ReadWrite)) {
            connectedFlag = true;
            connectingFlag = true;
            commandSend = false;
        }
    }
}

void EngagerController::engagerDisconnect() {
    serialPort.close();
    if (connectedFlag) {
        if (connectedFlag2) {
            connectedFlag2 = false;
            emit disconnectedFromEngager();
            addLog("Disconnected from engager");
        }
        connectedFlag = false;
        connectingFlag = false;
        commandSend = false;
    }
}

void EngagerController::createLaserPowerCommand(EngagerController::LaserPower power){
    switch (power) {
    case LaserPower::OFF:
        sendCommand("M05 L0\x0A");
        break;
    case LaserPower::LOW:
        sendCommand("M3 L2 P0\x0A");
        break;
    case LaserPower::MIDDLE:
        sendCommand("M3 L255 P0\x0A");
        break;
    case LaserPower::HIGH:
        sendCommand("M3 L255 P2800\x0A");
        break;
    }
}

void EngagerController::moveX(qreal offset) {
    QString offs;
    move(offs.sprintf("X%2.15lg", offset));
}

void EngagerController::moveY(qreal offset) {
    QString offs;
    move(offs.sprintf("Y%2.15lg", offset));
}

void EngagerController::moveX(int offset) {
    QString offs;
    move(offs.sprintf("X%i", offset));
}

void EngagerController::moveY(int offset) {
    QString offs;
    move(offs.sprintf("Y%i", offset));
}

void EngagerController::moveTo(qreal x, qreal y, int speed) {
    QStringList commands;
    if (!zeroCoordSet) {
        commands.append("M05 L0\x0A");
        commands.append("G00 X0 Y0 F3000\x0A");
        zeroCoordSet = true;
    }
    commands.append("G91\x0A");
    commands.append(createMoveCommand(x, y, speed));
    addCommandList(commands);
}

void EngagerController::rectangle() {
    QStringList commands;
    commands.append("G21\x0A");
    commands.append("G92\x0A");
    commands.append("G90\x0A");
    commands.append("M05 P0 L0\x0A");
    commands.append("G00 X4.404825582730206 Y-1.704825582730206 L0 F3000\x0A");
    commands.append("M03 L2 P0\x0A");
    commands.append("G00 X27.195174417269797 Y-1.704825582730206 L0 F3000\x0A");
    commands.append("G00 X27.195174417269797 Y-24.495174417269794 L0 F3000\x0A");
    commands.append("G00 X4.404825582730206 Y-24.495174417269794 L0 F3000\x0A");
    commands.append("G00 X4.404825582730206 Y-1.704825582730206 L0 F3000\x0A");
    commands.append("M05 L0\x0A");
    commands.append("G00 X0 Y0 L0 F3000\x0A");
    commands.append("M05 L0\x0A");
    commands.append("G00 X0 Y0 F3000\x0A");

    addCommandList(commands);
}

void EngagerController::rectangle(qreal x, qreal y, qreal w, qreal h, int power, int power2, int speed) {
    QStringList commands;
    commands.append(getStartSequence());
    commands.append(createMoveCommand(x, y, MAX_SPEED));
    commands.append(createLaserPowerCommand(true, power, power2));
    commands.append(createMoveCommand(x+w, y, speed));
    commands.append(createMoveCommand(x+w, y+h, speed));
    commands.append(createMoveCommand(x, y+h, speed));
    commands.append(createMoveCommand(x, y, speed));
    commands.append(createLaserPowerCommand(false, 0, 0));
    commands.append(getCenterSequence());

    addCommandList(commands);
}

void EngagerController::circle() {
    QStringList commands;
    commands.append("G21\x0A");
    commands.append("G90\x0A");
    commands.append("G92\x0A");
    commands.append("M05 L0\x0A");
    commands.append("G00 X0 Y0 F3000\x0A");
    commands.append("G92\x0A");
    commands.append("G00 X5.0000 Y-5.9000 F3000\x0A");
    commands.append("G03 I0.0000 J0.0000 L255\x0A");
    commands.append("G00 X4.4048 Y-13.1000 F3000\x0A");
    commands.append("G03 I11.3952 J0.0000 L255\x0A");
    commands.append("M05 L0\x0A");
    commands.append("G00 X0 Y0 F3000\x0A");

    addCommandList(commands);
}

void EngagerController::circle(qreal x, qreal y, qreal r, int power, int power2, int speed) {
    QStringList commands;
    commands.append(getStartSequence());
    commands.append("G00 X0 Y0 F3000\x0A");
    commands.append("G92\x0A");
    commands.append(createMoveCommand2(5, -5.9, 3000));
    commands.append(createArcCommand(0, 0, 255, speed));
    commands.append(createMoveCommand2(4.4048, -13.1, speed));
    commands.append(createArcCommand(11.3952, 0, 255, speed));
    commands.append(getCenterSequence());

    addCommandList(commands);
}

int EngagerController::connectedPortIndex() {
    return connectionPortIndex;
}

bool EngagerController::isConnected() {
    return connectedFlag2;
}

void EngagerController::setAutoConnect(bool autoConnect) {
    autoConnectFlag = autoConnect;
}

void EngagerController::setEngageProgressBar(QProgressBar *engageProgress) {
    this->engageProgress = engageProgress;
}

void EngagerController::setPassedTimeLabel(QLabel *passedTime) {
    passedTimeLabel = passedTime;
}

void EngagerController::setLeftTimeLabel(QLabel *leftTime) {
    leftTimeLable = leftTime;
}

void EngagerController::clearSequence() {
    commandList.clear();
    commandList.append(getCenterSequence());
    totalCommandCount = 3;
}

void EngagerController::engageImage(QImage image, qreal x, qreal y, int scaleX, int scaleY, int maxIntensity) {
    QStringList commands;
    commands.append(getStartSequence());
    commands.append("G00 X0 Y0 F3000\x0A");
    commands.append("G92\x0A");
    qreal stepX = 0.1 * scaleX;
    qreal startX = x;
    for (int i = 0; i < image.height(); i++) {
        for (int k = 0; k < scaleY; k++) {
            x = startX;
            for (int j = 0; j < image.width(); j++) {
                int power = 255 - QColor(image.pixel(j, i)).toHsl().lightness();
                commands.append(createMoveCommand(x, y, MAX_SPEED));
                commands.append(createLaserPowerCommand(static_cast<int>((maxIntensity/255.0) * power)));
                if (scaleX > 1) {
                    commands.append(createMoveCommand(x + stepX - 0.1, y, MAX_SPEED));
                }
                commands.append("M05 L0\x0A");
                x += stepX;
            }
            y += 0.1;
            addCommandList(commands);
            commands.clear();
        }
    }
    addCommandList(getCenterSequence());
}

void EngagerController::on_serialPortRead() {
    QString received = QString::fromLatin1(serialPort.readAll());
    if (received != "ok\r\n") {
        qDebug() << received;
        addLog("Received from com port: " + received);
    } else {
        addLog(received);
    }
    if (!commandSend) {
        if (connectingFlag && received.endsWith("ok\r\n")) {
            connectingFlag = false;
            connectedFlag2 = true;
            addLog("Connected to engager on port: " + serialPort.portName());
            emit connectedToEngager();
            sendCommand("$21 P-2\x0A");
        }
    } else if (received.endsWith("ok\r\n")) {
        commandSend = false;
        sendNextCommand();
    }
}

void EngagerController::on_timerEvent() {
    if (connectedFlag2 && !commandSend) {
        sendNextCommand();
    }
    if (autoConnectFlag && !connectedFlag2) {
        if (!connectedFlag) {
            timePassed = 0;
            if (connectionPortIndex < comPortList.count()) {
                engagerConnect(connectionPortIndex);
            } else {
                updateComPortList();
                connectionPortIndex = 0;
                engagerConnect(connectionPortIndex);
            }
        } else if (connectedFlag && connectingFlag) {
            timePassed++;
            if (timePassed > 4) {
                engagerDisconnect();
                connectionPortIndex++;
            }
        }
    }
    if (!commandList.isEmpty()) {
        QDateTime current = QDateTime::currentDateTime();
        qint64 tm = current.toMSecsSinceEpoch() - sequenceTimeStart.toMSecsSinceEpoch();
        current = current.fromMSecsSinceEpoch(tm);
        double forOneCommmand = static_cast<double>(tm) / passedCommandCount;
        passedTimeLabel->setText(current.toString("hh:mm:ss"));
        tm = static_cast<qint64>(forOneCommmand * (totalCommandCount - passedCommandCount));
        current = current.fromMSecsSinceEpoch(tm);
        leftTimeLable->setText(current.toString("hh:mm:ss"));
    }
}

void EngagerController::sendCommandFromSequence(QString command) {
    addLog("Sending command to com port: " + command);
    serialPort.write(command.toLatin1());
    commandSend = true;
}

void EngagerController::sendNextCommand() {
    if (!commandList.isEmpty()) {
        passedCommandCount++;
        sendCommandFromSequence(commandList.at(0));
        commandList.removeAt(0);
        if (engageProgress) {
            float progress = (totalCommandCount - commandList.count()) * 100.0f / static_cast<float>(totalCommandCount);
            engageProgress->setValue(static_cast<int>(progress));
        }
        if (commandList.isEmpty()) {
            totalCommandCount = 0;
        }
    } else {
        totalCommandCount = 0;
        passedCommandCount = 0;
    }
}

void EngagerController::move(QString offset) {
    QStringList commands;
    if (!zeroCoordSet) {
        commands.append("M05 L0\x0A");
        commands.append("G00 X0 Y0 F3000\x0A");
        zeroCoordSet = true;
    }
    commands.append("G91\x0A");
    commands.append("G01 " + offset + " L0 F3000\x0A");
    addCommandList(commands);
}

void EngagerController::addLog(QString logLine) {
    if (textLog) {
        if (logLine == "ok\x0d\x0a") {
            textLog->setFocus();
            textLog->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
            textLog->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
            textLog->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
            QString last = textLog->textCursor().selectedText();
            textLog->textCursor().removeSelectedText();
            textLog->textCursor().deletePreviousChar();
            last += " -> ok";
            textLog->append(last);
        } else {
            QDateTime time = QDateTime::currentDateTime();
            logLine = logLine.replace(0x0A,"\\x0A");
            logLine = logLine.replace(0x0D,"\\x0D");
            textLog->append(time.toString("[hh:mm:ss,zzz]: ") + logLine);
        }
        textLog->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    }
}

QString EngagerController::createLaserPowerCommand(bool enable, int power1, int power2) {
    QString command;
    if (enable) {
        return QString().sprintf("M3 L%i P%i\x0A", power1, power2);
    } else {
        return "M05 L0\x0A";
    }
}

QString EngagerController::createLaserPowerCommand(int power) {
    QString command;
    if (power > 255) {
        power = 255;
    }
    double power2 = 0.00784292452830189 * static_cast<double>(power);
    if (power > 0) {
        return QString().sprintf("M3 L%i P%1.4f\x0A", power, power2);
    } else {
        return "M05 L0\x0A";
    }
}

QString EngagerController::createMoveCommand(qreal x, qreal y, int speed) {
    return QString().sprintf("G00 X%2.15f Y%2.15f L0 F%i\x0A", x, y, speed);
}

QString EngagerController::createMoveCommand2(qreal x, qreal y, int speed) {
    return QString().sprintf("G00 X%2.4f Y%2.4f F%i\x0A", x, y, speed);
}

QString EngagerController::createArcCommand(qreal i, qreal j, int l, int speed) {
    return QString().sprintf("G03 I%2.4f J%2.4f L%i F%i\x0A", i, j, l, speed);
}

QStringList EngagerController::getStartSequence() {
    QStringList commands;
    commands.append("G21\x0A");
    commands.append("G90\x0A");
    commands.append("G92\x0A");
    commands.append("M05 L0\x0A");
    return commands;
}

QStringList EngagerController::getCenterSequence() {
    QStringList commands;
    commands.append("M05 L0\x0A");
    commands.append("G00 X0 Y0 L0 F3000\x0A");
    commands.append("G00 X0 Y0 F3000\x0A");
    return commands;
}
