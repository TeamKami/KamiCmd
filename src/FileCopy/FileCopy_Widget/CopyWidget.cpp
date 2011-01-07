#include "CopyWidget.h"
#include "IUnitManager.h"
#include "library.h"
#include "IFileCopy.h"
#include "IPanel.h"

CopyWidget::CopyWidget(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	IUnitManager *unitManagerTabs = dynamic_cast<IUnitManager *>(g_Core->QueryModule("UnitManager", 1));
	if(!unitManagerTabs)
		done(0);
 	ILinkedUnit *unit = dynamic_cast<ILinkedUnit *> (unitManagerTabs->GetActiveUnit());
	IPanel *active = dynamic_cast<IPanel *>(unit->GetActiveUnit());
	IPanel *passive = dynamic_cast<IPanel *>(unit->GetInactiveUnit());
	
	if(!active)
	{
		g_Core->DebugWrite("File Copy", "Not supported");
		done(0);
	}

	files = active->GetSelectedFiles();
	if(files.size() > 1)
		ui.copyLabel->setText(tr("Cop&ying %1 files to").arg(files.size()));
	else
		ui.copyLabel->setText(tr("Cop&ying \"%1\" to").arg(files[0]->name == ".." ? files[0]->path.mid(0, files[0]->path.lastIndexOf("/")) : files[0]->name));
	QString destination = passive ? passive->GetPath() : active->GetPath();
	ui.copyToComboBox->addItem(destination);

	connect(ui.copyPushButton, SIGNAL(clicked()), this, SLOT(on_copyPushButtonClicked()));
}

CopyWidget::~CopyWidget()
{

}

void CopyWidget::on_copyPushButtonClicked()
{
	IFileCopy *copy = dynamic_cast<IFileCopy *>(g_Core->QueryModule("FileCopy", 1, "FileCopy_Windows"));
	if(!copy)
	{
		g_Core->DebugWrite("Copy Widget", "Failed to load copy module!");
		done(0);
	}
	IOperationsQueue *queue = dynamic_cast<IOperationsQueue *>(g_Core->QueryModule("OperationsQueue", 1, "OQ"));
	if(!queue)
	{
		g_Core->DebugWrite("Copy Widget", "Failed to load operations queue module!");
		done(0);
	}
	QStringList paths;
	for(int i = 0; i < files.size(); i++)
		paths << files[i]->path + files[i]->name;

	copy->PrepareForCopy(paths, 
		ui.copyToComboBox->currentText(), 
		ui.filterCheckBox->isChecked() ? ui.includeMaskComboBox->currentText() : "",
		ui.filterCheckBox->isChecked() ? ui.excludeMaskComboBox->currentText() : "");
	
	queue->Add(*copy, IFileOperation::Running);	
	done(0);
}
