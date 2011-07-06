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
	ui.tabWidth->setValue(sci_->tabWidth());

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
	sci_->setAutoIndent(ui.autoIndent->isChecked());
	sci_->setBackspaceUnindents(ui.backspaceUnindents->isChecked());
	sci_->setIndentationGuides(ui.indentationGuides->isChecked());
	sci_->setIndentationsUseTabs(ui.useTabs->isChecked());
	sci_->setTabIndents(ui.tabIndents->isChecked());
	sci_->setIndentationWidth(ui.indentationWidth->value());
	sci_->setTabWidth(ui.tabWidth->value());

	sci_->setFolding(static_cast<QsciScintilla::FoldStyle>(ui.foldingStyle->currentIndex()));

	sci_->setEdgeColumn(ui.edgeColumn->value());
	sci_->setEdgeMode(static_cast<QsciScintilla::EdgeMode>(ui.edgeType->currentIndex()));

	sci_->setAutoCompletionCaseSensitivity(ui.autocompletionCaseSensitive->isChecked());
	sci_->setAutoCompletionReplaceWord(ui.autocompletionReplaceWord->isChecked());
	sci_->setAutoCompletionShowSingle(ui.autocompletionShowSingle->isChecked());
	sci_->setAutoCompletionFillupsEnabled(ui.autocompletionFillups->isChecked());
	sci_->setAutoCompletionThreshold(ui.autocompletionTreshold->value());
	sci_->setAutoCompletionSource(static_cast<QsciScintilla::AutoCompletionSource>(ui.autocompletionSource->currentIndex()));
	sci_->setAutoCompletionUseSingle(static_cast<QsciScintilla::AutoCompletionUseSingle>(ui.autocompletionUseSingle->currentIndex()));

	sci_->setWrapMode(static_cast<QsciScintilla::WrapMode>(ui.wrapMode->currentIndex()));
	sci_->setWrapIndentMode(static_cast<QsciScintilla::WrapIndentMode>(ui.wrapIndentation->currentIndex()));
}

