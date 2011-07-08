#ifndef LEXERSMODEL_H
#define LEXERSMODEL_H

#include <QAbstractItemModel>
#include <QTreeView>

class LexersModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	LexersModel(QObject * parent = 0);
	QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex & index) const;
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const; 

};

#endif //LEXERSMODEL_H
