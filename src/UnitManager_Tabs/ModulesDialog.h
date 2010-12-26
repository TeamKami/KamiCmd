#ifndef ModulesDialog_H
#define ModulesDialog_H

#include <QDialog>
#include <QTreeWidget>
#include <QVBoxLayout>

class ModulesDialog : public QDialog
{
	Q_OBJECT

public:
	ModulesDialog(QWidget *parent);

private:
	QTreeWidget *tree;
	QVBoxLayout *layout;
	void populateTree();

	QIcon libraryIcon, moduleIcon;
	
};

#endif // ModulesDialog_H
