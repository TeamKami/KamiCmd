	#include "ModulesDialog.h"
#include "library.h"

ModulesDialog::ModulesDialog( QWidget *parent )
 : QDialog(parent)
{
	resize(380, 360);
	setWindowTitle(tr("Modules"));

	tree = new QTreeWidget();
	tree->setAlternatingRowColors(false);
	tree->setSelectionMode(QAbstractItemView::NoSelection);
	tree->setAllColumnsShowFocus(true);
	tree->setUniformRowHeights(true);
//	tree->setSortingEnabled(true);
	tree->setColumnCount(2);
	tree->setHeaderLabels(QStringList() << tr("Name") << tr("Type"));

	layout = new QVBoxLayout();
	layout->addWidget(tree);
	setLayout(layout);

	libraryIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
		QIcon::Normal, QIcon::On);
	libraryIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
		QIcon::Normal, QIcon::Off);
	moduleIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

	populateTree();
}

bool moduleLessThan( const Module *first, const Module *second )
{
	if (first->parentLibName == second->parentLibName)
	{
		if (first->type == second->type)
		{
			if (first->interfaceVersion > second->interfaceVersion || 
				first->interfaceVersion == second->interfaceVersion && first->moduleVersion > second->moduleVersion)
				return true;
			return false;
		}
		return first->type < second->type;
	}
	return first->parentLibName < second->parentLibName;
}

void ModulesDialog::populateTree()
{
	QVector<const Module *> modules = g_Core->GetModulesInfo();
	qSort(modules.begin(), modules.end(), moduleLessThan);

	quintptr curLib = 0;
	QTreeWidgetItem *root = NULL;

	QFont boldFont = tree->font();
	boldFont.setBold(true);

	for (int i = 0; i < modules.size(); i++)
	{
		if (curLib != modules[i]->parentLib)
		{
			root = new QTreeWidgetItem(tree);
			root->setText(0, modules[i]->parentLibName);
			root->setIcon(0, libraryIcon);
			root->setFont(0, boldFont);
			tree->setItemExpanded(root, true);
			curLib = modules[i]->parentLib;
			i--;
		}
		else
		{
			QTreeWidgetItem *item = new QTreeWidgetItem(root);
			item->setIcon(0, moduleIcon);
			item->setText(0, modules[i]->name);
			item->setText(1, QString("%1 [%2.%3]").arg(modules[i]->type).arg(modules[i]->interfaceVersion).arg(modules[i]->moduleVersion));
		}
	}

	tree->resizeColumnToContents(0);
}