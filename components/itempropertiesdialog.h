#ifndef ITEMPROPERTIESDIALOG_H
#define ITEMPROPERTIESDIALOG_H

#include "itempropertiesmodel.h"

#include <QDialog>
#include <QGraphicsItem>

namespace Ui {
class ItemPropertiesDialog;
}

class ItemPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ItemPropertiesDialog(QWidget *parent = nullptr);
    ~ItemPropertiesDialog();

    void setItem(QGraphicsItem *item);
    void updateItem();

private:
    Ui::ItemPropertiesDialog *ui;

    ItemPropertiesModel *itemPropertyModel;
    QGraphicsItem *mainItem;
    QMap<QString, QVariant> itemProperties;
    QStringList itemPropertyNames;
    QList<int> itemPropertyIndexes;
    QList<bool> itemPropertyMultiply;
};

#endif // ITEMPROPERTIESDIALOG_H
