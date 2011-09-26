#ifndef DEBUGLOG_H
#define DEBUGLOG_H

#include <QtCore/QObject>
#include <QtCore/QDebug>

#include "DebugDialog.h"
#include "ILibrary.h"

class DebugLog : public QObject
{
	Q_OBJECT

public:
	DebugLog(QObject *parent);
	~DebugLog();

	void Write(ICoreFunctions::DebugWriteImportance type, const QString & msg);

public slots:
	void ShowDebugDialog();
signals:
	void SendText(ICoreFunctions::DebugWriteImportance type, const QString & msg);
	void ShowDialog();
private:
	DebugDialog *dialog;
};

#endif // DEBUGLOG_H
