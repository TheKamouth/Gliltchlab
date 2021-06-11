#include "TimeBarGraphicsItem.h"

#include "TimelineConstants.h"

#include "ColorPalette.h"

#include <QPainter>
#include <QPen>
#include <QWidget>
#include <QDebug>

TimeBarGraphicsItem::TimeBarGraphicsItem(Timeline * timeline) :
    _timeline(timeline),
    _selectionBeginTimeMs(0.0f),
    _selectionEndTimeMs(1000.0f),
    _cursorTimeMs(0.0f)
{}

QRectF TimeBarGraphicsItem::boundingRect() const
{
    QRect topBarRect = QRect(0,
                             0,
                             TIMELINE_TRACK_MAX_DURATION_MS + TIMELINE_TRACK_INFO_WIDTH,
                             TIMELINE_UPPER_RULE_HEIGHT);
    return topBarRect;
}

void TimeBarGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    // Background
    QRect topBarRect = QRect(0,
                             0,
                             TIMELINE_TRACK_MAX_DURATION_MS + TIMELINE_TRACK_INFO_WIDTH,
                             TIMELINE_UPPER_RULE_HEIGHT);

    QBrush brush = painter->brush();
    brush.setColor(LIGHT_GREY);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    painter->drawRect(topBarRect);

    // Selection
    QRect selectionRect = QRect(TIMELINE_TRACK_INFO_WIDTH + _selectionBeginTimeMs,
                                0,
                                _selectionEndTimeMs - _selectionBeginTimeMs,
                                TIMELINE_UPPER_RULE_HEIGHT);

    brush.setColor(RED);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    painter->drawRect(selectionRect);

    // Draw grid
    float minX = 0;
    float maxX = 10000.0f;
    float minY = 0.0f;
    float maxY = TIMELINE_UPPER_RULE_HEIGHT;
    float interval = 20.0f;

    //setForegroundBrush(QColor(5, 21, 21, 255));

    QPen linePen = QPen();
    linePen.setWidth(1);
    linePen.setColor(BLACK);

    //painter->drawText(QPointF(0.0f, 0.0f), "0");

    float currentX = minX;
    do
    {
        float timeMs = currentX;
        float timeSec = timeMs / 1000.0f;

        if(currentX == 1000.0f)
        {
            linePen.setWidth(4);
            linePen.setColor(BLACK);
        }
        else if((int)currentX % 1000 == 0)
        {
            linePen.setWidth(2);
            linePen.setColor(BLACK);

            QString timeSecString = QString::number(timeSec, 'g', 2);
            int charCount = timeSecString.count();
            float textRectWidth = charCount * 16.0f;

            QRect timeValueTextRect = QRect(TIMELINE_TRACK_INFO_WIDTH + currentX - textRectWidth / 2.0f,
                                            0.0f,
                                            textRectWidth,
                                            TIMELINE_UPPER_RULE_HEIGHT);

            painter->drawText(timeValueTextRect, timeSecString, Qt::AlignVCenter | Qt::AlignHCenter);
            painter->drawLine(TIMELINE_TRACK_INFO_WIDTH + currentX, minY, TIMELINE_TRACK_INFO_WIDTH + currentX, maxY);
        }
        else if((int)currentX % 100 == 0)
        {
            linePen.setWidth(1);
            linePen.setColor(BLACK);

            QString timeSecString = QString::number(timeSec, 'g', 2);
            int charCount = timeSecString.count();
            float textRectWidth = charCount * 8.0f;

            QRect timeValueTextRect = QRect(TIMELINE_TRACK_INFO_WIDTH + currentX - textRectWidth / 2.0f,
                                            0.0f,
                                            textRectWidth,
                                            TIMELINE_UPPER_RULE_HEIGHT);

            painter->drawText(timeValueTextRect, timeSecString, Qt::AlignVCenter | Qt::AlignHCenter);
            painter->drawLine(TIMELINE_TRACK_INFO_WIDTH + currentX, maxY / 2.0f, TIMELINE_TRACK_INFO_WIDTH + currentX, maxY);
        }
        else
        {
            linePen.setWidth(1);
            linePen.setColor(GREY);

            QString timeSecString = QString::number(timeSec, 'g', 2);
            int charCount = timeSecString.count();
            float textRectWidth = charCount * 8.0f;

            QRect timeValueTextRect = QRect(TIMELINE_TRACK_INFO_WIDTH + currentX - textRectWidth / 2.0f,
                                            0.0f,
                                            textRectWidth,
                                            TIMELINE_UPPER_RULE_HEIGHT);

            painter->drawLine(TIMELINE_TRACK_INFO_WIDTH + currentX, 3.0 * maxY / 4.0f, TIMELINE_TRACK_INFO_WIDTH +currentX, maxY);
        }

        currentX += interval;

    }while(currentX <= maxX);

    // Cursor rect
    QRect cursorRect = QRect(TIMELINE_TRACK_INFO_WIDTH + _cursorTimeMs,
                                0,
                                5,
                                TIMELINE_UPPER_RULE_HEIGHT);

    brush.setColor(BLACK);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    painter->drawRect(cursorRect);
}

void TimeBarGraphicsItem::SetTime(float time)
{
    _cursorTimeMs = time;

    update();
}

void TimeBarGraphicsItem::SetSelectionTime(float time)
{
    if (time < 0.0f || time > TIMELINE_TRACK_MAX_DURATION_MS)
    {
        qDebug() << __FUNCTION__ << " incorrect time passed: " << time;
        return;
    }

    float timeToCurrentBegin = std::abs(time - _selectionBeginTimeMs);
    float timeToCurrentEnd = std::abs(time - _selectionEndTimeMs);

    if (time <_selectionBeginTimeMs
            || timeToCurrentBegin < timeToCurrentEnd)
    {
        _selectionBeginTimeMs = time;
    }
    else if (time > _selectionEndTimeMs
             || timeToCurrentBegin > timeToCurrentEnd)
    {
        _selectionEndTimeMs = time;
    }

    update();
}

std::pair<float, float> TimeBarGraphicsItem::GetSelectionBeginAndEndTimes()
{
    return std::pair<float,float>(_selectionBeginTimeMs,_selectionEndTimeMs);
}
