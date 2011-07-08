#include "Unit_TextEditor/LexersDelegate.h"
#include "Unit_TextEditor/LexersModel.h"
#include "Unit_TextEditor/ILexerPlugin.h"
#include <Qsci/qscilexer.h>
#include <QPainter>

LexersDelegate::LexersDelegate(QObject * parent) :
	QStyledItemDelegate(parent)
{
}

void LexersDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	LexersModelNode * node = static_cast<LexersModelNode*>(index.internalPointer());

	if (index.column() == 1)
	{
		switch (node->type())
		{
		case Font:
			painter->setPen(QPen());
			painter->setFont(lexers()[node->lexer()]->getLexer()->font(node->style()));
			painter->drawText(option.rect, lexers()[node->lexer()]->getLexer()->font(node->style()).family());
			break;
		case Style:
			painter->setPen(lexers()[node->lexer()]->getLexer()->color(node->style()));
			painter->setFont(lexers()[node->lexer()]->getLexer()->font(node->style()));
			painter->drawText(option.rect, lexers()[node->lexer()]->getLexer()->font(node->style()).family());
			break;
		case Color:
			painter->setPen(lexers()[node->lexer()]->getLexer()->color(node->style()));
			painter->drawText(option.rect, lexers()[node->lexer()]->getLexer()->color(node->style()).name());
			break;
		default:
			QStyledItemDelegate::paint(painter, option, index);
		}
	}
	else
	{
		QStyledItemDelegate::paint(painter, option, index);
	}
}

QSize LexersDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	return QStyledItemDelegate::sizeHint(option, index);
}