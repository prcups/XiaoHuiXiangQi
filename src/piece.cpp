#include "piece.h"

QRectF Piece::boundingRect() const
{
    return QRectF(0, 0, 90, 90);
}

void Piece::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if (Invalid) return;
    painter->setPen(Qt::white);
    QPoint hanziPos = {10, 72};
    QString hanzi;
    if (color == Red) {
        painter->setBrush(Qt::red);
        painter->drawEllipse(0, 0, 90, 90);
        switch (type) {
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
    } else {
        painter->setBrush(Qt::black);
        painter->drawEllipse(0, 0, 90, 90);
        switch (type) {
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
    : type(type), color(color), X(x), Y(y)
{
    setTransformOriginPoint(45, 45);
}

Piece::Piece(int x, int y)
    : X(x), Y(y)
{
    Invalid = 1;
    setTransformOriginPoint(45, 45);
}

Piece::Piece(const Piece &p)
{
    if (p.Invalid) Invalid = 1;
    else {
        type = p.type;
        color = p.color;
    }
    setTransformOriginPoint(45, 45);
}

Piece &Piece::operator=(const Piece &p)
{
    if (p.Invalid) Invalid = 1;
    else {
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

