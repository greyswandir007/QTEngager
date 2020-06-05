#include "engagercontroller.h"
#include <QDebug>

EngagerController::EngagerController(ICommandCreator *creator) {
    this->creator = creator;
    connect(&serialPort, &QSerialPort::readyRead, this, &EngagerController::on_serialPortRead);
    connect(&mainTimer, &QTimer::timeout, this, &EngagerController::on_timerEvent);
    updateComPortList();
    mainTimer.start(normalTimerPeriod);
    sequenceTimeStart = QDateTime::currentDateTime();
}

void EngagerController::sendCommand(const QString &command) {
    if ((connectedFlag2 || testMode) && !commandSend && !engagerProgram) {
        sendCommandFromSequence(EngagerCommand(command));
    }
}

void EngagerController::sendCommand(const EngagerCommand &command) {
    if ((connectedFlag2 || testMode) && !commandSend && !engagerProgram) {
        sendCommandFromSequence(command);
    }
}

void EngagerController::updateComPortList() {
    comPortList.clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        comPortList.append(info.portName());
    }
    comPortList.append("Test");
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
        if (index == comPortList.count() - 1) {
            testMode = true;
            mainTimer.stop();
            mainTimer.start(shortTimerPeriod);
            addLog("Test mode");
        } else {
            serialPort.close();
            serialPort.setPortName(comPortList.at(index));
            serialPort.setBaudRate(115200);
            if (serialPort.open(QIODevice::ReadWrite)) {
                connectedFlag = true;
                connectingFlag = true;
                commandSend = false;
                testMode = false;
            }
        }
    }
}

void EngagerController::engagerDisconnect() {
    if (!testMode) {
        serialPort.close();
    } else {
        mainTimer.stop();
        mainTimer.start(normalTimerPeriod);
    }
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
    testMode = false;
}

void EngagerController::runEngagerProgram(EngagerProgram *program) {
    if (connectedFlag2 || testMode) {
        if (engagerProgram != nullptr) {
            delete engagerProgram;
        }
        engagerProgram = program;
        sequenceTimeStart = QDateTime::currentDateTime();
        if (engagerProgram->hasCommand()) {
            sendCommandFromSequence(engagerProgram->pullCommand());
        }
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
    leftTimeLabel = leftTime;
}

void EngagerController::clearSequence() {
    if (connectedFlag2 || testMode) {
        if (engagerProgram != nullptr) {
            delete engagerProgram;
        }
        engagerProgram = new EngagerProgram();
        engagerProgram->setCurrentProgram(creator->centerQueue());
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
            creator->reset();
            sendCommand("$21 P-2\x0A");
        }
    } else if (received.endsWith("ok\r\n")) {
        commandSend = false;
        sendNextCommand();
    }
}

void EngagerController::on_timerEvent() {
    if (testMode) {
        sendNextCommand();
    }
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
        double forOneCommmand = static_cast<double>(tm) / engagerProgram->passedCommandCount();
        passedTimeLabel->setText(timeFromEpoch(tm, true));
        tm = static_cast<qint64>(forOneCommmand * engagerProgram->leftCommandCount());
        leftTimeLabel->setText(timeFromEpoch(tm, false));
    }
}

void EngagerController::sendCommandFromSequence(const EngagerCommand &command) {
    addLog("Sending command to com port: " + command.getCommand());
    if (!testMode) {
        serialPort.write(command.getCommand().toLatin1());
        commandSend = true;
    }
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
            textLog->append(time.toString("[hh:mm:ss,zzz]: ") + logText);
        }
        textLog->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    }
}

void EngagerController::clearLog() {
    if (textLog) {
        textLog->clear();
    }
}

QString EngagerController::timeFromEpoch(qint64 time, bool milliseconds) {
    QString str = milliseconds ? "%1:%2:%3.%4" : "%1:%2:%3";
    str = str.arg(time / 3600000, 2, 10, QChar('0')).arg((time / 60000) % 60, 2,  10, QChar('0'))
            .arg((time / 1000) % 60, 2,  10, QChar('0'));
    return milliseconds ? str.arg(time % 1000,  2,  10, QChar('0')) : str;
}
