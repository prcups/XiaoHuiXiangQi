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

#include "board.h"
#include "player.h"

Player::~Player() = default;

Player::Player(PieceColor color)
:playerColor(color), type(Human) {}

void Player::Go()
{
    if (!board) return;
    bar() << tr("就绪");
    board->SetMovable(true);
}

PieceColor Player::GetColor()
{
    return playerColor;
}

PlayerType Player::GetType()
{
    return type;
}

void Player::Pause()
{
    board->SetMovable(false);
}

void Player::SetBoard(Board* newBoard)
{
    board = newBoard;
}

UCIEngine::UCIEngine(PieceColor color, QString enginePath, int depth)
:Player(color), depth(depth)
{
    type = Computer;
    engineProcess = new QProcess();
    connect(engineProcess, &QProcess::readyRead, this, &UCIEngine::handleOutput);
    engineProcess->start(enginePath, QStringList());
    engineProcess->write("uci\n");
}

UCIEngine::~UCIEngine()
{
    engineProcess->kill();
    engineProcess->waitForFinished();
    delete engineProcess;
}

void UCIEngine::Go()
{
    if (!board) return;
    if (status != EnginePrepared)
    {
        deferGo = 1;
        return;
    }
    bar() << (playerColor == Red ? tr("红方") : tr("黑方")) + "正在思考...";
    status = EngineThinking;
    QString output = "position fen ";
    output.append(board->ToFenString());
    output.append("\ngo depth ");
    output.append(QString::number(depth));
    if (board->Draw) output.append(" draw");
    output.append("\n");
    engineProcess->write(output.toLocal8Bit());
    board->Draw = 0;
}

void UCIEngine::Pause()
{
    if (status == EngineThinking)
    {
        status = EnginePrepared;
        engineProcess->write("stop\n");
    }
}

void UCIEngine::handleOutput()
{
    QString output;
    switch (status)
    {
        case EngineCreated:
            while (1)
            {
                output = engineProcess->readLine();
                if (output.isEmpty()) break;
                if (output == "uciok\n")
                {
                    log() << (this->playerColor == Red ? QString("Red: ") : QString("Black: ")) << output;
                    status = EnginePrepared;
                    if (deferGo)
                    {
                        Go();
                        deferGo = 0;
                    }
                    break;
                }
            }
            break;
        case EnginePrepared:
            output = engineProcess->readAll();
            log() << (this->playerColor == Red ? QString("Red: ") : QString("Black: "));
            break;
        case EngineThinking:
            while (1)
            {
                output = engineProcess->readLine();
                if (output.isEmpty()) break;
                if (output.contains("bestmove"))
                {
                    log() << (this->playerColor == Red ? QString("Red: ") : QString("Black: "))
                    << output;
                    handleShortMoveString(output.remove(0, 9).chopped(1));
                    status = EnginePrepared;
                    break;
                }
            }
            break;
    }
}

void UCIEngine::handleShortMoveString(QString moveString)
{
    int fromX = moveString[1].toLatin1() - '0';
    int fromY = moveString[0].toLatin1() - 'a';
    int toX = moveString[3].toLatin1() - '0';
    int toY = moveString[2].toLatin1() - 'a';
    auto fromPiece = board->GetPiece(fromX, fromY);
    auto toPiece = board->GetPiece(toX, toY);
    if (fromPiece != nullptr && toPiece != nullptr && !fromPiece->Invalid
        && (toPiece->Invalid || fromPiece->GetColor() != toPiece->GetColor()))
            board->Move(fromX, fromY, toX, toY);
}

UCCIEngine::UCCIEngine(PieceColor color, QString enginePath, int depth)
:Player(color), depth(depth)
{
    type = Computer;
    engineProcess = new QProcess();
    connect(engineProcess, &QProcess::readyRead, this, &UCCIEngine::handleOutput);
    engineProcess->start(enginePath, QStringList());
    engineProcess->write("ucci\n");
}

UCCIEngine::~UCCIEngine()
{
    engineProcess->kill();
    engineProcess->waitForFinished();
    delete engineProcess;
}

void UCCIEngine::Go()
{
    if (!board) return;
    if (status != EnginePrepared)
    {
        deferGo = 1;
        return;
    }
    bar() << (playerColor == Red ? tr("红方") : tr("黑方")) + "正在思考...";
    status = EngineThinking;
    QString output = "position fen ";
    output.append(board->ToFenString());
    output.append("\ngo depth ");
    output.append(QString::number(depth));
    if (board->Draw) output.append(" draw");
    output.append("\n");
    engineProcess->write(output.toLocal8Bit());
    board->Draw = 0;
}

void UCCIEngine::Pause()
{
    if (status == EngineThinking)
    {
        status = EnginePrepared;
        engineProcess->write("stop\n");
    }
}

void UCCIEngine::handleOutput()
{
    QString output;
    switch (status)
    {
        case EngineCreated:
            while (1)
            {
                output = engineProcess->readLine();
                if (output.isEmpty()) break;
                if (output == "ucciok\n")
                {
                    log() << (this->playerColor == Red ? QString("Red: ") : QString("Black: ")) << output;
                    status = EnginePrepared;
                    if (deferGo)
                    {
                        Go();
                        deferGo = 0;
                    }
                    break;
                }
            }
            break;
        case EnginePrepared:
            output = engineProcess->readAll();
            log() << (this->playerColor == Red ? QString("Red: ") : QString("Black: "));
            break;
        case EngineThinking:
            while (1)
            {
                output = engineProcess->readLine();
                if (output.isEmpty()) break;
                if (output.contains("bestmove"))
                {
                    log() << (this->playerColor == Red ? QString("Red: ") : QString("Black: "))
                    << output;
                    handleShortMoveString(output.remove(0, 9).chopped(1));
                    status = EnginePrepared;
                    break;
                }
            }
            break;
    }
}

void UCCIEngine::handleShortMoveString(QString moveString)
{
    int fromX = moveString[1].toLatin1() - '0';
    int fromY = moveString[0].toLatin1() - 'a';
    int toX = moveString[3].toLatin1() - '0';
    int toY = moveString[2].toLatin1() - 'a';
    auto fromPiece = board->GetPiece(fromX, fromY);
    auto toPiece = board->GetPiece(toX, toY);
    if (fromPiece != nullptr && toPiece != nullptr && !fromPiece->Invalid
        && (toPiece->Invalid || fromPiece->GetColor() != toPiece->GetColor()))
            board->Move(fromX, fromY, toX, toY);
}
