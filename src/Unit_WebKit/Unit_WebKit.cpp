#include "Unit_WebKit.h"
#include "library.h"
#include "../IPanel.h"

Unit_WebKit::Unit_WebKit(QWidget *parent)
	: IUnit(parent)
{
// 	layout = new QVBoxLayout(this);
// 	layout->setMargin(0);
	setCentralWidget(webView = new QWebView(this));
//	layout->addWidget(webView);
}

QString Unit_WebKit::GetText()
{
	return text;
}

void Unit_WebKit::SaveState( QSettings &/*set*/ )
{
}

void Unit_WebKit::LoadState( QSettings &/*set*/ )
{
}

void Unit_WebKit::Create( IUnit *createdFrom )
{
	if (IPanel *panel = dynamic_cast<IPanel *>(createdFrom))
	{
		const FileInfo *const info = panel->GetCurrentFile();
		text = info->name;
		path = info->path;
		emit TextChanged();
		OpenFile();
	}
}

void Unit_WebKit::OpenFile()
{
	webView->load(path + text);
}

QIcon Unit_WebKit::GetIcon()
{
	return QIcon();
}
