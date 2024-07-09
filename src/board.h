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

#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QStringView>
#include <QPropertyAnimation>
#include "piece.h"
#include "player.h"

/**
 * @todo write docs
 */

class Board;

class BoardBackground : public QGraphicsItem
{
    float boardLeft, boardTop, boardBottom, boardRight,
          heightDis, widthDis, heightMargin, widthMargin;
    void drawSoldierPos(QPainter *painter, int xPos, int yPos);
    QRectF boundingRect() const override;
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) override;
    float getX(int xPos)
    {
        return boardLeft + xPos * widthDis;
    }
    float getY(int yPos)
    {
        return boardTop + yPos * heightDis;
    }
public:
    friend Board;
    BoardBackground();
};

class Player;

enum BoardStatus
{
    BannedOperation, Prepared, Selected
};

class Board : public QGraphicsScene
{
    Q_OBJECT
    Piece *content[10][9];
    BoardBackground* background;
    Player *player[2];
    int playerColor;
    BoardStatus status = BannedOperation;
    Piece *selectedPiece;
    int moveNumber;
    QGraphicsRectItem *focusFrame;

    void initBoard();
    bool putPieces(QStringView fenMain);
    void changePlayer();
    float getX(int xPos);
    float getY(int yPos);
    void mousePressEvent ( QGraphicsSceneMouseEvent * event ) override;
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) override;
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event ) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

public:
    /**
     * Default constructor
     */
    Board();
    Board(QString fen);
    ~Board() noexcept;
    void Move(Piece *from, Piece *to);
    void setMovable();
    QString ToFenString();
};

#endif // BOARD_H
