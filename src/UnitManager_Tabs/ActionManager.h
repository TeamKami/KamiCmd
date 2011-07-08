#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>
#include <QDialog>
#include "IUnitManager.h"

class ActionManager : public QObject, public IActionManager
{
	Q_OBJECT

public:
	ActionManager(QObject *parent);
	~ActionManager();

	virtual void ShowDialog(QWidget *parent);
	virtual void RegisterActions(QList<QAction *> actions);
	virtual void UnregisterActions(QList<QAction *> actions);
private:
	QList<QAction *> Actions;
};

#endif // ACTIONMANAGER_H
