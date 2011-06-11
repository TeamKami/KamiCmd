#ifndef UNIT_WEBKIT_H
#define UNIT_WEBKIT_H

#include <QtGui>
#include "../IUnitManager.h"
#include <QWebView>

class Unit_WebKit : public IUnit
{
	Q_OBJECT

public:
	Unit_WebKit(QWidget *parent);
	virtual void Create(IUnit *createdFrom);
	virtual void Link(IUnit * /*withUnit*/) {};
	virtual QString GetText();
	virtual QIcon GetIcon();
	virtual void SaveState(QSettings &set);
	virtual void LoadState(QSettings &set);

private:
	QVBoxLayout *layout;
	QWebView *webView;
	QString text;
	QString path;

	void OpenFile();

signals:
	/*virtual*/ void TextChanged();
};

#endif // UNIT_WEBKIT_H