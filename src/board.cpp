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

void Board::drawBackground(QPainter* painter, const QRectF& rect)
{
    painter->fillRect(rect, QColor("green"));
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

void Board::drawSoldierPos(QPainter* painter, int xPos, int yPos)
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

}
