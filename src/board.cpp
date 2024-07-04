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
    return  QRectF(0, 0, 960, 1280);
}

BoardBackground::BoardBackground()
{
    auto rect = boundingRect();
    if (rect.height() / 9 > rect.width() / 8)
    {
        widthMargin = 20;
        heightMargin = (rect.height() - rect.width() / 8 * 9) / 2 + 20;
    }else
    {
        heightMargin = 20;
        widthMargin = (rect.width() - rect.height() / 9 * 8) / 2 + 20;
    }
    boardLeft = rect.x() + widthMargin;
    boardTop = rect.y() + heightMargin;
    boardRight = rect.x() + rect.width() - widthMargin;
    boardBottom = rect.y() + rect.height() - heightMargin;
    lineDis = (boardBottom - boardTop) / 9;
}

void BoardBackground::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

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

Board::Board()
{
    background = new BoardBackground;
    addItem(background);
    putPieces();
    focusFrame = new QGraphicsRectItem(0, 0, 90, 90);
}

void Board::putPieces()
{
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 9; ++j)
            content[i][j] = nullptr;

    content[0][0] = new Piece(PieceType::Che, playerColor, 0, 0);
    content[0][1] = new Piece(PieceType::Ma, playerColor, 0, 1);
    content[0][2] = new Piece(PieceType::Xiang, playerColor, 0, 2);
    content[0][3] = new Piece(PieceType::Shi, playerColor, 0, 3);
    content[0][4] = new Piece(PieceType::Jiang, playerColor, 0, 4);
    content[0][5] = new Piece(PieceType::Shi, playerColor, 0, 5);
    content[0][6] = new Piece(PieceType::Xiang, playerColor, 0, 6);
    content[0][7] = new Piece(PieceType::Ma, playerColor, 0, 7);
    content[0][8] = new Piece(PieceType::Che, playerColor, 0, 8);
    content[2][1] = new Piece(PieceType::Pao, playerColor, 2, 1);
    content[2][7] = new Piece(PieceType::Pao, playerColor, 2, 7);
    content[3][0] = new Piece(PieceType::Zu, playerColor, 3, 0);
    content[3][2] = new Piece(PieceType::Zu, playerColor, 3, 2);
    content[3][4] = new Piece(PieceType::Zu, playerColor, 3, 4);
    content[3][6] = new Piece(PieceType::Zu, playerColor, 3, 6);
    content[3][8] = new Piece(PieceType::Zu, playerColor, 3, 8);

    PieceColor rivalColor = PieceColor(~playerColor);
    content[9][0] = new Piece(PieceType::Che, rivalColor, 9, 0);
    content[9][1] = new Piece(PieceType::Ma, rivalColor, 9, 1);
    content[9][2] = new Piece(PieceType::Xiang, rivalColor, 9, 2);
    content[9][3] = new Piece(PieceType::Shi, rivalColor, 9, 3);
    content[9][4] = new Piece(PieceType::Jiang, rivalColor, 9, 4);
    content[9][5] = new Piece(PieceType::Shi, rivalColor, 9, 5);
    content[9][6] = new Piece(PieceType::Xiang, rivalColor, 9, 6);
    content[9][7] = new Piece(PieceType::Ma, rivalColor, 9, 7);
    content[9][8] = new Piece(PieceType::Che, rivalColor, 9, 8);
    content[7][1] = new Piece(PieceType::Pao, rivalColor, 7, 1);
    content[7][7] = new Piece(PieceType::Pao, rivalColor, 7, 7);
    content[6][0] = new Piece(PieceType::Zu, rivalColor, 6, 0);
    content[6][2] = new Piece(PieceType::Zu, rivalColor, 6, 2);
    content[6][4] = new Piece(PieceType::Zu, rivalColor, 6, 4);
    content[6][6] = new Piece(PieceType::Zu, rivalColor, 6, 6);
    content[6][8] = new Piece(PieceType::Zu, rivalColor, 6, 8);

    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 9; ++j)
        {
            if (content[i][j] == nullptr) content[i][j] = new Piece(i, j);
            content[i][j]->setPos(getX(j), getY(i));
            addItem(content[i][j]);
        }
}

Board::~Board() noexcept
{
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 9; ++j)
            delete content[i][j];
    delete background;
}

void Board::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    auto pos = event->lastScenePos();
    if (!items(pos).isEmpty())
    {
        auto clickedPiece =  dynamic_cast<Piece*>(items(pos).first());
        if (clickedPiece == nullptr || clickedPiece->Invalid || clickedPiece->GetColor() != playerColor) return;
        selectedPiece = clickedPiece;
        selectedMode = 1;
        focusFrame->setPos(getX(selectedPiece->Y), getY(selectedPiece->X));
        sendEvent(clickedPiece, event);
        if (focusFrame->scene() != this)
            addItem(focusFrame);
    }
}

void Board::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (selectedMode == 0) return;
    auto pos = event->lastScenePos();
    if (!items(pos).isEmpty())
    {
        auto clickedPiece =  dynamic_cast<Piece*>(items(pos).first());
        if (clickedPiece == nullptr
            || (clickedPiece->Invalid == 0
            && clickedPiece->GetColor() == selectedPiece->GetColor())) return;
        Move(selectedPiece, clickedPiece);
        removeItem(focusFrame);
        selectedMode = 0;
    }
}

void Board::Move(Piece* from, Piece* to)
{
    auto toX = to->X, toY = to->Y;
    removeItem(to);
    delete content[toX][toY];
    content[toX][toY] = from;
    content[from->X][from->Y] = new Piece(from->X, from->Y);
    addItem(content[from->X][from->Y]);
    content[from->X][from->Y]->setPos(getX(from->Y), getY(from->X));
    from->setPos(getX(toY), getY(toX));
    from->X = toX;
    from->Y = toY;
}
