#include "Unit_TextEditor/LexersModel.h"
#include "Unit_TextEditor/ILexerPlugin.h"
#include <Qsci/qscilexer.h>

LexersModelNode::LexersModelNode(int lexer, int style, LexersModelLeafType type, LexersModelCacheItem * cache, QObject * parent) :
	QObject(parent), lexer_(lexer), style_(style), type_(type), cache_(cache)
{
}

int LexersModelNode::lexer() const
{
	return lexer_;
}

int LexersModelNode::style() const
{
	return style_;
}

LexersModelLeafType LexersModelNode::type() const
{
	return type_;
}

QFont & LexersModelNode::font() const
{
	return cache_->font();
}

QColor & LexersModelNode::color() const
{
	return cache_->color();
}

LexersModelCacheItem::LexersModelCacheItem(ILexer * lexer, int style, QObject * parent) :
	QObject(parent), lexer_(lexer), style_(style)
{
	reset();
}

void LexersModelCacheItem::save(QSettings & set) const
{
	if (font_ != lexer_->getLexer()->font(style_) || color_ != lexer_->getLexer()->color(style_))
	{
		lexer_->getLexer()->setFont(font_, style_);
		lexer_->getLexer()->setColor(color_, style_);
		lexer_->getLexer()->writeSettings(set);
	}
}

void LexersModelCacheItem::reset()
{
	font_ = lexer_->getLexer()->font(style_);
	color_ = lexer_->getLexer()->color(style_);
}

QFont & LexersModelCacheItem::font()
{
	return font_;
}

QColor & LexersModelCacheItem::color()
{
	return color_;
}

LexersModelNode * LexersModel::createNode(int lexer, int style, LexersModelLeafType type) const
{
	QString key = QString() + lexer + " - " + style + " - " + type;
	if (!nodes[key])
	{
		LexersModelCacheItem * ci = cache_[QString() + lexer + " - " + style];
		
		if (!ci && (type == Style || type < Properties))
		{
			ci = new LexersModelCacheItem(lexers()[lexer], style, const_cast<LexersModel*>(this));
			cache_[QString() + lexer + " - " + style] = ci;
		}

		nodes[key] = new LexersModelNode(lexer, style, type, ci, const_cast<LexersModel*>(this));
	}

	return nodes[key];
}

LexersModel::LexersModel(QObject * parent) :
	QAbstractItemModel(parent)
{
}

QModelIndex LexersModel::index(int row, int column, const QModelIndex & parent) const
{
	if (!parent.isValid())
	{
		return createIndex(row, column, createNode(row, -1, Lexer));
	}
	else
	{
		LexersModelNode * node = static_cast<LexersModelNode*>(parent.internalPointer());

		if (node)
		{
			if (node->type() == Lexer)
			{
				return createIndex(row, column, createNode(node->lexer(), row, Style));
			}
			else if (node->type() == Style && row < Properties)
			{
				return createIndex(row, column, createNode(node->lexer(), node->style(), static_cast<LexersModelLeafType>(row)));
			}
		}
	}
	return QModelIndex();
}

QModelIndex LexersModel::parent(const QModelIndex & index) const
{
	if (index.isValid())
	{
		LexersModelNode * node = static_cast<LexersModelNode*>(index.internalPointer());
		
		if (node)
		{
			if (node->type() == Style)
			{
				return createIndex(node->lexer(), index.column(), createNode(node->lexer(), -1, Lexer));
			}
			else if (node->type() < Properties)
			{
				return createIndex(node->style(), index.column(), createNode(node->lexer(), node->style(), Style));
			}
		}
	}

	return QModelIndex();
}

int LexersModel::rowCount(const QModelIndex & parent) const
{
	if (!parent.isValid())
	{
		return lexers().size();
	}
	else
	{
		LexersModelNode * node = static_cast<LexersModelNode*>(parent.internalPointer());
		
		if (node)
		{
			if (node->type() == Style)
			{
				return Properties;
			}
			else if (node->type() == Lexer)
			{
				int count = 0;
				
				for (;lexers()[node->lexer()]->getLexer()->description(count) != ""; ++count);
				
				return count;
			}
		}
	}
	return 0;
}

int LexersModel::columnCount(const QModelIndex &) const
{
	return 2;
}

QVariant LexersModel::data(const QModelIndex & index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		LexersModelNode * node = static_cast<LexersModelNode*>(index.internalPointer());
		
		if (node)
		{
			switch (node->type())
			{
			case Lexer:
				if (index.column() == 0)
				{
					return QVariant::fromValue(QString(lexers()[node->lexer()]->getLexer()->language()));
				}
				else
				{
					return QVariant::fromValue(QString("(...)"));
				}
			case Style:
				if (index.column() == 0)
				{
					return QVariant::fromValue(QString(lexers()[node->lexer()]->getLexer()->description(node->style())));
				}
				break;
			case Font:
				if (index.column() == 0)
				{
					return QVariant::fromValue(QString("Font"));
				}
				break;
			case Color:
				if (index.column() == 0)
				{
					return QVariant::fromValue(QString("Color"));
				}
				break;
			/*case Wildcard:
				if (index.column() == 0)
				{
					return QVariant::fromValue(QString("Wildcard"));
				}
				else
				{
					QString wc;
					QString delim = "";

					foreach(QString const & s, lexers()[node->lexer()]->getMask())
					{
						wc += delim + s;
						delim = ",";
					}

					return QVariant::fromValue(wc);
				}*/
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

Qt::ItemFlags LexersModel::flags(const QModelIndex & index) const
{
	if (index.isValid())
	{
		LexersModelNode * node = static_cast<LexersModelNode*>(index.internalPointer());
		if (node)
		{
			if (node->type() < Properties)
			{
				return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
			}
		}
	}

	return QAbstractItemModel::flags(index);
}

void LexersModel::save()
{
	QSettings set;
	set.setIniCodec("UTF-8");
	set.beginGroup("ILexerPlugin");

	foreach(LexersModelCacheItem* it, cache_)
	{
		if (it)
		{
			it->save(set);
		}
	}

	set.endGroup();
}
