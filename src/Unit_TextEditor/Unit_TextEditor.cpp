#include "Unit_TextEditor.h"
#include "library.h"
#include <Qsci/qsciscintilla.h>

Unit_TextEditor::Unit_TextEditor(QWidget *parent)
	: IUnit(parent)
{
	setCentralWidget(editor = new QsciScintilla(this));
}

QString Unit_TextEditor::GetText()
{
	return text;
}

void Unit_TextEditor::SaveState( QSettings &/*set*/ )
{
}

void Unit_TextEditor::LoadState( QSettings &/*set*/ )
{
}

void Unit_TextEditor::Create( IUnit *createdFrom )
{
	if (hostPanel = dynamic_cast<IPanel *>(createdFrom))
	{
		const FileInfo *const info = hostPanel->GetCurrentFile();
		text = info->name;
		path = info->path;
		emit TextChanged();

		QFile file(info->path + info->name);

		if (!file.open(QFile::ReadOnly)) {
			QMessageBox::warning(this, tr("Application"),
								 tr("Cannot read file %1:\n%2.")
								 .arg(info->path + info->name)
								 .arg(file.errorString()));
			return;
		}

		QTextStream in(&file);
		QApplication::setOverrideCursor(Qt::WaitCursor);
		editor->setText(in.readAll());
		QApplication::restoreOverrideCursor();
	}
}

QIcon Unit_TextEditor::GetIcon()
{
	return QIcon();
}

