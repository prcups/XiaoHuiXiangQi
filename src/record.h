#ifndef RECORD_H
#define RECORD_H

#include "piece.h"
#include "player.h"

enum RecordType
{
    GameStart, NormalMove, EndResign, EndDraw
};

enum EndType
{
    NotEnd = 0, RedWin, Draw, BlackWin
};

struct Record
{
    RecordType type;
    int fromX, fromY, toX, toY;
    PieceColor dstColor;
    PieceType dstType;
    EndType endType;
    bool isBlack;
    bool ifJiangjun;
    bool ifEat;
    bool requestDraw;
    int lastEat;
    QString fenStr;

    bool operator == (const Record & x)const;
    QString ToMoveString();
};

uint qHash(const Record & key);

#endif // RECORD_H
