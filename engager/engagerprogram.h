#ifndef ENGAGERPROGRAM_H
#define ENGAGERPROGRAM_H

#include <QObject>

class EngagerProgram
{
public:
    EngagerProgram();
    EngagerProgram(const QStringList &engagerProgram);
    QStringList currentProgram() const;
    void setCurrentProgram(const QStringList &engagerProgram);
    void loadProgram(const QString &filename);
    void saveProgram(const QString &filename);
    void newProgram();

    void addCommand(const QString &command);

    QString pullCommand();
    bool hasCommand() const;

    int totalCommandCount() const;
    int passedCommandCount() const;
    int leftCommandCount() const;

    float getCurrentProgress() const;

private:
    QStringList engagerProgram;
    int totalCount;
    int passedCount;
};

#endif // ENGAGERPROGRAM_H
