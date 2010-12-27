#include "oqwidget.h"
#include "OperationsQueue.h"

OQWidget::OQWidget(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.treeWidget->setColumnCount(3);
	ui.treeWidget->setHeaderLabels(QStringList() << "Operation type" << "Progress" << "State");
	
	if (!(operationsQueue_ = dynamic_cast<IOperationsQueue *>(g_Core->QueryModule("OperationsQueue", 1, "OQ"))))
	{
		g_Core->DebugWrite(g_moduleName, "Operations Queue module not found", ICoreFunctions::Error);
		done(0);
		return;
	}
	int count = operationsQueue_->size();
 	QList<QTreeWidgetItem *> items;
	
	for(int i = 0; i < count; i++)
	{
		const IFileOperation* fileOperation = operationsQueue_->getFileOperation(i);
		
		QString state;
		switch(fileOperation->getState())
		{
		case IFileOperation::Paused:
			state = tr("Paused");
			break;
		case IFileOperation::Running:
			state = tr("Running");
			break;
		case IFileOperation::ForcedRunning:
			state = tr("[F] Running");
			break;
		case IFileOperation::Queued:
			state = tr("Queued");
			break;
		case IFileOperation::Error:
			state = tr("Error");
			break;
		case IFileOperation::Finished:
			state = "Finished";
			break;
		default:
			state = tr("Unknown state");
		}
		QString & t = fileOperation->getType();
		items << new QTreeWidgetItem((QTreeWidget*)0, QStringList() << t
			<< QString::number(fileOperation->getProgress()) + "%" << state);
	}
	ui.treeWidget->insertTopLevelItems(0, items);
}

OQWidget::~OQWidget()
{

}
