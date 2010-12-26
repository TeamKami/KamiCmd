#ifndef UNIT_WEBKIT_H
#define UNIT_WEBKIT_H

#include <QtGui>
#include "../IUnitManager.h"
#include <QWebView>

// class WebKit : public QWidget
// {
// public:
// 	WebKit(QWidget *parent);
// 
// private:
// 	void paintEvent(QPaintEvent *event);
// };
// 
class Unit_WebKit : public IUnit
{
	Q_OBJECT

public:
	Unit_WebKit(QWidget *parent);
	void Create(IUnit *createdFrom); // overload
	void Link(IUnit *withUnit) {}; // overload
	QString GetText(); // overload
	void SaveState(QSettings &set); // overload
	void LoadState(QSettings &set); // overload

private:
	QVBoxLayout *layout;
	QWebView *webView;
	QString text;
	QString path;

	void OpenFile();

signals:
	void TextChanged();
};

#endif // UNIT_WEBKIT_H