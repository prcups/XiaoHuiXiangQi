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

#include "piece.h"

QRectF Piece::boundingRect() const
{
    return QRectF(0, 0, 90, 90);
}

void Piece::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if (Invalid) return;
    painter->setPen(Qt::white);
    QPoint hanziPos = {10, 72};
    QString hanzi;
    if (color == Red)
    {
        painter->setBrush(Qt::red);
        painter->drawEllipse(0, 0, 90, 90);
        switch (type)
        {
            case PieceType::Jiang:
                hanzi = "帥";
                break;
            case PieceType::Shi:
                hanzi = "仕";
                break;
            case PieceType::Xiang:
                hanzi = "相";
                break;
            case PieceType::Ma:
                hanzi = "馬";
                break;
            case PieceType::Che:
                hanzi = "車";
                break;
            case PieceType::Zu:
                hanzi = "兵";
                break;
            case PieceType::Pao:
                hanzi = "砲";
        }
    }
    else
    {
        painter->setBrush(Qt::black);
        painter->drawEllipse(0, 0, 90, 90);
        switch (type)
        {
            case PieceType::Jiang:
                hanzi = "將";
                break;
            case PieceType::Shi:
                hanzi = "士";
                break;
            case PieceType::Xiang:
                hanzi = "象";
                break;
            case PieceType::Ma:
                hanzi = "马";
                break;
            case PieceType::Che:
                hanzi = "车";
                break;
            case PieceType::Zu:
                hanzi = "卒";
                break;
            case PieceType::Pao:
                hanzi = "炮";
        }
    }
    QFont font = painter->font();
    font.setPixelSize(72);
    painter->setFont(font);
    painter->drawText(hanziPos, hanzi);
}

Piece::Piece(PieceType type, PieceColor color, int x, int y)
:type(type), color(color), X(x), Y(y) {}

Piece::Piece(int x, int y)
:X(x), Y(y)
{
    Invalid = 1;
}

Piece::Piece(const Piece& p)
{
    if (p.Invalid) Invalid = 1;
    else
    {
        type = p.type;
        color = p.color;
    }
}

Piece & Piece::operator=(const Piece& p)
{
    if (p.Invalid) Invalid = 1;
    else
    {
        type = p.type;
        color = p.color;
    }
    return *this;
}

PieceColor Piece::GetColor()
{
    return color;
}

PieceType Piece::GetType()
{
    return type;
}

QPixmap Piece::GetPixmap()
{
    QPixmap pixmap(boundingRect().size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    paint(&painter, nullptr, nullptr);
    return pixmap;
}

