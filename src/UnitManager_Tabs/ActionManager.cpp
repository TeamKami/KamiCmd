#include "ActionManager.h"
#include "ActionsDialog.h"

ActionManager::ActionManager(QObject *parent)
	: QObject(parent)
{
}

ActionManager::~ActionManager()
{

}

bool lessThan(QAction *a1, QAction *a2)
{
	return a1->data().toString() < a2->data().toString();
}

void ActionManager::ShowDialog( QWidget *parent )
{
	qStableSort(Actions.begin(), Actions.end(), lessThan);

	ActionsDialog *dialog = new ActionsDialog(parent);
	//Dialog->PopulateList(&Ascs, &Actions);
	static_cast<ActionsDialog *>(dialog)->show(Actions);
}

void ActionManager::RegisterActions( QList<QAction *> actions )
{
	Actions.append(actions);
}

void ActionManager::UnregisterActions( QList<QAction *> actions )
{
	foreach (QAction *act, actions)
		Actions.removeOne(act);
}
