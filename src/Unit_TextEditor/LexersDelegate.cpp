#include "Unit_TextEditor/LexersDelegate.h"
#include "Unit_TextEditor/LexersModel.h"
#include "Unit_TextEditor/ILexerPlugin.h"
#include <Qsci/qscilexer.h>
#include <QPainter>
#include <QFontComboBox>
#include <QMessageBox>
#include <QColorDialog>

LexersDelegate::LexersDelegate(QObject * parent) :
	QStyledItemDelegate(parent), cdlg(0), cache(0)
{
}

LexersDelegate::~LexersDelegate()
{
	if (cdlg)
	{
		cdlg->close();
		delete cdlg;
	}
}

void LexersDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	LexersModelNode * node = static_cast<LexersModelNode*>(index.internalPointer());

	if (index.column() == 1)
	{
		QPen pen = painter->pen();
		QFont font = painter->font();

		switch (node->type())
		{
		case Font:		
			painter->setFont(node->font());
			painter->drawText(option.rect, node->font().family());
			break;
		case Style:
			painter->setPen(node->color());
			painter->setFont(node->font());
			painter->drawText(option.rect, node->font().family());
			break;
		case Color:
			painter->setPen(node->color());
			painter->drawText(option.rect, node->color().name());
			break;
		default:
			QStyledItemDelegate::paint(painter, option, index);
		}

		painter->setPen(pen);
		painter->setFont(font);
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

QWidget * LexersDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == 1)
	{
		LexersModelNode * node = static_cast<LexersModelNode*>(index.internalPointer());
		switch (node->type())
		{
		case Font:
			return new QFontComboBox(parent);
                default:
                    break;
		}
	}

	return QStyledItemDelegate::createEditor(parent, option, index);
}

void LexersDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	if (index.column() == 1)
	{
		LexersModelNode * node = static_cast<LexersModelNode*>(index.internalPointer());
		switch (node->type())
		{
		case Font:
                {
			QFontComboBox * fb = dynamic_cast<QFontComboBox*>(editor);
			if (fb)
			{
				fb->setCurrentFont(node->font());
			}
			break;
                }
                default:
                    break;
		}
	}
}

void LexersDelegate::setModelData(QWidget *editor, QAbstractItemModel *, const QModelIndex &index) const
{
	if (index.column() == 1)
	{
		LexersModelNode * node = static_cast<LexersModelNode*>(index.internalPointer());
		switch (node->type())
		{
		case Font:
                {
			QFontComboBox * fb = dynamic_cast<QFontComboBox*>(editor);
			if (fb)
			{
				node->font() = fb->currentFont();
			}
			break;
                }
                default:
                    break;
		}
	}
}

void LexersDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &)const
{
	editor->setGeometry(option.rect);
}

bool LexersDelegate::editorEvent(QEvent *, QAbstractItemModel *, const QStyleOptionViewItem &, const QModelIndex & index)
{
	if (cache)
		return false;

	if (index.column() == 1)
	{
		LexersModelNode * node = static_cast<LexersModelNode*>(index.internalPointer());

		if (node->type() == Color)
		{
			cache = node;

			if (!cdlg)
			{
				cdlg = new QColorDialog(node->color(), 0);
			}
			else
			{
				cdlg->setCurrentColor(node->color());
			}

			connect(cdlg, SIGNAL(colorSelected(QColor const &)), this, SLOT(selectColor(QColor const &)));
			cdlg->show();
			return true;
		}
	}

	return false;
}

void LexersDelegate::selectColor(QColor const & color)
{
	if (cache)
	{
		cache->color() = color;
		cache = 0;
	}

}
