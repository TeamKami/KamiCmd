#ifndef ASSOCIATIONS_H
#define ASSOCIATIONS_H

#include "../IAssociations.h"
#include <QFileIconProvider>
#include <QMainWindow>

class Associations : public QObject, public IAssociations
{
	Q_OBJECT

public:
	Associations(QObject *parent);
	~Associations();
	virtual void ShowDialog(QWidget *parent);
	virtual QColor GetTextColor(QString path);
	virtual QColor GetBackColor(QString path);
	virtual void GetIcon(QIcon &outIcon, QString path);

	Association* GetAssociation(QString path);
	void LoadSettings();

	static QColor QColorFromQString(QString str);
	static QString QStringFromQColor(QColor color);

private:
	QVector<Association> Ascs;
	QVector<QAction *> Actions;
	QMainWindow *Dialog;
	QFileIconProvider provider;

public slots:
	void SaveSettings();
};

#endif // ASSOCIATIONS_H
