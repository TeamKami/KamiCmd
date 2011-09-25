#ifndef DEBUGLOG_H
#define DEBUGLOG_H

#include <QtCore/QObject>
#include <QtCore/QDebug>

#include "DebugDialog.h"

class DebugLog : public QObject
{
	Q_OBJECT

public:
	DebugLog(QObject *parent);
	~DebugLog();

	void Write(QtMsgType type, const QString & msg);

public slots:
	void ShowDebugDialog();
signals:
	void SendText(QtMsgType type, const QString & msg);
private:
	DebugDialog *dialog;
};

#endif // DEBUGLOG_H
