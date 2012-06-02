#include "OperationsQueue.h"
#include "OperationQueueDialog.h"
#include "OperationQueueModel.h"
#include "ProgressDelegate.h"

OperationsQueueDialog::OperationsQueueDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	if (!(queue = dynamic_cast<IOperationsQueue *>(g_Core->QueryModule("OperationsQueue", 1, "", -1, this))))
	{
		g_Core->DebugWrite("OperationsQueue", "Operations Queue module not found", ICoreFunctions::Error);
		close();
		return;
	}

	OperationQueueModel *model = new OperationQueueModel(static_cast<OperationsQueue *>(queue), this);
	ui.treeView->setItemDelegate(new ProgressDelegate(ui.treeView));
	ui.treeView->setModel(model);
}

OperationsQueueDialog::~OperationsQueueDialog()
{

}

void OperationsQueueDialog::on_addDummyButton_clicked()
{
    // This line did not compile on Mac OS X. I assume the cause is inheritance of QObject in IFileOperation interface
    IFileOperation *op = dynamic_cast<IFileOperation *>(g_Core->QueryModule("DummyFileOperation", 1));

	if(op)
	{
        queue->Add(op, IFileOperation::Running);
		op->ShowProgressDialog(this);
	}
	else
        g_Core->DebugWrite("OperationsQueue", "Can't query DummyFileOperation");
}
 
