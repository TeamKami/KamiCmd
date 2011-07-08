#ifndef LEXERSMODEL_H
#define LEXERSMODEL_H

#include <QAbstractItemModel>
#include <QTreeView>

enum LexersModelLeafType{
	Font,
	Color,
	//Wildcard,
	Properties,
	Lexer,
	Style
};

class LexersModelNode : public QObject
{
	Q_OBJECT
public:
	LexersModelNode(int lexer, int style, LexersModelLeafType type, QObject * parent = 0);
	int lexer() const;
	int style() const;
	LexersModelLeafType type() const;

private:
	int lexer_, style_;
	LexersModelLeafType type_;
};

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
private:
	mutable QMap<QString, LexersModelNode*> nodes;
	LexersModelNode * createNode(int lexer, int style, LexersModelLeafType type) const;
};

#endif //LEXERSMODEL_H
