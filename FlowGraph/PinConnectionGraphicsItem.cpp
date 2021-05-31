#include "PinConnectionGraphicsItem.h"

ConnectionGraphicsItem::ConnectionGraphicsItem() :
    _fromPinItem(nullptr),
    _toPinItem(nullptr)
{
}

ConnectionGraphicsItem::ConnectionGraphicsItem( PinGraphicsItem * fromPin, PinGraphicsItem * toPin) :
    _fromPinItem(fromPin),
    _toPinItem(toPin)
{
}

QRectF ConnectionGraphicsItem::boundingRect() const
{
    return _fromPinItem->boundingRect().united(_toPinItem->boundingRect());
}

void ConnectionGraphicsItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->setRenderHint( QPainter::Antialiasing, true);

    QPointF fromPinPos = _fromPinItem->GetConnectionPosition() + QPointF( 0.0f, 0.0f);
    QPointF fromPinPosB = _fromPinItem->GetConnectionPosition() + QPointF( 150.0f, 0.0f);
    QPointF toPinPos = _toPinItem->GetConnectionPosition() + QPointF( 0.0f, 0.0f);
    QPointF toPinPosB = _toPinItem->GetConnectionPosition() - QPointF( 150.0f, 0.0f);

    QPainterPath connectionPath;
    connectionPath.moveTo(fromPinPos);
    connectionPath.cubicTo(fromPinPosB, toPinPosB, toPinPos);
    connectionPath.setFillRule(Qt::OddEvenFill);

    QBrush painterBrush = painter->brush();
    QColor typeColor = FlowData::TypeToColor(_fromPinItem->Pin()->Type());
    painterBrush.setColor(typeColor);
    painterBrush.setStyle(Qt::SolidPattern);

    // connection edge
    painter->setPen( QPen( QColor(0,0,0), 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawPath(connectionPath);

    // connection center line
    painter->setPen( QPen( typeColor, 3, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawPath(connectionPath);

    // debug
    /*
    painter->setPen( QPen( QColor(255,0,0), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawPoint(fromPinPos);
    painter->drawPoint(toPinPos);

    painter->setPen( QPen( QColor(0,255,0), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawPoint(fromPinPosB);
    painter->drawPoint(toPinPosB);
    */
}
