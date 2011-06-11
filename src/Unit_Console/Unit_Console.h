#ifndef UNIT_Console_H
#define UNIT_Console_H

#include <QtGui>
#include "../IUnitManager.h"
#include "ConsoleWidget.h"

class Unit_Console : public IUnit
{
	Q_OBJECT

public:
	Unit_Console(QWidget *parent);
	virtual void Create(IUnit *createdFrom);
	virtual void Link(IUnit * /*withUnit*/) {};
	virtual QString GetText();
	virtual QIcon GetIcon();
	virtual void SaveState(QSettings &set);
	virtual void LoadState(QSettings &set);

	QString ConvertFromConsoleCP(QByteArray string);
	QByteArray ConvertToConsoleCP(QString string);
private:
	QVBoxLayout *layout;
	ConsoleWidget *console;
	QString text;
	QString path;

	QProcess *cmd;

	void CreateConsole();

signals:
	void TextChanged();

private slots:
	void readyReadStandardOutput();
	void ExecuteCommand(QString cmd);
};

#endif // UNIT_Console_H