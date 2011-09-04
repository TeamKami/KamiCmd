#include <QtCore/QStack>
#include <QtGui/QMessageBox>

#include "CopyDialog.h"
#include "CopyProgressDialog.h"
#include "IUnitManager.h"
#include "IPanel.h"

CopyDialog::CopyDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog);
	setAttribute(Qt::WA_DeleteOnClose);
	InitializeCopy();

	fileSystem = dynamic_cast<IFileSystem *>(g_Core->QueryModule("FS", 1));
	if(!fileSystem)
		g_Core->DebugWrite("FileCopy", "Can't query FS");
}

CopyDialog::~CopyDialog()
{

}

void CopyDialog::InitializeCopy()
{
	IUnitManager *unitManagerTabs = dynamic_cast<IUnitManager *>(g_Core->QueryModule("UnitManager", 1));
	if(!unitManagerTabs)
	{
		g_Core->DebugWrite("FileCopy_Qt", "Can't query UnitManager");
		close();
	}

	ILinkedUnit *linkedUnit = dynamic_cast<ILinkedUnit *> (unitManagerTabs->GetActiveUnit());
	IPanel *activePanel = dynamic_cast<IPanel *>(linkedUnit->GetActiveUnit());
	IPanel *inactivePanel = dynamic_cast<IPanel *> (linkedUnit->GetInactiveUnit());

	selectedFiles = activePanel->GetSelectedFiles();
	if(selectedFiles.size() > 1)
		ui.copyDestinationLabel->setText(tr("Copy %1 files to").arg(selectedFiles.size()));
	else if(selectedFiles.size() == 1)
		ui.copyDestinationLabel->setText(tr("Copy %1 to").arg(selectedFiles.at(0).path + selectedFiles.at(0).name));
	ui.copyToComboBox->setEditText(inactivePanel->GetPath());

}

void CopyDialog::on_copyPushButton_clicked()
{
	QStack<const FileInfo*> scan;
	QVector<FileInfo *> filesToCopy;
	
	for(int i = 0; i < selectedFiles.size(); i++)
		if(selectedFiles.at(i).attributes & FileInfo::Directory)
			scan.push(&selectedFiles[i]);
		else
			filesToCopy.append(&selectedFiles[i]);

	while(!scan.empty())
	{
		const FileInfo *topItem = scan.pop();
		QVector< FileInfo* > filesInFolder = ScanFolder(topItem->path + topItem->name);
		for(int i = 0; i < filesInFolder.size(); i++)
			if(filesInFolder.at(i)->attributes & FileInfo::Directory)
				scan.push(filesInFolder.at(i));
			else
				filesToCopy.append(filesInFolder.at(i));
	}

	fileCopyOperation.PrepareForCopy(filesToCopy, ui.copyToComboBox->currentText());

	CopyProgressDialog *dialog = new CopyProgressDialog(&fileCopyOperation, this);
	hide();
	dialog->exec();
	close();

}

QVector< FileInfo *> CopyDialog::ScanFolder( const QString & path )
{
	if(!fileSystem->SetPath(path))
		return QVector<FileInfo *>();

	QVector<FileInfo *> files(fileSystem->GetNumberOfFiles());
	for(int i = 0; i < files.size(); i++) // this memory is not deleted
		files[i] = new FileInfo;

	int i = 0;
	for(bool r = fileSystem->GetFirstFileInfo(*files[i]); 
		r && i < files.size() - 1;
		r = fileSystem->GetNextFileInfo(*files[++i]));

	return files;		
}

