#include "board.h"
#include "player.h"

Player::~Player() = default;

Player::Player(PieceColor color)
    : playerColor(color), type(Human) {}

void Player::Go()
{
    if (!board) return;
    if (board->GetDraw())
        bar() << tr("对方求和，如同意请点击求和/接受求和选项");
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

void Player::SetBoard(Board *newBoard)
{
    board = newBoard;
}

UCIEngine::UCIEngine(PieceColor color, QString enginePath, int depth)
    : Player(color), depth(depth)
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
    if (status != EnginePrepared) {
        deferGo = 1;
        return;
    }
    bar() << (playerColor == Red ? tr("红方") : tr("黑方")) + "正在思考...";
    status = EngineThinking;
    QString output = "position fen ";
    output.append(board->ToFenString());
    output.append("\ngo depth ");
    output.append(QString::number(depth));
    if (board->GetDraw()) output.append(" draw");
    output.append("\n");
    engineProcess->write(output.toLocal8Bit());
}

void UCIEngine::Pause()
{
    if (status == EngineThinking) {
        status = EnginePrepared;
        engineProcess->write("stop\n");
    }
}

void UCIEngine::handleOutput()
{
    QString output;
    switch (status) {
    case EngineCreated:
        while (1) {
            output = engineProcess->readLine();
            if (output.isEmpty()) break;
            log() << (this->playerColor == Red ? QString("Red: ") : QString("Black: ")) << output;
            if (output == "uciok\n") {
                status = EnginePrepared;
                if (deferGo) {
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
        while (1) {
            output = engineProcess->readLine();
            if (output.isEmpty()) break;
            log() << (this->playerColor == Red ? QString("Red: ") : QString("Black: "))
                  << output;
            if (output.leftRef(8) == "bestmove") {
                auto bestmoveList = output.splitRef(QRegExp("[ \n]"));
                if (bestmoveList.size() > 1) {
                    if (bestmoveList[1] == "(none)") {
                        board->Resign();
                        return;
                    }
                    for (int i = 0; i < bestmoveList.size(); ++i) {
                        if (bestmoveList[i] == "draw" && board->RequestDraw()) return;
                        if (bestmoveList[i] == "resign") {
                            board->Resign();
                            return;
                        }
                    }
                    handleShortMoveString(bestmoveList[1].toString());
                }
                status = EnginePrepared;
                break;
            }
        }
        break;
    }
}

void UCIEngine::handleShortMoveString(const QString &moveString)
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
    : Player(color), depth(depth)
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
    if (status != EnginePrepared) {
        deferGo = 1;
        return;
    }
    bar() << (playerColor == Red ? tr("红方") : tr("黑方")) + "正在思考...";
    status = EngineThinking;
    QString output = "position fen ";
    output.append(board->ToFenString());
    output.append("\ngo depth ");
    output.append(QString::number(depth));
    if (board->GetDraw()) output.append(" draw");
    output.append("\n");
    engineProcess->write(output.toLocal8Bit());
}

void UCCIEngine::Pause()
{
    if (status == EngineThinking) {
        status = EnginePrepared;
        engineProcess->write("stop\n");
    }
}

void UCCIEngine::handleOutput()
{
    QString output;
    switch (status) {
    case EngineCreated:
        while (1) {
            output = engineProcess->readLine();
            if (output.isEmpty()) break;
            log() << (this->playerColor == Red ? QString("Red: ") : QString("Black: ")) << output;
            if (output == "ucciok\n") {
                status = EnginePrepared;
                if (deferGo) {
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
        while (1) {
            output = engineProcess->readLine();
            if (output.isEmpty()) break;
            log() << (this->playerColor == Red ? QString("Red: ") : QString("Black: "))
                  << output;
            if (output.leftRef(10) == "nobestmove") {
                board->Resign();
                return;
            }
            if (output.leftRef(8) == "bestmove") {
                auto bestmoveList = output.splitRef(QRegExp("[ \n]"));
                if (bestmoveList.size() > 1) {
                    for (int i = 0; i < bestmoveList.size(); ++i) {
                        if (bestmoveList[i] == "draw" && board->RequestDraw()) return;
                        if (bestmoveList[i] == "resign") {
                            board->Resign();
                            return;
                        }
                    }
                    handleShortMoveString(bestmoveList[1].toString());
                }
                status = EnginePrepared;
                break;
            }
        }
        break;
    }
}

void UCCIEngine::handleShortMoveString(const QString &moveString)
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
