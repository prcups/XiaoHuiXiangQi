#ifndef PIECE_H
#define PIECE_H

#include <QGraphicsObject>
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>

enum PieceType
{
    Jiang, Shi, Xiang, Ma, Che, Zu, Pao
};

enum PieceColor
{
    Red = 0, Black = 1
};

/**
 * @todo write docs
 */
class Piece : public QGraphicsObject
{
    PieceType type;
    PieceColor color;
    QRectF boundingRect() const override;
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) override;
public:
    Piece(PieceType type, PieceColor color, int x, int y);
    Piece(int x, int y);
    Piece(const Piece &p);
    QPixmap GetPixmap();
    Piece & operator = (const Piece &p);
    PieceType GetType();
    PieceColor GetColor();
    bool Invalid = 0;
    int X, Y;
};

#endif // PIECE_H
