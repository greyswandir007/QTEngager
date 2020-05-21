#include "engagercontroller.h"
#include "gcodehelper.h"
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
    timePassed = 0;
    sequenceTimeStart = QDateTime::currentDateTime();
    engagerProgram = nullptr;
}

void EngagerController::sendCommand(const QString &command) {
    if (connectedFlag2 && !commandSend && !engagerProgram) {
        sendCommandFromSequence(command);
    }
}

void EngagerController::sendCommand(const EngagerCommand &command) {
    if (connectedFlag2 && !commandSend && !engagerProgram) {
        sendCommandFromSequence(command.getCommand());
    }
}

void EngagerController::updateComPortList() {
    comPortList.clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        comPortList.append(info.portName());
    }
    emit comPortListUpdate();
}

QStringList EngagerController::getComPortList() const {
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

void EngagerController::runEngagerProgram(EngagerProgram *program) {
    if (connectedFlag2) {
        if (engagerProgram != nullptr) {
            delete engagerProgram;
        }
        engagerProgram = program;
        sendCommandFromSequence(engagerProgram->pullCommand());
    }
}

int EngagerController::connectedPortIndex() const {
    return connectionPortIndex;
}

bool EngagerController::isConnected() const {
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
    if (connectedFlag2) {
        if (engagerProgram != nullptr) {
            delete engagerProgram;
        }
        engagerProgram = new EngagerProgram();
        engagerProgram->setCurrentProgram(GCodeHelper::createCenterSequence());
        sendCommandFromSequence(engagerProgram->pullCommand());
    }
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
            clearLog();
            addLog("Connected to engager on port: " + serialPort.portName());
            emit connectedToEngager();
            GCodeHelper::reset();
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
    if (engagerProgram && engagerProgram->hasCommand()) {
        QDateTime current = QDateTime::currentDateTime();
        qint64 tm = current.toMSecsSinceEpoch() - sequenceTimeStart.toMSecsSinceEpoch();
        current = current.fromMSecsSinceEpoch(tm);
        double forOneCommmand = static_cast<double>(tm) / engagerProgram->passedCommandCount();
        passedTimeLabel->setText(current.toString("hh:mm:ss"));
        tm = static_cast<qint64>(forOneCommmand * engagerProgram->leftCommandCount());
        current = current.fromMSecsSinceEpoch(tm);
        leftTimeLable->setText(current.toString("hh:mm:ss"));
    }
}

void EngagerController::sendCommandFromSequence(const QString &command) {
    addLog("Sending command to com port: " + command);
    serialPort.write(command.toLatin1());
    commandSend = true;
}

void EngagerController::sendNextCommand() {
    if (engagerProgram && engagerProgram->hasCommand()) {
        sendCommandFromSequence(engagerProgram->pullCommand());
        if (engageProgress) {
            engageProgress->setValue(static_cast<int>(engagerProgram->getCurrentProgress()));
        }
        if (!engagerProgram->hasCommand()) {
            delete engagerProgram;
            engagerProgram = nullptr;
        }
    }
}

void EngagerController::addLog(const QString &logLine) {
    if (textLog) {
        QString logText = logLine;
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
            logText = logText.replace(0x0A, "\\x0A");
            logText = logText.replace(0x0D, "\\x0D");
            textLog->append(time.toString("[hh:mm:ss,zzz]: ") + logLine);
        }
        textLog->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    }
}

void EngagerController::clearLog() {
    if (textLog) {
        textLog->clear();
    }
}
