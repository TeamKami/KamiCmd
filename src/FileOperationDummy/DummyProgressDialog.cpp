#include "DummyProgressDialog.h"
#include "FileOperationDummy.h"
#include "IFileOperation.h"
#include "library.h"

DummyProgressDialog::DummyProgressDialog(IFileOperation *fileOperation, QWidget *parent)
	: QDialog(parent), f(fileOperation)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);

	timer.start(UpdateInterval);

	queue = dynamic_cast<IOperationsQueue *>(g_Core->QueryModule("OperationsQueue", 1));
	if(!queue)
	{
		g_Core->DebugWrite("DummyFileOperation", "Unable to query OperationsQueue");
		close();
	}

	connect(&timer, SIGNAL(timeout()), SLOT(timeout()));
}

DummyProgressDialog::~DummyProgressDialog()
{

}

void DummyProgressDialog::timeout()
{
	if(queue->IsValid(f) && f->GetState() != IFileOperation::Finished)
		ui.progress->setValue(f->GetProgress());
	else
		close();
}

void DummyProgressDialog::on_pause_clicked()
{
	int s = f->GetState();
	if(s == IFileOperation::Paused)
	{
		ui.pause->setText("Resume");
		queue->Pause(f);
	}
	else if(s == IFileOperation::Running)
	{
		ui.pause->setText("Pause");
		queue->Resume(f);
	}
}

void DummyProgressDialog::on_cancel_clicked()
{
	if(queue->IsValid(f))
	{
		queue->Cancel(f);
		close();
	}
}

QWidget * DummyProgressDialog::ProgressBar()
{
	return ui.progress;
}
