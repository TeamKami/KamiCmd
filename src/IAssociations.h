#ifndef ASSOCIATIONS_INTERFACE_H
#define ASSOCIATIONS_INTERFACE_H

#include <QObject>
#include <QDialog>
#include <QAction>
#include <QDateTime>

struct Association
{
	QString mask;
	QString description;
	QStringList actions;
	QString iconPath;
	QIcon icon;
	QColor textColor;
	QColor backColor;

	quint32 attributes;
	quint32 attributesMask;
	qint64 sizeLowerBound;
	qint64 sizeUpperBound;
	QDateTime timeLowerBound;
	QDateTime timeUpperBound;
	quint8 timeFlags;
};

class IAssociations
{
public:
	virtual void ShowDialog(QWidget *parent) = 0;
	virtual QColor GetTextColor(QString path) = 0;
	virtual QColor GetBackColor(QString path) = 0;
	virtual void GetIcon(QIcon &outIcon, QString path) = 0;
};

#endif // ASSOCIATIONS_INTERFACE_H
