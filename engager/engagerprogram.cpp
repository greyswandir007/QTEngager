#include "engagerprogram.h"
#include <QFile>

EngagerProgram::EngagerProgram() {
    totalCount = 0;
    passedCount = 0;
}

EngagerProgram::EngagerProgram(const CommandQueue &engagerProgram) {
    this->engagerProgram = engagerProgram;
    totalCount = engagerProgram.count();
    passedCount = 0;
}

CommandQueue EngagerProgram::currentProgram() const {
    return engagerProgram;
}

void EngagerProgram::setCurrentProgram(const CommandQueue &program) {
    engagerProgram = program;
    totalCount = engagerProgram.count();
    passedCount = 0;
}

void EngagerProgram::loadProgram(const QString &filename) {
    QFile file(filename);
    engagerProgram.clear();
    if (file.open(QIODevice::ReadOnly)) {
        QString str = str.fromLatin1(file.readAll());
        QStringList program = str.split("\x0A");
        for (int i = 0; i < engagerProgram.size(); i++) {
            if (program [i].endsWith("\x0D")) {
                program [i].truncate(engagerProgram[i].getCommand().length()-1);
            }
            program[i] += "\x0A";
            engagerProgram.append(EngagerCommand(program[i]));
        }
        file.close();
    }
}

void EngagerProgram::saveProgram(const QString &filename) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QString program = "";
        for(EngagerCommand command : engagerProgram) {
            program += command.getCommand();
        }
        file.write(program.toLatin1());
        file.close();
    }
}

void EngagerProgram::newProgram() {
    engagerProgram.clear();
    totalCount = 0;
    passedCount = 0;
}

void EngagerProgram::addCommand(const CommandQueue &command) {
    engagerProgram.append(command);
    totalCount++;
}

EngagerCommand EngagerProgram::pullCommand() {
    EngagerCommand command;
    if (!engagerProgram.isEmpty()) {
        command = engagerProgram.at(0);
        engagerProgram.removeAt(0);
        passedCount++;
    }
    return command;
}

bool EngagerProgram::hasCommand() const {
    return !engagerProgram.isEmpty();
}

int EngagerProgram::totalCommandCount() const {
    return totalCount;
}

int EngagerProgram::passedCommandCount() const {
    return passedCount;
}

int EngagerProgram::leftCommandCount() const {
    return engagerProgram.count();
}

float EngagerProgram::getCurrentProgress() const {
    return engagerProgram.isEmpty() ? 0 : passedCount * 100.0f / static_cast<float>(totalCount);
}
