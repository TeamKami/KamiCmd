#include "FilesDelegate.h"
#include <QPainter>
#include <QtWidgets/QStyleOption>
#include <QtWidgets/QAbstractItemView>
#include "FilesDelegate_win.h"
#include "../IFileSystem.h"
#include "FileListModel.h"
#include <QtWidgets/QTreeView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QApplication>

// This whole file is a pack of hacks and crutches

#define VIEWPORT_MARGIN_LEFT 7/*13*/
#define VIEWPORT_MARGIN_RIGHT 5/*13*/

void FilesDelegate::QCommonStylePrivate_viewItemDrawText(QStyle *style, QPainter *p, const QStyleOptionViewItemV4 *opt, const QRect &rect, bool isSplitExtension /*= false*/) const
{
	const QWidget *widget = opt->widget;
	const int textMargin = style->pixelMetric(QStyle::PM_FocusFrameHMargin, 0, widget) + 1;
	QRect textRect = rect.adjusted(textMargin, 0, -textMargin, 1); // remove width padding. Adjusted height by 1 is somehow what we need exactly
	int textExtent = p->fontMetrics().width(opt->text);

	// FIXME: colorize extension if found also

	// Special look for files with '.' in names - name is left aligned, extension is right-aligned
	int dotPos = opt->text.lastIndexOf('.');
	QString name = opt->text, ext;
	isSplitExtension = isSplitExtension && dotPos > 0 && dotPos < opt->text.size() - 1 && textExtent < textRect.width();
	if (isSplitExtension)
	{
		name = opt->text.left(dotPos);
		ext = opt->text.right(opt->text.size() - dotPos - 1);
	}

	// Draw QuickSearched text in different color
	if (!quickSearch.isEmpty())
	{
		int matchPos = name.indexOf(quickSearch, 0, Qt::CaseInsensitive);
		if (matchPos != -1)
		{
			QString leftText = name.left(matchPos);
			QString foundText = name.mid(matchPos, quickSearch.size());
			QString rightText = name.right(name.size() - matchPos - quickSearch.size());

			// Drawing text before QuickSearched text
			p->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, leftText);
			int foundTextExtent = p->fontMetrics().width(foundText),
				leftPlusFoundTextExtent = p->fontMetrics().width(leftText + foundText);
			textRect.adjust(leftPlusFoundTextExtent - foundTextExtent, 0, 0, 0);

			// Drawing QuickSearched text
			QBrush brush = p->background();
			QPen pen = p->pen();
#if defined(Q_OS_WIN32)
			if (!FilesDelegate_win_useVista(style) && opt->state & QStyle::State_Selected)
				p->setPen(QPen(Qt::black));
#endif // Q_OS_WIN32
			Qt::BGMode bgMode = p->backgroundMode();
			p->setBackgroundMode(Qt::OpaqueMode);
			p->setBackground(QBrush(QColor(255, 255, 64)));
			p->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, foundText);
			p->setBackground(brush);
			p->setBackgroundMode(bgMode);
			p->setPen(pen);
			int nameTextExtent = p->fontMetrics().width(name),
				rightTextExtent = p->fontMetrics().width(rightText);
			textRect.adjust(nameTextExtent - rightTextExtent - (leftPlusFoundTextExtent - foundTextExtent), 0, 0, 0);

			// Drawing text after QuickSearched text
			p->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, rightText);
			if (isSplitExtension)
				p->drawText(textRect, Qt::AlignVCenter | Qt::AlignRight, ext);
			return;
		}
	}
	
	if (isSplitExtension)
	{
		p->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, name);
		p->drawText(textRect, Qt::AlignVCenter | Qt::AlignRight, ext);
	}
	else
		p->drawText(textRect, opt->displayAlignment, opt->text);
}


// Based on QCommonStyle::drawControl case CE_ItemViewItem
void FilesDelegate::paint( QPainter *p, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
// 	QStyledItemDelegate::paint(p, option, index);
// 	return;

	Q_ASSERT(index.isValid());

	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);
	const QWidget *widget = opt.widget;
	const QTreeView *view = qobject_cast<const QTreeView *>(widget);
        QStyle *style = widget ? widget->style() : qApp->style();
// 	style->drawControl(QStyle::CE_ItemViewItem, &opt, p, widget);
// 	return;

#if defined(Q_OS_WIN32)
	// Based on QWindowsVistaStyle::drawControl case CE_ItemViewItem
	if (FilesDelegate_win_useVista(style))
	{
		QPalette palette = opt.palette;
		palette.setColor(QPalette::All, QPalette::HighlightedText, palette.color(QPalette::Active, QPalette::Text));
		// Note that setting a saturated color here results in ugly XOR colors in the focus rect
		palette.setColor(QPalette::All, QPalette::Highlight, palette.base().color().darker(108));

		opt.palette = palette;
		// We hide the focusrect in singleselection as it is not required
		if ((view->selectionMode() == QAbstractItemView::SingleSelection) && !(opt.state & QStyle::State_KeyboardFocusChange))
			opt.state &= ~QStyle::State_HasFocus;
	}
#endif

	// Custom padding for first and last columns; Also fixing viewItemPosition for swapped columns
	if (!view->header()->logicalIndex(index.column()))
	{
		opt.rect.setLeft(opt.rect.left() + VIEWPORT_MARGIN_LEFT);
		opt.viewItemPosition = QStyleOptionViewItemV4::Beginning;
	}
	if (view->header()->logicalIndex(index.column()) == index.model()->columnCount() - 1)
	{
		opt.rect.setRight(opt.rect.right() - VIEWPORT_MARGIN_RIGHT);
		opt.viewItemPosition = QStyleOptionViewItemV4::End;
	}


	p->save();
	p->setClipRect(opt.rect);

	QRect checkRect = style->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &opt, widget);
	QRect iconRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &opt, widget);
	QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt, widget);

	// Initially selected elements have State_Selected state, but it's not what we want, so we remove it and set background color
	if (opt.state & QStyle::State_Selected)
	{
		opt.state &= ~(QStyle::State_Selected);
		opt.backgroundBrush = QBrush(QColor(255, 215, 188)); // Soft orange-pink color for selection
	}

	// State_Selected state is used for displaying cursor row only
	if (currentRow == index.data(FileListModel::IndexRowRole).toInt())
		opt.state |= QStyle::State_Selected;

	// draw the background
	style->proxy()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, p, widget);

	// draw the check mark
	if (opt.features & QStyleOptionViewItemV2::HasCheckIndicator)
	{
		QStyleOptionViewItemV4 option(opt);
		option.rect = checkRect;
		option.state = option.state & ~QStyle::State_HasFocus;

		switch (opt.checkState)
		{
		case Qt::Unchecked:
			option.state |= QStyle::State_Off;
			break;
		case Qt::PartiallyChecked:
			option.state |= QStyle::State_NoChange;
			break;
		case Qt::Checked:
			option.state |= QStyle::State_On;
			break;
		}
		style->proxy()->drawPrimitive(QStyle::PE_IndicatorViewItemCheck, &option, p, widget);
	}

	// draw the icon
	QIcon::Mode mode = QIcon::Normal;
	if (!(opt.state & QStyle::State_Enabled))
		mode = QIcon::Disabled;
	else if (opt.state & QStyle::State_Selected)
		mode = QIcon::Selected;
	QIcon::State state = opt.state & QStyle::State_Open ? QIcon::On : QIcon::Off;
	opt.icon.paint(p, iconRect, opt.decorationAlignment, mode, state);

	// draw the text
	if (!opt.text.isEmpty())
	{
		QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
		if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
			cg = QPalette::Inactive;

		if (opt.state & QStyle::State_Selected)
			p->setPen(opt.palette.color(cg, QPalette::HighlightedText));
		else
			p->setPen(opt.palette.color(cg, QPalette::Text));

		if (opt.state & QStyle::State_Editing)
		{
			p->setPen(opt.palette.color(cg, QPalette::Text));
			p->drawRect(textRect.adjusted(0, 0, -1, -1));
		}

		QCommonStylePrivate_viewItemDrawText(style, p, &opt, textRect, !(index.data(FileListModel::AttributesRole).toUInt() & FileInfo::Directory));
	}

	// draw the focus rect
// 	if (opt.state & QStyle::State_HasFocus)
// 	{
// 		QStyleOptionFocusRect o;
// 		o.QStyleOption::operator=(opt);
// 		o.rect = style->subElementRect(QStyle::SE_ItemViewItemFocusRect, &opt, widget);
// 		o.state |= QStyle::State_KeyboardFocusChange;
// 		o.state |= QStyle::State_Item;
// 		QPalette::ColorGroup cg = (opt.state & QStyle::State_Enabled)	? QPalette::Normal : QPalette::Disabled;
// 		o.backgroundColor = opt.palette.color(cg, (opt.state & QStyle::State_Selected) ? QPalette::Highlight : QPalette::Window);
// 		style->proxy()->drawPrimitive(QStyle::PE_FrameFocusRect, &o, p, widget);
// 	}

	p->restore();
}

void FilesDelegate::PaletteChanged()
{
#if defined(Q_OS_WIN32) || defined(Q_OS_WINCE)
	FilesDelegate_win_PaletteChanged();
#endif
}

FilesDelegate::FilesDelegate( QObject *parent /*= 0*/ )
	: QStyledItemDelegate(parent)
{
	PaletteChanged();
}

void FilesDelegate::QuickSearchChanged( QString search )
{
	quickSearch = search;
}

void FilesDelegate::CurrentRowChanged( int newCurrentRow )
{
	currentRow = newCurrentRow;
}

QSize FilesDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
#if defined(Q_OS_WIN32)
	size.setHeight(21);
#endif
	return size;
}
