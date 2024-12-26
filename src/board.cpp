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

QRectF BoardBackground::boundingRect() const
{
    return QRectF(0, 0, 960, 1120);
}

BoardBackground::BoardBackground()
{
    auto rect = boundingRect();
    if (rect.height() / 9 > rect.width() / 8)
    {
        widthMargin = 60;
        heightMargin = (rect.height() - rect.width() / 8 * 9) / 2 + 60;
    }else
    {
        heightMargin = 60;
        widthMargin = (rect.width() - rect.height() / 9 * 8) / 2 + 60;
    }
    boardLeft = rect.x() + widthMargin;
    boardTop = rect.y() + heightMargin;
    boardRight = rect.x() + rect.width() - widthMargin;
    boardBottom = rect.y() + rect.height() - heightMargin;
    heightDis = (boardBottom - boardTop) / 9;
    widthDis = (boardRight - boardLeft) / 8;
}

void BoardBackground::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setBrush(QColor(131, 203, 172, 127));
    painter->drawRect(boundingRect());
    painter->setBrush(Qt::transparent);

    painter->drawRect(boardLeft, boardTop, boardRight - boardLeft, boardBottom - boardTop);
    for (int i = 1; i < 9; ++i)
    {
        painter->drawLine(boardLeft, getY(i), boardRight, getY(i));
    }
    for (int i = 1; i < 8; ++i)
    {
        painter->drawLine(getX(i), boardTop, getX(i), getY(4));
        painter->drawLine(getX(i), getY(5), getX(i), boardBottom);
    }
    painter->drawLine(getX(3), boardTop, getX(5), getY(2));
    painter->drawLine(getX(5), boardTop, getX(3), getY(2));
    painter->drawLine(getX(3), boardBottom, getX(5), getY(7));
    painter->drawLine(getX(5), boardBottom, getX(3), getY(7));
    for (int i = 0; i < 5; ++i)
    {
        drawSoldierPos(painter, i * 2, 3);
        drawSoldierPos(painter, i * 2, 6);
    }
}

void BoardBackground::drawSoldierPos(QPainter* painter, int xPos, int yPos)
{
    if (xPos != 0)
    {
        painter->drawLine(getX(xPos) - 16, getY(yPos) - 6, getX(xPos) - 6, getY(yPos) - 6);
        painter->drawLine(getX(xPos) - 6, getY(yPos) - 6, getX(xPos) - 6, getY(yPos) - 16);
        painter->drawLine(getX(xPos) - 16, getY(yPos) + 6, getX(xPos) - 6, getY(yPos) + 6);
        painter->drawLine(getX(xPos) - 6, getY(yPos) + 6, getX(xPos) - 6, getY(yPos) + 16);
    }
    if (xPos != 8)
    {
        painter->drawLine(getX(xPos) + 16, getY(yPos) - 6, getX(xPos) + 6, getY(yPos) - 6);
        painter->drawLine(getX(xPos) + 6, getY(yPos) - 6, getX(xPos) + 6, getY(yPos) - 16);
        painter->drawLine(getX(xPos) + 16, getY(yPos) + 6, getX(xPos) + 6, getY(yPos) + 6);
        painter->drawLine(getX(xPos) + 6, getY(yPos) + 6, getX(xPos) + 6, getY(yPos) + 16);
    }
}

Board::Board(PlayerType playerType[])
{
    background = new BoardBackground;
    addItem(background);
    focusFrame = new QGraphicsRectItem(0, 0, 90, 90);
    initPieces(QString("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR"));
    for (int i = 0; i < 2; ++i)
    {
        if (playerType[i] == Human)
            player[i] = new Player(this, PieceColor(i));
        else player[i] = new Engine(this, PieceColor(i), "./pikafish");
    }
    playerColor = Red;
    moveNumber = 0;
    player[playerColor]->Go();
}

Board::~Board() noexcept
{
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 9; ++j)
            delete content[i][j];
    delete background;
    delete player[0];
    delete player[1];
}

void Board::execMoveOnBoard(int fromX, int fromY, int toX, int toY)
{
    removeItem(content[toX][toY]);
    delete content[toX][toY];
    content[toX][toY] = content[fromX][fromY];
    content[toX][toY]->X = toX;
    content[toX][toY]->Y = toY;
    content[fromX][fromY] = new Piece(fromX, fromY);
    addItem(content[fromX][fromY]);
    content[fromX][fromY]->setPos(getX(fromY), getY(fromX));
    content[fromX][fromY]->X = fromX;
    content[fromX][fromY]->Y = fromY;
    QPropertyAnimation *animation = new QPropertyAnimation(content[toX][toY], "pos");
    animation->setDuration(100);
    animation->setEndValue(QPointF(getX(toY), getY(toX)));
    animation->start();
}

bool Board::initPieces(QStringView fenMain)
{
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 9; ++j)
            content[i][j] = nullptr;

    int line = 9, column = 0;
    for (QChar t : fenMain)
    {
        if (line < 0) return false;
        switch (t.unicode())
        {
            case 'r':
            case 'R':
                if (column > 8) return false;
                content[line][column] = new Piece(Che, t.isLower() ? Black : Red, line, column);
                content[line][column]->setPos(getX(column), getY(line));
                addItem(content[line][column]);
                ++column;
                break;
            case 'n':
            case 'N':
                if (column > 8) return false;
                content[line][column] = new Piece(Ma, t.isLower() ? Black : Red, line, column);
                content[line][column]->setPos(getX(column), getY(line));
                addItem(content[line][column]);
                ++column;
                break;
            case 'b':
            case 'B':
                if (column > 8) return false;
                content[line][column] = new Piece(Xiang, t.isLower() ? Black : Red, line, column);
                content[line][column]->setPos(getX(column), getY(line));
                addItem(content[line][column]);
                ++column;
                break;
            case 'a':
            case 'A':
                if (column > 8) return false;
                content[line][column] = new Piece(Shi, t.isLower() ? Black : Red, line, column);
                content[line][column]->setPos(getX(column), getY(line));
                addItem(content[line][column]);
                ++column;
                break;
            case 'k':
            case 'K':
                if (column > 8) return false;
                content[line][column] = new Piece(Jiang, t.isLower() ? Black : Red, line, column);
                content[line][column]->setPos(getX(column), getY(line));
                addItem(content[line][column]);
                ++column;
                break;
            case 'c':
            case 'C':
                if (column > 8) return false;
                content[line][column] = new Piece(Pao, t.isLower() ? Black : Red, line, column);
                content[line][column]->setPos(getX(column), getY(line));
                addItem(content[line][column]);
                ++column;
                break;
            case 'p':
            case 'P':
                if (column > 8) return false;
                content[line][column] = new Piece(Zu, t.isLower() ? Black : Red, line, column);
                content[line][column]->setPos(getX(column), getY(line));
                addItem(content[line][column]);
                ++column;
                break;
            case '/':
                if (column != 9) return false;
                column = 0;
                --line;
                break;
            case '0'...'9':
                for (int i = t.unicode() - '0'; i > 0; --i)
                {
                    if (column > 8) return false;
                    content[line][column] = new Piece(line, column);
                    content[line][column]->setPos(getX(column), getY(line));
                    addItem(content[line][column]);
                    ++column;
                }
                break;
            default:
                return false;
        }
    }
    if (line != -1) return false;
    return true;
}

QString Board::ToFenString()
{
    QString fen;
    short invalidCount = 0;
    for (int i = 9; i >= 0; --i)
        for (int j = 0; j < 9; ++j)
        {
            if (content[i][j]->Invalid)
                ++invalidCount;
            else
            {
                if (invalidCount != 0) fen.append(QString::number(invalidCount));
                invalidCount = 0;
                if (content[i][j]->GetColor() == Black)
                {
                    switch (content[i][j]->GetType())
                    {
                        case Che:
                            fen.append('r');
                            break;
                        case Pao:
                            fen.append('c');
                            break;
                        case Ma:
                            fen.append('n');
                            break;
                        case Zu:
                            fen.append('p');
                            break;
                        case Xiang:
                            fen.append('b');
                            break;
                        case Shi:
                            fen.append('a');
                            break;
                        case Jiang:
                            fen.append('k');
                    }
                }
                else
                {
                    switch (content[i][j]->GetType())
                    {
                        case Che:
                            fen.append('R');
                            break;
                        case Pao:
                            fen.append('C');
                            break;
                        case Ma:
                            fen.append('N');
                            break;
                        case Zu:
                            fen.append('P');
                            break;
                        case Xiang:
                            fen.append('B');
                            break;
                        case Shi:
                            fen.append('A');
                            break;
                        case Jiang:
                            fen.append('K');
                    }
                }
            }
            if (j == 8)
            {
                if (invalidCount != 0) fen.append(QString::number(invalidCount));
                invalidCount = 0;
                if (i != 0) fen.append('/');
            }
        }
    if (playerColor == Black) fen.append(" b");
    else fen.append(" w");
    fen.append(" - - 0 ");
    fen.append(QString::number(moveNumber));
    return fen;
}

float Board::getY ( int yPos )
{
    return background->getY ( 9 - yPos ) - 45;
}

float Board::getX ( int xPos )
{
    return background->getX ( xPos ) - 45;
}

void Board::changePlayer()
{
    playerColor ^= 1;
    player[playerColor]->Go();
}

void Board::SetMovable()
{
    status = BoardPrepared;
}


void Board::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (status == BoardBanned) return;
    auto pos = event->lastScenePos();
    if (!items(pos).isEmpty())
    {
        auto clickedPiece =  dynamic_cast<Piece*>(items(pos).first());
        if (clickedPiece == nullptr || clickedPiece->Invalid || clickedPiece->GetColor() != playerColor) return;
        selectedPiece = clickedPiece;
        status = PieceSelected;
        focusFrame->setPos(getX(selectedPiece->Y), getY(selectedPiece->X));
        if (focusFrame->scene() != this)
            addItem(focusFrame);
    }
}

void Board::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
        .length() < QApplication::startDragDistance()) {
        return;
    }

    if (status != PieceSelected) return;
    auto pos = event->lastScenePos();
    if (selectedPiece->contains(selectedPiece->mapFromScene(pos)))
    {
        QDrag *drag = new QDrag(event->widget());
        QMimeData *mime = new QMimeData;
        mime->setText("xhxq");
        drag->setMimeData(mime);
        drag->setPixmap(selectedPiece->GetPixmap());
        drag->exec();
    }
}

void Board::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    if (event->mimeData()->text() != "xhxq") return;
}

void Board::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    auto pos = event->scenePos();
    handlePutEvent(pos);
}

void Board::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    auto pos = event->lastScenePos();
    handlePutEvent(pos);
}

void Board::handlePutEvent(QPointF & pos)
{
    if (status != PieceSelected) return;
    if (!items(pos).isEmpty())
    {
        auto clickedPiece =  dynamic_cast<Piece*>(items(pos).first());
        if (clickedPiece == nullptr
            || (clickedPiece->Invalid == 0
                && clickedPiece->GetColor() == selectedPiece->GetColor())) return;
        Move(selectedPiece->X, selectedPiece->Y, clickedPiece->X, clickedPiece->Y);
        removeItem(focusFrame);
    }
}

void Board::Move(int fromX, int fromY, int toX, int toY)
{
    status = BoardBanned;
    ++moveNumber;
    execMoveOnBoard(fromX, fromY, toX, toY);
    QTimer::singleShot(100, this, &Board::changePlayer);
}
