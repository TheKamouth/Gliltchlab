#include "TimeBarGraphicsItem.h"

#include "TimelineConstants.h"
#include "TimeManager.h"
#include "TimelineManager.h"

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
{
    setZValue(5.0);
}

QRectF TimeBarGraphicsItem::boundingRect() const
{
    //Always drawing
    QRect topBarRect = QRect(0.0f,
                             TimelineManager::Instance().GetViewPosition().y(),
                             TIMELINE_TRACK_MAX_DURATION_MS + TIMELINE_TRACK_INFO_WIDTH + TimelineManager::Instance().GetViewPosition().x(),
                             TIMELINE_UPPER_RULE_HEIGHT);
    return topBarRect;
}

void TimeBarGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    //qDebug() << TimelineManager::Instance().GetViewPosition().y();

    // Background
    QRect topBarRect = QRect(0.0f,
                             TimelineManager::Instance().GetViewPosition().y(),
                             TIMELINE_TRACK_MAX_DURATION_MS + TIMELINE_TRACK_INFO_WIDTH,
                             TIMELINE_UPPER_RULE_HEIGHT);

    QBrush brush = painter->brush();
    brush.setColor(LIGHT_GREY);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    painter->drawRect(topBarRect);

    // Selection
    QRect selectionRect = QRect(TIMELINE_TRACK_INFO_WIDTH + _selectionBeginTimeMs,
                                TimelineManager::Instance().GetViewPosition().y(),
                                _selectionEndTimeMs - _selectionBeginTimeMs,
                                TIMELINE_UPPER_RULE_HEIGHT);

    brush.setColor(RED);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    painter->drawRect(selectionRect);

    // Draw grid
    float minX = 0;
    float maxX = TIMELINE_TRACK_MAX_DURATION_MS;
    float minY = 0.0f;
    float maxY = TIMELINE_UPPER_RULE_HEIGHT+ 10000;
    float interval = 10.0f;

    QPen linePen = QPen();
    linePen.setWidth(1);
    linePen.setColor(BLACK);

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
            linePen.setWidth(4);
            linePen.setColor(BLACK);

            QString timeSecString = QString::number(timeSec, 'g', 4);
            int charCount = timeSecString.count();
            float textRectWidth = charCount * 16.0f;

            QRect timeValueTextRect = QRect( TIMELINE_TRACK_INFO_WIDTH + currentX - textRectWidth / 2.0f,
                                             TimelineManager::Instance().GetViewPosition().y(),
                                             textRectWidth,
                                             TIMELINE_UPPER_RULE_HEIGHT);

            painter->drawText(timeValueTextRect, timeSecString, Qt::AlignVCenter | Qt::AlignHCenter);

            painter->drawLine(TIMELINE_TRACK_INFO_WIDTH + currentX,
                              TimelineManager::Instance().GetViewPosition().y() + TIMELINE_UPPER_RULE_HEIGHT / 2.0f,
                              TIMELINE_TRACK_INFO_WIDTH + currentX,
                              TimelineManager::Instance().GetViewPosition().y() + TIMELINE_UPPER_RULE_HEIGHT
                              );
        }
        else if((int)currentX % 100 == 0)
        {
            linePen.setWidth(1);
            linePen.setColor(BLACK);

            QString timeSecString = QString::number(timeSec, 'g', 4);
            int charCount = timeSecString.count();
            float textRectWidth = charCount * 8.0f;

            QRect timeValueTextRect = QRect( TIMELINE_TRACK_INFO_WIDTH + currentX - textRectWidth / 2.0f,
                                             TimelineManager::Instance().GetViewPosition().y(),
                                             textRectWidth,
                                             TIMELINE_UPPER_RULE_HEIGHT);

            painter->drawText(timeValueTextRect, timeSecString, Qt::AlignVCenter | Qt::AlignHCenter);

            painter->drawLine(TIMELINE_TRACK_INFO_WIDTH + currentX,
                              TimelineManager::Instance().GetViewPosition().y() + TIMELINE_UPPER_RULE_HEIGHT / 2.0f,
                              TIMELINE_TRACK_INFO_WIDTH + currentX,
                              TimelineManager::Instance().GetViewPosition().y() + TIMELINE_UPPER_RULE_HEIGHT);
        }
        else
        {
            linePen.setWidth(1);
            linePen.setColor(GREY);

            QString timeSecString = QString::number(timeSec, 'g', 4);
            int charCount = timeSecString.count();
            float textRectWidth = charCount * 8.0f;

            QRect timeValueTextRect = QRect(TIMELINE_TRACK_INFO_WIDTH + currentX - textRectWidth / 2.0f,
                                            TimelineManager::Instance().GetViewPosition().y(),
                                            textRectWidth,
                                            TIMELINE_UPPER_RULE_HEIGHT);

            painter->drawLine(TIMELINE_TRACK_INFO_WIDTH + currentX,
                              TimelineManager::Instance().GetViewPosition().y() + 4.0 * TIMELINE_UPPER_RULE_HEIGHT / 5.0f,
                              TIMELINE_TRACK_INFO_WIDTH + currentX,
                              TimelineManager::Instance().GetViewPosition().y() + TIMELINE_UPPER_RULE_HEIGHT);
        }

        currentX += interval;

    }while(currentX <= maxX);

    // Cursor rect
    QRect cursorRect = QRect(TIMELINE_TRACK_INFO_WIDTH + TimeManager::Instance().Time(),
                             TimelineManager::Instance().GetViewPosition().y(),
                             1,
                             TIMELINE_UPPER_RULE_HEIGHT);

    brush.setColor(GREY);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    painter->drawRect(cursorRect);

    // Fixed position top left corner
    QRect fixexTopLeftCornerRect = QRect(TimelineManager::Instance().GetViewPosition().x(),
                                         TimelineManager::Instance().GetViewPosition().y(),
                                         TIMELINE_TRACK_INFO_WIDTH,
                                         TIMELINE_UPPER_RULE_HEIGHT);

    brush.setColor(GREY);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    painter->drawRect(fixexTopLeftCornerRect);
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
    return std::pair<float,float>(_selectionBeginTimeMs, _selectionEndTimeMs);
}
