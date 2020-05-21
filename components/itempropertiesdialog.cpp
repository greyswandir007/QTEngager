#include "itempropertiesdialog.h"
#include "ui_itempropertiesdialog.h"

ItemPropertiesDialog::ItemPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemPropertiesDialog) {
    ui->setupUi(this);
    itemPropertyModel = new ItemPropertiesModel(nullptr);
    ui->propertiesTable->setModel(itemPropertyModel);
}

ItemPropertiesDialog::~ItemPropertiesDialog() {
    delete ui;
}

void ItemPropertiesDialog::setItemProperties(QMap<QString, QVariant> values) {
    itemPropertyModel->setValueMap(values);
}

QMap<QString, QVariant> ItemPropertiesDialog::itemProperties() {
    return itemPropertyModel->valueMap();
}
