#include "FilesDelegate.h"
#include <QPainter>
#include <QStyleOption>
#include <QApplication>
#include <QTextLayout>
#include <QAbstractItemView>
#include <QWindowsVistaStyle>

#include "../IFileSystem.h"
#include "FileListModel.h"
// This whole file is a pack of hacks and crutches

void FilesDelegate::QCommonStylePrivate_viewItemDrawText(QStyle *s, QPainter *p, const QStyleOptionViewItemV4 *option, const QRect &rect, bool isSplitExtension /*= false*/) const
{
	const QWidget *widget = option->widget;
	const int textMargin = s->pixelMetric(QStyle::PM_FocusFrameHMargin, 0, widget) + 1;

	// 1 in yp2 is a crutch and it works as it should somehow
	QRect textRect = rect.adjusted(textMargin, 0, -textMargin, 1); // remove width padding

	int textExtent = p->fontMetrics().width(option->text);

 	//p->drawRect(textRect);

	// FIXME: colorize extension if found also

	// Special look for files with '.' in names - name is left aligned, extension is right-aligned
	int dotPos = option->text.lastIndexOf('.');
	QString name = option->text, ext;
	isSplitExtension = isSplitExtension && dotPos > 0 && dotPos < option->text.size() - 1 && textExtent < textRect.width();
	if (isSplitExtension)
	{
		name = option->text.left(dotPos);
		ext = option->text.right(option->text.size() - dotPos - 1);
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

			p->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, leftText);
			int foundTextExtent = p->fontMetrics().width(foundText),
				leftPlusFoundTextExtent = p->fontMetrics().width(leftText + foundText);
			textRect.adjust(leftPlusFoundTextExtent - foundTextExtent, 0, 0, 0);

			QBrush brush = p->background();
			//QPen pen = p->pen();
			//p->setPen(QPen(QColor(255, 128, 0)));
			Qt::BGMode bgMode = p->backgroundMode();
			p->setBackgroundMode(Qt::OpaqueMode);
			
			p->setBackground(QBrush(QColor(255, 255, 64)));
			p->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, foundText);
			p->setBackground(brush);
			p->setBackgroundMode(bgMode);
			//p->setPen(pen);
			int nameTextExtent = p->fontMetrics().width(name),
				rightTextExtent = p->fontMetrics().width(rightText);
			textRect.adjust(nameTextExtent - rightTextExtent - (leftPlusFoundTextExtent - foundTextExtent), 0, 0, 0);

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
		p->drawText(textRect, option->displayAlignment, option->text);
}

static const QWidget * QStyledItemDelegatePrivate_widget(const QStyleOptionViewItem &option)
{
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
		return v3->widget;
	return 0;
}

#if defined(Q_OS_WIN32) || defined(Q_OS_WINCE)

#include "windows.h"

class QSystemLibrary
{
public:
	explicit QSystemLibrary(const QString &libraryName)
	{
		m_libraryName = libraryName;
		m_handle = 0;
		m_didLoad = false;
	}

	explicit QSystemLibrary(const wchar_t *libraryName)
	{
		m_libraryName = QString::fromWCharArray(libraryName);
		m_handle = 0;
		m_didLoad = false;
	}

	bool load(bool onlySystemDirectory = true)
	{
		m_handle = load((const wchar_t *)m_libraryName.utf16(), onlySystemDirectory);
		m_didLoad = true;
		return (m_handle != 0);
	}

	bool isLoaded()
	{
		return (m_handle != 0);
	}

	void *resolve(const char *symbol)
	{
		if (!m_didLoad)
			load();
		if (!m_handle)
			return 0;
#ifdef Q_OS_WINCE
		return (void*)GetProcAddress(m_handle, (const wchar_t*)QString::fromLatin1(symbol).utf16());
#else
		return (void*)GetProcAddress(m_handle, symbol);
#endif
	}

	static void *resolve(const QString &libraryName, const char *symbol)
	{
		return QSystemLibrary(libraryName).resolve(symbol);
	}

	static Q_CORE_EXPORT HINSTANCE load(const wchar_t *lpFileName, bool onlySystemDirectory = true);
private:
	HINSTANCE m_handle;
	QString m_libraryName;
	bool m_didLoad;
};

typedef bool (WINAPI *PtrIsAppThemed)();
typedef bool (WINAPI *PtrIsThemeActive)();
static PtrIsAppThemed pIsAppThemed = 0;
static PtrIsThemeActive pIsThemeActive = 0;


bool QWindowsXPStylePrivate_resolveSymbols()
{
	static bool tried = false;
	if (!tried) {
		tried = true;
		QSystemLibrary themeLib(QLatin1String("uxtheme"));
		pIsAppThemed = (PtrIsAppThemed)themeLib.resolve("IsAppThemed");
		if (pIsAppThemed)
			pIsThemeActive = (PtrIsThemeActive)themeLib.resolve("IsThemeActive");
	}
	return pIsAppThemed != 0;
}

static bool use_xp;

bool QWindowsVistaStylePrivate_useVista()
{
	return (use_xp &&
		(QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA &&
		QSysInfo::WindowsVersion < QSysInfo::WV_NT_based));
}

#endif

// Based on QCommonStyle::drawControl case CE_ItemViewItem
void FilesDelegate::paint( QPainter *p, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
// 	QStyledItemDelegate::paint(p, option, index);
// 	return;

	Q_ASSERT(index.isValid());

	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	const QWidget *widget = QStyledItemDelegatePrivate_widget(option);
	QStyle *style = widget ? widget->style() : QApplication::style();
// 	style->drawControl(QStyle::CE_ItemViewItem, &opt, p, widget);
// 	return;


	if (const QStyleOptionViewItemV4 *vopt = &opt)
	{
// 		QString txt = opt.text;
// 		opt.text = "";
// 		style->drawControl(QStyle::CE_ItemViewItem, &opt, p, widget);
// 		opt.text = txt;
// 		QRect textRect2 = style->subElementRect(QStyle::SE_ItemViewItemText, vopt, widget);
// 
// 		if (!vopt->text.isEmpty())
// 		{
// 			QPalette::ColorGroup cg = vopt->state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
// 			if (cg == QPalette::Normal && !(vopt->state & QStyle::State_Active))
// 				cg = QPalette::Inactive;
// 
// 			if (vopt->state & QStyle::State_Selected)
// 				p->setPen(vopt->palette.color(cg, QPalette::HighlightedText));
// 			else
// 				p->setPen(vopt->palette.color(cg, QPalette::Text));
// 
// 			if (vopt->state & QStyle::State_Editing)
// 			{
// 				p->setPen(vopt->palette.color(cg, QPalette::Text));
// 				p->drawRect(textRect2.adjusted(0, 0, -1, -1));
// 			}
// 
// 			QCommonStylePrivate_viewItemDrawText(style, p, vopt, textRect2);
// 		}
// 
// 		return;

		// Based on QWindowsVistaStyle::drawControl case CE_ItemViewItem
#if defined(Q_OS_WIN32) || defined(Q_OS_WINCE)
		QStyleOptionViewItemV4 adjustedOption = *vopt;
		if (qobject_cast<QWindowsVistaStyle *>(style) && QWindowsVistaStylePrivate_useVista())
		{
			const QAbstractItemView *view = qobject_cast<const QAbstractItemView *>(widget);
		
			QPalette palette = vopt->palette;
			palette.setColor(QPalette::All, QPalette::HighlightedText, palette.color(QPalette::Active, QPalette::Text));
			// Note that setting a saturated color here results in ugly XOR colors in the focus rect
			palette.setColor(QPalette::All, QPalette::Highlight, palette.base().color().darker(108));

			adjustedOption.palette = palette;
			// We hide the focusrect in singleselection as it is not required
			if ((view->selectionMode() == QAbstractItemView::SingleSelection) && !(vopt->state & QStyle::State_KeyboardFocusChange))
				adjustedOption.state &= ~QStyle::State_HasFocus;

			opt = adjustedOption;
			vopt = &adjustedOption;
		}
#endif

		p->save();
		p->setClipRect(opt.rect);

		QRect checkRect = style->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, vopt, widget);
		QRect iconRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, vopt, widget);
		QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, vopt, widget);

		if (vopt->state & QStyle::State_Selected)
		{
			opt.state &= ~(QStyle::State_Selected);

			opt.backgroundBrush = QBrush(QColor(255, 215, 188));
// 			QPalette palette = opt.palette;
// 			palette.setColor(QPalette::Highlight, QColor(128, 0, 0));
// 			palette.setColor(QPalette::Base, QColor(128, 0, 0));
// 			opt.palette = palette;
// 			vopt = &opt;
		}
		if (vopt->state & QStyle::State_HasFocus || currentRow == index.row())
			opt.state |= QStyle::State_Selected;

		// draw the background
		style->proxy()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, p, widget);

		// draw the check mark
		if (vopt->features & QStyleOptionViewItemV2::HasCheckIndicator)
		{
			QStyleOptionViewItemV4 option(*vopt);
			option.rect = checkRect;
			option.state = option.state & ~QStyle::State_HasFocus;

			switch (vopt->checkState)
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
		if (!(vopt->state & QStyle::State_Enabled))
			mode = QIcon::Disabled;
		else if (vopt->state & QStyle::State_Selected)
			mode = QIcon::Selected;
		QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
		vopt->icon.paint(p, iconRect, vopt->decorationAlignment, mode, state);

		// draw the text
		if (!vopt->text.isEmpty())
		{
			QPalette::ColorGroup cg = vopt->state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
			if (cg == QPalette::Normal && !(vopt->state & QStyle::State_Active))
				cg = QPalette::Inactive;

			if (vopt->state & QStyle::State_Selected)
				p->setPen(vopt->palette.color(cg, QPalette::HighlightedText));
			else
				p->setPen(vopt->palette.color(cg, QPalette::Text));

			if (vopt->state & QStyle::State_Editing)
			{
				p->setPen(vopt->palette.color(cg, QPalette::Text));
				p->drawRect(textRect.adjusted(0, 0, -1, -1));
			}

			QCommonStylePrivate_viewItemDrawText(style, p, vopt, textRect, !(index.data(FileListModel::AttributesRole).toUInt() & FileInfo::Directory));
		}

		// draw the focus rect
// 		if (vopt->state & QStyle::State_HasFocus)
// 		{
// 			QStyleOptionFocusRect o;
// 			o.QStyleOption::operator=(*vopt);
// 			o.rect = style->subElementRect(QStyle::SE_ItemViewItemFocusRect, vopt, widget);
// 			o.state |= QStyle::State_KeyboardFocusChange;
// 			o.state |= QStyle::State_Item;
// 			QPalette::ColorGroup cg = (vopt->state & QStyle::State_Enabled)	? QPalette::Normal : QPalette::Disabled;
// 			o.backgroundColor = vopt->palette.color(cg, (vopt->state & QStyle::State_Selected) ? QPalette::Highlight : QPalette::Window);
// 			style->proxy()->drawPrimitive(QStyle::PE_FrameFocusRect, &o, p, widget);
// 		}

		p->restore();
	}
}

void FilesDelegate::PaletteChanged()
{
#if defined(Q_OS_WIN32) || defined(Q_OS_WINCE)
	use_xp = QWindowsXPStylePrivate_resolveSymbols() && pIsThemeActive() && (pIsAppThemed() || !QApplication::instance());
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