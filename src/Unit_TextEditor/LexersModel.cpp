#include "Unit_TextEditor/LexersModel.h"
#include "Unit_TextEditor/ILexerPlugin.h"
#include <Qsci/qscilexer.h>

enum {
	Font,
	Color,
	Wildcard,
	Properties
};

LexersModel::LexersModel(QObject * parent) :
	QAbstractItemModel(parent)
{
}

QModelIndex LexersModel::index(int row, int column, const QModelIndex & parent) const
{
	if (!parent.isValid())
	{
		return createIndex(row, column, 0);
	}
	else if(parent.internalId() == 0)
	{
		return createIndex(row, column, parent.row() + 1);
	}

	return QModelIndex();
}

QModelIndex LexersModel::parent(const QModelIndex & index) const
{
	if (index.isValid() && index.internalId() != 0)
	{
		return createIndex(index.internalId() - 1, 0, 0);
	}

	return QModelIndex();
}

int LexersModel::rowCount(const QModelIndex & parent) const
{
	if (!parent.isValid())
	{
		return lexers().size();
	}
	else if (parent.internalId() == 0)
	{
		return Properties;
	}
	return 0;
}

int LexersModel::columnCount(const QModelIndex & parent) const
{
	return 1;
}

QVariant LexersModel::data(const QModelIndex & index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (index.internalId() == 0)
		{
			return QVariant::fromValue(QString(lexers()[index.row()]->getLexer()->language()));
		}
		else
		{
			switch (index.row())
			{
			case Font:
				return QVariant::fromValue(QString("Font"));
			case Color:
				return QVariant::fromValue(QString("Color"));
			case Wildcard:
				return QVariant::fromValue(QString("Wildcard"));
			default:
				return QVariant::fromValue(QString("(unknown)"));
			}
		}
	}
	return QVariant();
}

QVariant LexersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		switch (section)
		{
		case 0:
			return QVariant::fromValue(QString("Property"));
		case 1:
			return QVariant::fromValue(QString("Value"));
		}
	}

	return QAbstractItemModel::headerData(section, orientation, role);
}