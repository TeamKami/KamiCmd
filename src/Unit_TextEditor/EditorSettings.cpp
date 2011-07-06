#include "EditorSettings.h"
#include <QMessageBox>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <Qsci/qsciscintilla.h>

EditorSettings::EditorSettings(QWidget *parent, QsciScintilla * sci)
	: QWidget(parent), sci_(sci)
{
	ui.setupUi(this);

	ui.autoIndent->setChecked(sci_->autoIndent());
	ui.backspaceUnindents->setChecked(sci_->backspaceUnindents());
	ui.indentationGuides->setChecked(sci_->indentationGuides());
	ui.useTabs->setChecked(sci_->indentationsUseTabs());
	ui.tabIndents->setChecked(sci_->tabIndents());
	ui.indentationWidth->setValue(sci_->indentationWidth());
	ui.tabWIdth->setValue(sci_->tabWidth());
	ui.indentationWidth->setValue(sci_->indentationWidth());

	ui.foldingStyle->setCurrentIndex(sci_->folding());

	ui.edgeColumn->setValue(sci_->edgeColumn());
	ui.edgeType->setCurrentIndex(sci_->edgeMode());

	ui.autocompletionCaseSensitive->setChecked(sci_->autoCompletionCaseSensitivity());
	ui.autocompletionReplaceWord->setChecked(sci_->autoCompletionReplaceWord());
	ui.autocompletionShowSingle->setChecked(sci_->autoCompletionShowSingle());
	ui.autocompletionFillups->setChecked(sci_->autoCompletionFillupsEnabled());
	ui.autocompletionTreshold->setValue(sci_->autoCompletionThreshold());
	ui.autocompletionSource->setCurrentIndex(sci_->autoCompletionSource());
	ui.autocompletionUseSingle->setCurrentIndex(sci_->autoCompletionUseSingle());

	ui.wrapMode->setCurrentIndex(sci_->wrapMode());
	ui.wrapIndentation->setCurrentIndex(sci_->wrapIndentMode());
}


EditorSettings::~EditorSettings()
{

}

void EditorSettings::save()
{
}

