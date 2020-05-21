#ifndef ITEMPROPERTIESDIALOG_H
#define ITEMPROPERTIESDIALOG_H

#include "itempropertiesmodel.h"

#include <QDialog>

namespace Ui {
class ItemPropertiesDialog;
}

class ItemPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ItemPropertiesDialog(QWidget *parent = nullptr);
    ~ItemPropertiesDialog();

    void setItemProperties(QMap<QString, QVariant> values);
    QMap<QString, QVariant> itemProperties();

private:
    Ui::ItemPropertiesDialog *ui;

    ItemPropertiesModel *itemPropertyModel;
};

#endif // ITEMPROPERTIESDIALOG_H
