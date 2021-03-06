#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>
#include <QTextEdit>

namespace Ui {
class LogDialog;
}

class LogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogDialog(QWidget *parent = nullptr);
    ~LogDialog();
    QTextEdit *logTextEdit();

private:
    Ui::LogDialog *ui;
};

#endif // LOGDIALOG_H
