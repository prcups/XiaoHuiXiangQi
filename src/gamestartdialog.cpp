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

#include "gamestartdialog.h"
#include "ui_gamestartdialog.h"

GameStartDialog::GameStartDialog()
    : m_ui(new Ui::GameStartDialog), v(new QIntValidator(1, 50, this))
{
    m_ui->setupUi(this);
    connect(m_ui->RedPlayer, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GameStartDialog::handleRedPlayerChanged);
    connect(m_ui->BlackPlayer, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GameStartDialog::handleBlackPlayerChanged);
    connect(m_ui->RedDiff, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GameStartDialog::handleRedDiffChanged);
    connect(m_ui->BlackDiff, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GameStartDialog::handleBlackDiffChanged);
    connect(m_ui->RedDepth, &QLineEdit::textChanged, this, &GameStartDialog::handleDepthChanged);
    connect(m_ui->BlackDepth, &QLineEdit::textChanged, this, &GameStartDialog::handleDepthChanged);
}

int GameStartDialog::GetPlayerSelection(PieceColor color)
{
    if (color == Red) return m_ui->RedPlayer->currentIndex();
    else return m_ui->BlackPlayer->currentIndex();
}

int GameStartDialog::GetPlayerDepthSelection(PieceColor color)
{
    if (color == Red) return m_ui->RedDepth->text().toInt();
    else return m_ui->BlackDepth->text().toInt();
}

int GameStartDialog::GetPlayerDiffSelection(PieceColor color)
{
    if (color == Red) return m_ui->RedDiff->currentIndex();
    else return m_ui->BlackDiff->currentIndex();
}

void GameStartDialog::handleRedPlayerChanged(int index)
{
    m_ui->RedDiff->setEnabled(index != 0);
    m_ui->RedDiffLabel->setEnabled(index != 0);
}

void GameStartDialog::handleBlackPlayerChanged(int index)
{
    m_ui->BlackDiff->setEnabled(index != 0);
    m_ui->BlackDiffLabel->setEnabled(index != 0);
}

void GameStartDialog::handleRedDiffChanged(int index)
{
    m_ui->RedDepth->setEnabled(index == 2);
    m_ui->RedDepthLabel->setEnabled(index == 2);
}

void GameStartDialog::handleBlackDiffChanged(int index)
{
    m_ui->BlackDepth->setEnabled(index == 2);
    m_ui->BlackDepthLabel->setEnabled(index == 2);
}

void GameStartDialog::handleDepthChanged()
{
    auto bs = m_ui->BlackDepth->text();
    auto rs = m_ui->RedDepth->text();
    auto pos = 0;
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
        v->validate(bs, pos) == QValidator::Acceptable
        && v->validate(rs, pos) == QValidator::Acceptable
    );
}

GameStartDialog::~GameStartDialog() {}

