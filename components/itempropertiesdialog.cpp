#include "itempropertiesdialog.h"
#include "ui_itempropertiesdialog.h"
#include "graphicitems/maingraphicsitem.h"

ItemPropertiesDialog::ItemPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemPropertiesDialog) {
    ui->setupUi(this);
    itemPropertyModel = new ItemPropertiesModel(nullptr);
    ui->propertiesTable->setModel(itemPropertyModel);
    mainItem = nullptr;
    itemPropertyNames << tr("Scale") << tr("Position X, mm") << tr("Position Y, mm");
    itemPropertyIndexes << SCALE << POSITION_X << POSITION_Y;
    itemPropertyMultiply << false << true << true;
}

ItemPropertiesDialog::~ItemPropertiesDialog() {
    delete ui;
}

void ItemPropertiesDialog::setItem(QGraphicsItem *item) {
    mainItem = item;
    qreal multiply = 0.1 / mainItem->data(MAIN_SCALE_FACTOR).toDouble();
    for (int i = 0; i < itemPropertyIndexes.size(); i++) {
        if (itemPropertyMultiply.at(i)) {
            itemProperties.insert(itemPropertyNames.at(i), item->data(itemPropertyIndexes.at(i)).toDouble() * multiply);
        } else {
            itemProperties.insert(itemPropertyNames.at(i), item->data(itemPropertyIndexes.at(i)));
        }
    }
    itemPropertyModel->setValueMap(itemProperties);
}

void ItemPropertiesDialog::updateItem() {
    itemProperties = itemPropertyModel->valueMap();
    qreal multiply = mainItem->data(MAIN_SCALE_FACTOR).toDouble() * 10;
    for (int i = 0; i < itemPropertyIndexes.size(); i++) {
        if (itemPropertyMultiply.at(i)) {
            mainItem->setData(itemPropertyIndexes.at(i), itemProperties[itemPropertyNames.at(i)].toDouble() * multiply);
        } else {
            mainItem->setData(itemPropertyIndexes.at(i), itemProperties[itemPropertyNames.at(i)]);
        }
    }

    qreal mainScaleFactor = mainItem->data(MAIN_SCALE_FACTOR).toDouble();
    qreal mainScale = mainItem->data(MAIN_SCALE).toDouble();
    qreal scale = mainItem->data(SCALE).toDouble();
    qreal x = mainItem->data(POSITION_X).toDouble() * mainScale;
    qreal y = mainItem->data(POSITION_Y).toDouble() * mainScale;

    mainItem->setScale(mainScaleFactor * mainScale * scale);
    mainItem->setPos(x, y);
}
