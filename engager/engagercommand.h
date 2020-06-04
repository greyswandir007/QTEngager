#ifndef ENGAGERCOMMAND_H
#define ENGAGERCOMMAND_H

#include <QObject>

class EngagerCommand
{
public:
    EngagerCommand();
    EngagerCommand(const QString &command, int delay = 0, const QString &description = QString());
    QString getCommand() const;
    void setCommand(const QString &command);
    QString getDescription() const;
    void setDescription(const QString &description);
    int getDelay() const;
    void setDelay(int value);

private:
    QString command; // Full command send to engager like G00 ... and so on
    QString description; // Meaningfull description for command like move, turn on/off laser, curve, arc and so on
    int delay; // delay in ms
};

#endif // ENGAGERCOMMAND_H
