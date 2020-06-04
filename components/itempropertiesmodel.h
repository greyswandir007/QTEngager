#ifndef ITEMPROPERTIESMODEL_H
#define ITEMPROPERTIESMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

class ItemPropertiesModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ItemPropertiesModel(QObject *parent);

    void setValueMap(QMap<QString, QVariant> values);
    QMap<QString, QVariant> valueMap();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QStringList headers;
    QMap<QString, QVariant> mainValueMap;
};

#endif // ITEMPROPERTIESMODEL_H
