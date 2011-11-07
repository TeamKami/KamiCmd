#include "OperationsQueue.h"
#include "OperationQueueDialog.h"
#include "OperationQueueModel.h"

OperationsQueueDialog::OperationsQueueDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	if (!(operationsQueue_ = dynamic_cast<IOperationsQueue *>(g_Core->QueryModule("OperationsQueue", 1, "", -1, this))))
	{
		g_Core->DebugWrite("OperationsQueue", "Operations Queue module not found", ICoreFunctions::Error);
		close();
		return;
	}
	int count = operationsQueue_->GetCount();

	OperationQueueModel *model = new OperationQueueModel(static_cast<OperationsQueue *>(operationsQueue_), this);
	ui.treeView->setModel(model);
}

OperationsQueueDialog::~OperationsQueueDialog()
{

}

void OperationsQueueDialog::on_addDummyButton_clicked()
{
	IFileOperation *op = dynamic_cast<IFileOperation *>(g_Core->QueryModule("DummyFileOperation", 1));

	if(op)
	{
		operationsQueue_->Add(op, IFileOperation::Running);
		op->ShowProgressDialog(this);
	}
	else
		g_Core->DebugWrite("OperationsQueue", "Can't query DummyFileOperation");
}
 