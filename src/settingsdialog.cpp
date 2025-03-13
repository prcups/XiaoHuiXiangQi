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
    QSettings settings;
    engineList = settings.value("list").toJsonArray();
    for (auto i : engineList)
    {
        auto engine = i.toObject();
        m_ui->engineListWidget->addItem(engine["path"].toString()
            + "(" + engine["protocol"].toString() + ")");
    }
    m_ui->rotate->setCheckState(
        settings.value("rotate", true).toBool() ? Qt::Checked : Qt::Unchecked);
    m_ui->animation->setCheckState(
        settings.value("animation", true).toBool() ? Qt::Checked : Qt::Unchecked);
    connect(m_ui->chooseFile, &QToolButton::clicked, this, &SettingsDialog::onChooseTriggered);
    connect(m_ui->addEngine, &QToolButton::clicked, this, &SettingsDialog::onAddTriggered);
    connect(m_ui->deleteEngine, &QToolButton::clicked, this, &SettingsDialog::onRemoveTriggered);
}

SettingsDialog::~SettingsDialog() {}

void SettingsDialog::onAddTriggered()
{

}

void SettingsDialog::onChooseTriggered()
{
    m_ui->enginePath->setText(
        QFileDialog::getOpenFileName(this, tr("选择引擎程序")));
}

void SettingsDialog::onRemoveTriggered()
{

}
