#include "engagercommand.h"

EngagerCommand::EngagerCommand() {
    delay = 0;
}

EngagerCommand::EngagerCommand(const QString &command, int delay, const QString &description) {
    this->command = command;
    this->delay = delay;
    this->description = description;
    if (!this->command.endsWith('\0x0A')) {
        this->command += "\x0A";
    }
}

QString EngagerCommand::getCommand() const {
    return command;
}

void EngagerCommand::setCommand(const QString &command) {
    this->command = command;
    if (!this->command.endsWith('\0x0A')) {
        this->command += "\x0A";
    }
}

QString EngagerCommand::getDescription() const {
    return description;
}

void EngagerCommand::setDescription(const QString &description) {
    this->description = description;
}

int EngagerCommand::getDelay() const {
    return delay;
}

void EngagerCommand::setDelay(int value) {
    delay = value;
}
