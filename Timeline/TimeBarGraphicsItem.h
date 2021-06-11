#ifndef TIMEBARGRAPHICSITEM_H
#define TIMEBARGRAPHICSITEM_H

#include "Timeline.h"

#include <QGraphicsItem>

class TimeBarGraphicsItem : public QGraphicsItem
{
public:
    TimeBarGraphicsItem(Timeline * timeline);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

    void SetTime(float time);
    void SetSelectionTime(float time);
    std::pair<float,float> GetSelectionBeginAndEndTimes();

private:
    Timeline * _timeline;

    float _selectionBeginTimeMs;
    float _selectionEndTimeMs;
    float _cursorTimeMs;
};

#endif // TIMEBARGRAPHICSITEM_H
