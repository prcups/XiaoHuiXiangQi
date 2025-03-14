// The contents of this file are subject to the Mozilla Public License
// Version 1.1 (the "License"); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://www.mozilla.org/MPL/
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
// License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code is ______________________________________.
// 
// The Initial Developer of the Original Code is ________________________.
// Portions created by ______________________ are Copyright (C) ______
// _______________________. All Rights Reserved.
// 
// Contributor(s): ______________________________________.
// 
// Alternatively, the contents of this file may be used under the terms
// of the _____ license (the  "[___] License"), in which case the
// provisions of [______] License are applicable instead of those
// above.  If you wish to allow use of your version of this file only
// under the terms of the [____] License and not to allow others to use
// your version of this file under the MPL, indicate your decision by
// deleting  the provisions above and replace  them with the notice and
// other provisions required by the [___] License.  If you do not delete
// the provisions above, a recipient may use your version of this file
// under either the MPL or the [___] License."

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog()
:m_ui(new Ui::SettingsDialog)
{
    m_ui->setupUi(this);
    m_ui->buttonBox->button(QDialogButtonBox::Apply)->setDisabled(true);
    m_ui->engineListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    QSettings settings;
    engineList = settings.value("list").toJsonArray();
    for (auto i : engineList)
    {
        auto engine = i.toObject();
        m_ui->engineListWidget->addItem(engine["name"].toString() + "("
            + engine["path"].toString() + ","
           + (engine["protocol"].toInt() ? QString("UCCI") : QString("UCI")) + ")");
    }
    m_ui->rotate->setChecked(settings.value("rotate", true).toBool());
    m_ui->animation->setChecked(settings.value("animation", true).toBool());

    connect(m_ui->chooseFile, &QToolButton::clicked, this, &SettingsDialog::onChooseTriggered);
    connect(m_ui->addEngine, &QToolButton::clicked, this, &SettingsDialog::onAddTriggered);
    connect(m_ui->deleteEngine, &QToolButton::clicked, this, &SettingsDialog::onRemoveTriggered);
    connect(m_ui->buttonBox->button(QDialogButtonBox::Apply),
            &QPushButton::clicked, this, &SettingsDialog::onApplyTriggered);
    connect(this, &SettingsDialog::accepted, this, &SettingsDialog::onApplyTriggered);
    connect(m_ui->rotate, &QCheckBox::stateChanged, this, &SettingsDialog::onApplyEnabled);
    connect(m_ui->animation, &QCheckBox::stateChanged, this, &SettingsDialog::onApplyEnabled);
}

SettingsDialog::~SettingsDialog() {}

void SettingsDialog::onAddTriggered()
{
    QJsonObject engine;
    auto name = m_ui->engineName->text();
    auto path = m_ui->enginePath->text();
    auto protocol = m_ui->engineType->currentIndex();
    if (!QFile::exists(path)) return;
    engine["name"] = name;
    engine["path"] = path;
    engine["protocol"] = protocol;
    engineList.append(engine);
    m_ui->engineListWidget->addItem(name + "(" + path + ","
           + (protocol ? QString("UCCI") : QString("UCI")) + ")");
    m_ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void SettingsDialog::onChooseTriggered()
{
    m_ui->enginePath->setText(
        QFileDialog::getOpenFileName(this, tr("选择引擎程序")));
}

void SettingsDialog::onRemoveTriggered()
{
    auto index = m_ui->engineListWidget->currentRow();
    if (index == -1) return;
    engineList.removeAt(index);
    auto item = m_ui->engineListWidget->takeItem(index);
    if (item) delete item;
    m_ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void SettingsDialog::onApplyTriggered()
{
    QSettings settings;
    settings.setValue("list", engineList);
    settings.setValue("rotate", m_ui->rotate->isChecked());
    settings.setValue("animation", m_ui->animation->isChecked());
    m_ui->buttonBox->button(QDialogButtonBox::Apply)->setDisabled(true);
}

void SettingsDialog::onApplyEnabled()
{
    m_ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}
