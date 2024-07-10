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

#include "player.h"

Player::~Player() = default;

Player::Player(Board *board, PieceColor color)
:playerColor(color), board(board) {}

void Player::Go()
{
    board->setMovable();
}

Engine::Engine(Board *board, PieceColor color, QString engineName)
:Player(board, color)
{
    engineProcess = new QProcess();
    connect(engineProcess, &QProcess::readyRead, this, &Engine::handleOutput);
    engineProcess->start(engineName, QStringList());
    engineProcess->write("uci\n");
}

Engine::~Engine()
{
    delete engineProcess;
}

void Engine::Go()
{
    if (status != EnginePrepared) return;
    status = EngineThinking;
}

void Engine::handleOutput()
{
    QString output;
    switch (status)
    {
        case EngineCreated:
            while (1)
            {
                output = engineProcess->readLine();
                qDebug()<<"test "<<output;
                if (output.isEmpty()) break;
                if (output == "uciok\n")
                {
                    status = EnginePrepared;
                    break;
                }
            }
            break;
        case EnginePrepared:
            break;
        case EngineThinking:
            break;
    }
}
