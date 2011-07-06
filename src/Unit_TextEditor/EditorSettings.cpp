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
	//ui.autoIndent->setChecked(sci_->autoIndent());
	sci_->setAutoIndent(ui.autoIndent->isChecked());
	//ui.backspaceUnindents->setChecked(sci_->backspaceUnindents());
	sci_->setBackspaceUnindents(ui.backspaceUnindents->isChecked());
	//ui.indentationGuides->setChecked(sci_->indentationGuides());
	sci_->setIndentationGuides(ui.indentationGuides->isChecked());
	//ui.useTabs->setChecked(sci_->indentationsUseTabs());
	sci_->setIndentationsUseTabs(ui.useTabs->isChecked());
	//ui.tabIndents->setChecked(sci_->tabIndents());
	sci_->setTabIndents(ui.tabIndents->isChecked());
	//ui.indentationWidth->setValue(sci_->indentationWidth());
	sci_->setIndentationWidth(ui.indentationWidth->value());
	//ui.tabWIdth->setValue(sci_->tabWidth());
	sci_->setTabWidth(ui.tabWidth->value());

	//ui.foldingStyle->setCurrentIndex(sci_->folding());
	sci_->setFolding(static_cast<QsciScintilla::FoldStyle>(ui.foldingStyle->currentIndex()));

	//ui.edgeColumn->setValue(sci_->edgeColumn());
	sci_->setEdgeColumn(ui.edgeColumn->value());
	//ui.edgeType->setCurrentIndex(sci_->edgeMode());
	sci_->setEdgeMode(static_cast<QsciScintilla::EdgeMode>(ui.edgeType->currentIndex()));

	//ui.autocompletionCaseSensitive->setChecked(sci_->autoCompletionCaseSensitivity());
	sci_->setAutoCompletionCaseSensitivity(ui.autocompletionCaseSensitive->isChecked());
	//ui.autocompletionReplaceWord->setChecked(sci_->autoCompletionReplaceWord());
	sci_->setAutoCompletionReplaceWord(ui.autocompletionReplaceWord->isChecked());
	//ui.autocompletionShowSingle->setChecked(sci_->autoCompletionShowSingle());
	sci_->setAutoCompletionShowSingle(ui.autocompletionShowSingle->isChecked());
	//ui.autocompletionFillups->setChecked(sci_->autoCompletionFillupsEnabled());
	sci_->setAutoCompletionFillupsEnabled(ui.autocompletionFillups->isChecked());
	//ui.autocompletionTreshold->setValue(sci_->autoCompletionThreshold());
	sci_->setAutoCompletionThreshold(ui.autocompletionTreshold->value());
	//ui.autocompletionSource->setCurrentIndex(sci_->autoCompletionSource());
	sci_->setAutoCompletionSource(static_cast<QsciScintilla::AutoCompletionSource>(ui.autocompletionSource->currentIndex()));
	//ui.autocompletionUseSingle->setCurrentIndex(sci_->autoCompletionUseSingle());
	sci_->setAutoCompletionUseSingle(static_cast<QsciScintilla::AutoCompletionUseSingle>(ui.autocompletionUseSingle->currentIndex()));

	//ui.wrapMode->setCurrentIndex(sci_->wrapMode());
	sci_->setWrapMode(static_cast<QsciScintilla::WrapMode>(ui.wrapMode->currentIndex()));
	//ui.wrapIndentation->setCurrentIndex(sci_->wrapIndentMode());*/
	sci_->setWrapIndentMode(static_cast<QsciScintilla::WrapIndentMode>(ui.wrapIndentation->currentIndex()));
}

