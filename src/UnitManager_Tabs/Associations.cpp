#include "Associations.h"
#include "AssociationsDialog.h"
#include <QRegExp>
#include <QSettings>


QColor Associations::QColorFromQString(QString str)
{
	QColor out;
	QStringList list = str.split(' ');
	if (list.size() == 3 || list.size() == 4)
	{
		out = QColor(Qt::black);
		for (int i = 0; i < list.size(); i++)
			switch (i)
		{
			case 0: out.setRed(list[i].toInt()); break;
			case 1: out.setGreen(list[i].toInt()); break;
			case 2: out.setBlue(list[i].toInt()); break;
			case 3: out.setAlpha(list[i].toInt()); break;
		}
	}
	return out;
}

QString Associations::QStringFromQColor(QColor color)
{
	if (color.isValid())
	{
		if (color.alpha() != 255)
			return QString("%1 %2 %3 %4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
		return QString("%1 %2 %3").arg(color.red()).arg(color.green()).arg(color.blue());
	}
	return QString();
}

Associations::Associations(QObject *parent)
	: QObject(parent)
{
// 	Ascs.resize(1);
// 	Ascs[0].mask = "*.exe,*.com,*.bat";
// 	Ascs[0].iconPath = ":/Icons/Executables.png";
// 	Ascs[0].description = "Executables";
// 	Ascs[0].textColor = QColor(0, 128, 0);
// 	Ascs[0].actions.append("start !/!.!");

// 	Actions.append(new QAction(this));
// 	Actions.last()->setText("Execute");
// 	Actions.last()->setShortcuts(QList<QKeySequence>() << Qt::Key_Enter << Qt::Key_Return);
// 
// 	Actions.append(new QAction(this));
// 	Actions.last()->setText("Enter archive");
// 	Actions.last()->setShortcuts(QList<QKeySequence>() << Qt::CTRL + Qt::Key_PageDown);
// 
// 	Actions.append(new QAction(this));
// 	Actions.last()->setText("Viewer");
// 	Actions.last()->setShortcuts(QList<QKeySequence>() << Qt::Key_F3);
// 
// 	Actions.append(new QAction(this));
// 	Actions.last()->setText("Alternative Viewer");
// 	Actions.last()->setShortcuts(QList<QKeySequence>() << Qt::ALT + Qt::Key_F3);
// 
// 	Actions.append(new QAction(this));
// 	Actions.last()->setText("Editor");
// 	Actions.last()->setShortcuts(QList<QKeySequence>() << Qt::Key_F4);
// 
// 	Actions.append(new QAction(this));
// 	Actions.last()->setText("Alternative Editor");
// 	Actions.last()->setShortcuts(QList<QKeySequence>() << Qt::ALT + Qt::Key_F4);
// 
// 	for (int i = 0; i < Actions.size(); i++)
// 		Actions[i]->setShortcutContext(Qt::WidgetShortcut);

	LoadSettings();
}

void Associations::ShowDialog( QWidget *parent )
{
	AssociationsDialog *dialog = new AssociationsDialog(parent);
	Dialog = dialog;
	dialog->PopulateList(&Ascs, &Actions);
	connect(dialog, SIGNAL(Closing()), SLOT(SaveSettings()));
	dialog->show();
}

Associations::~Associations()
{
}

Association* Associations::GetAssociation(QString path)
{
	int dotPos = path.lastIndexOf(".");
	QString extension;
	if (dotPos != -1)
		extension = path.right(path.size() - dotPos - 1);

	for (int i = 0; i < Ascs.size(); i++)
	{
		QString mask = Ascs.at(i).mask;
		// If we just used QRegExp - it would be too slow, so this is "optimization" for the most common type of mask "*.extension"
		for (int j = 0, start = 0, isSimple = false; j < mask.size(); j++)
		{
			if (mask.at(j) == ';') // new mask
			{
				if (isSimple && !extension.compare(mask.mid(start + 2, j - start - 2), Qt::CaseInsensitive)) // mask happened to be "*.ext"
					return &Ascs[i];
				else if (!isSimple) // mask is more complicated than "*.ext". For further speed optimizations rewrite without the use of QRegExp
				{
					QRegExp expr(mask.mid(start, j - start), Qt::CaseInsensitive, QRegExp::Wildcard);
					if (expr.exactMatch(path) || mask == "*.*") // CRUTCHES. Somehow "*.*" is not equal to "*" while in windows it is
						return &Ascs[i];
				}

				start = j + 1;
			}
			else if (mask.at(j) == '*') // assumed start of "*.ext"
			{
				if (mask.size() > j + 1 && mask.at(j+1) == '.' && start == j)
				{
					isSimple = true;
					j++;
				}
				else
					isSimple = false;
			}
			else if (mask.at(j) == '.')
				isSimple = false;
		}
	}

 	return NULL;

// This whole implementation happened to be too slow, so it is rewritten above
/*
		for (int i = 0; i < Ascs.size(); i++)
		{
			QStringList listOfExt = Ascs[i].mask.split(';');
			foreach (QString mask, listOfExt)
			{
				bool needRegExp = false;
	
				// If we just used QRegExp - it would be too slow, so this is "optimization" for the most common type of mask "*.extension"
				{
					if (mask.size() > 2 && mask.at(0) == '*' && mask.at(1) == '.')
					{
						for (int j = 0; j < mask.size(); j++)
							if (mask.at(j) == '*' && j || mask.at(j) == '.' && j != 1)
							{
								needRegExp = true;
								break;
							}
						if (!needRegExp)
						{
							int dotPos = path.lastIndexOf(".");
							if (dotPos != -1 && !path.right(path.size() - dotPos - 1).compare(mask.right(mask.size() - 2), Qt::CaseInsensitive))
								return &Ascs[i];
						}
					}
					else
						needRegExp = true;
				}
	
				if (needRegExp && 0)
				{
					QRegExp expr(mask, Qt::CaseInsensitive, QRegExp::Wildcard);
					if (expr.exactMatch(path) || mask == "*.*") // CRUTCHES. Somehow "*.*" is not equal to "*" while in windows it is
						return &Ascs[i];
				}
			}
		}
		return NULL;*/
	
}

QColor Associations::GetTextColor( QString path )
{
	if (Association *assc = GetAssociation(path))
		return assc->textColor;
	return QColor();
}

QColor Associations::GetBackColor( QString path )
{
	if (Association *assc = GetAssociation(path))
		return assc->backColor;
	return QColor();
}

void Associations::SaveSettings()
{
	QSettings set;
	set.setIniCodec("UTF-8");
	set.beginGroup("Associations");
	set.remove("");

	set.beginWriteArray("Actions");
	for (int i = 0; i < Actions.size(); i++)
	{
		set.setArrayIndex(i);
		set.setValue("Text", Actions[i]->text());

		set.beginWriteArray("Shortcuts");
		for (int j = 0; j < Actions[i]->shortcuts().size(); j++)
		{
			set.setArrayIndex(j);
			set.setValue("Keys", Actions[i]->shortcuts().at(j));
		}
		set.endArray();
	}
	set.endArray();

	set.beginWriteArray("Associations");
	for (int i = 0; i < Ascs.size(); i++)
	{
		set.setArrayIndex(i);
		set.setValue("Description", Ascs[i].description);
		set.setValue("Mask", Ascs[i].mask);
		set.setValue("IconPath", Ascs[i].iconPath);
		set.setValue("TextColor", QStringFromQColor(Ascs[i].textColor));
		set.setValue("BackColor", QStringFromQColor(Ascs[i].backColor));

		set.beginWriteArray("Actions");
		for (int j = 0; j < Ascs[i].actions.size(); j++)
		{
			set.setArrayIndex(j);
			set.setValue("Path", Ascs[i].actions[j]);
		}
		set.endArray();
	}
	set.endArray();
	set.endGroup();
}

void Associations::LoadSettings()
{
	QSettings set;
	set.setIniCodec("UTF-8");
	set.beginGroup("Associations");

	int n = set.beginReadArray("Actions");
	for (int i = 0; i < n; i++)
	{
		set.setArrayIndex(i);
		Actions.append(new QAction(this));
		Actions.last()->setText(set.value("Text").toString());
		Actions.last()->setShortcutContext(Qt::WidgetShortcut);

		QList<QKeySequence> keys;
		int n2 = set.beginReadArray("Shortcuts");
		for (int j = 0; j < n2; j++)
		{
			set.setArrayIndex(j);
			keys.append(set.value("Keys").toString());
		}
		set.endArray();

		Actions.last()->setShortcuts(keys);
	}
	set.endArray();

	n = set.beginReadArray("Associations");
	for (int i = 0; i < n; i++)
	{
		set.setArrayIndex(i);
		Ascs.resize(Ascs.size() + 1);
		Ascs.last().description = set.value("Description").toString();
		Ascs.last().mask = set.value("Mask").toString();
		Ascs.last().iconPath = set.value("IconPath").toString();
		Ascs.last().textColor = QColorFromQString(set.value("TextColor").toString());
		Ascs.last().backColor = QColorFromQString(set.value("BackColor").toString());
		Ascs.last().icon = QIcon(Ascs.last().iconPath);

		int n2 = set.beginReadArray("Actions");
		for (int j = 0; j < n2; j++)
		{
			set.setArrayIndex(j);
			Ascs.last().actions.append(set.value("Path").toString());
		}
		for (int j = 0; j < Actions.size() - n2; j++) // Case when appended new action
			Ascs.last().actions.append(QString());
		set.endArray();
	}
	set.endArray();
	set.endGroup();
}

void Associations::GetIcon( QIcon &outIcon, QString path )
{
	// 	QRegExp expr("*.exe", Qt::CaseInsensitive, QRegExp::Wildcard);
	// 	if (expr.indexIn(mask) != -1)
	// 
	if (Association *assc = GetAssociation(path))
		if (!assc->iconPath.isEmpty())
		{
			outIcon = assc->icon;
			return;
		}

	if (Ascs.size() > 7)
		outIcon = Ascs.at(7).icon;
// 	QFileInfo info(path);
// 	outIcon = provider.icon(info);
}