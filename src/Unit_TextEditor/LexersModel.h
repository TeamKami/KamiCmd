#ifndef LEXERSMODEL_H
#define LEXERSMODEL_H

#include <QAbstractItemModel>
#include <QTreeView>
#include <QSettings>
class ILexer;

enum LexersModelLeafType{
	Font,
	Color,
	//Wildcard,
	Properties,
	Lexer,
	Style
};

class LexersModelCacheItem : public QObject
{
	Q_OBJECT
public:
	LexersModelCacheItem(ILexer * lexer = 0, int style = 0, QObject * parent = 0);
	void save(QSettings & set) const;
	void reset();
	QFont & font();
	QColor & color();
private:
	ILexer * lexer_;
	int style_;
	QFont font_;
	QColor color_;
};

class LexersModelNode : public QObject
{
	Q_OBJECT
public:
	LexersModelNode(int lexer, int style, LexersModelLeafType type, LexersModelCacheItem * cache = 0, QObject * parent = 0);
	int lexer() const;
	int style() const;
	LexersModelLeafType type() const;
	QFont & font() const;
	QColor & color() const;

private:
	int lexer_, style_;
	LexersModelLeafType type_;
	LexersModelCacheItem * cache_;
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
	Qt::ItemFlags flags(const QModelIndex & index) const;

public slots:
	void save();
private:
	mutable QMap<QString, LexersModelNode*> nodes;
	mutable QMap<QString, LexersModelCacheItem*> cache_;
	LexersModelNode * createNode(int lexer, int style, LexersModelLeafType type) const;
};

#endif //LEXERSMODEL_H
