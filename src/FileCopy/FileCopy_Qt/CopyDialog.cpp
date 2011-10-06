#include <QtCore/QStack>
#include <QtGui/QMessageBox>

#include "CopyDialog.h"
#include "CopyProgressDialog.h"
#include "IUnitManager.h"
#include "IPanel.h"
#include "IUnitManager.h"
#include "UnitManager_Tabs/LinkedUnit.h"

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
	connect(ui.cancelPushButton, SIGNAL(clicked()), SLOT(close()));
}

CopyDialog::~CopyDialog()
{

}

void CopyDialog::InitializeCopy()
{
	GetSelectedFilesAndCopyPath();

	if(selectedFiles.size() > 1)
	{
		ui.copyDestinationLabel->setText(tr("Copy %1 files to").arg(selectedFiles.size()));
		ui.copyToComboBox->setEditText(copyPath);
	}
	else if(selectedFiles.size() == 1)
	{
		ui.copyDestinationLabel->setText(tr("Copy %1 to").arg(selectedFiles.at(0).path + selectedFiles.at(0).name));
		ui.copyToComboBox->setEditText(copyPath + selectedFiles.at(0).name);
	}
}


void CopyDialog::GetSelectedFilesAndCopyPath()
{
	IUnitManager *unitManager = dynamic_cast<IUnitManager *>(g_Core->QueryModule("UnitManager", 1));
	if(!unitManager)
	{
		g_Core->DebugWrite("FileCopy_Qt", "Can't query UnitManager");
		close();
	}

	LinkedUnit *link = dynamic_cast<LinkedUnit *>(unitManager->GetActiveUnit());
	IPanel *activePanel = NULL;
	IPanel *inactivePanel = NULL;

	if (link)
	{
		activePanel = dynamic_cast<IPanel *>(link->GetActiveUnit());
		inactivePanel = dynamic_cast<IPanel *>(link->GetInactiveUnit());
	}
	else
		activePanel = dynamic_cast<IPanel *>(unitManager->GetActiveUnit());

	if (activePanel)
		selectedFiles = activePanel->GetSelectedFiles();
	else
	{
		g_Core->DebugWrite("FileCopy_Qt", "Copy called not from a panel");
		close();
	}

	copyPath = inactivePanel ? inactivePanel->GetPath() : activePanel->GetPath();
}



void CopyDialog::on_copyPushButton_clicked()
{
	GetFilesToCopy();
	fileCopyOperation.PrepareForCopy(filesToCopy);
	CopyProgressDialog *dialog = new CopyProgressDialog(&fileCopyOperation, this);
	hide();
	connect(dialog, SIGNAL(finished(int)), SLOT(close()));
	dialog->show();
}

void CopyDialog::GetFilesToCopy()
{	
	QStack<FileScanElement> scan;
	filesToCopy.SetDestination(copyPath);

	for(int i = 0; i < selectedFiles.size(); i++)
		if(selectedFiles.at(i).attributes & FileInfo::Directory)
			scan.push(FileScanElement(selectedFiles.at(i).name + '/', selectedFiles.at(i)));
		else if(selectedFiles.at(i).attributes & ~FileInfo::UpOneLevel)
			filesToCopy.AddFile("", selectedFiles.at(i));

	while(!scan.empty())
	{
		const FileScanElement topItem = scan.pop();
		QVector< FileInfo > filesInFolder = ScanFolder(topItem.fileInfo.path + topItem.fileInfo.name);

		for(int i = 0; i < filesInFolder.size(); i++)
			if(filesInFolder.at(i).attributes & FileInfo::Directory)
				scan.push(FileScanElement(topItem.relativePath + filesInFolder.at(i).name + '/', filesInFolder.at(i)));
			else
				filesToCopy.AddFile(topItem.relativePath, filesInFolder.at(i));
	}

	filesToCopy.PrintAllFiles();
}

QVector< FileInfo > CopyDialog::ScanFolder( const QString & path )
{
	if(!fileSystem->SetPath(path))
		return QVector<FileInfo>();

	QVector<FileInfo> files(fileSystem->GetNumberOfFiles());

	int i = 0;
	for(bool r = fileSystem->GetFirstFileInfo(files[i]); 
		r && i < files.size() - 1;
		r = fileSystem->GetNextFileInfo(files[++i]));

	return files;		
}
