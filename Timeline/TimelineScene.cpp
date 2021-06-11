#include "TimelineScene.h"

#include "TimelineConstants.h"
#include "ColorPalette.h"

#include <QGraphicsTextItem>

TimelineScene::TimelineScene()
{
    /*
    // Draw grid
    float minX = TIMELINE_TRACK_INFO_WIDTH;
    // pos x = 1000 = 1000ms from start
    float maxX = 250000.0f;

    float minY = TIMELINE_UPPER_RULE_HEIGHT;
    float maxY = TIMELINE_TRACK_HEIGHT * 64;

    float intervalY = TIMELINE_TRACK_HEIGHT;
    float intervalX = 100;

    //setForegroundBrush(QColor(5, 21, 21, 255));

    QPen linePen = QPen();
    linePen.setWidth(1);
    linePen.setColor(QColor(0, 0, 0, 255));

    QFont font = QFont();
    font.setPointSize(8);

    float currentX = minX;
    do
    {
        linePen.setWidth(1);
        linePen.setColor(BLACK);

        addLine(currentX, minY, currentX, 0.0f, linePen);

        float timeValue = currentX - TIMELINE_TRACK_INFO_WIDTH;
        QString timeValueString = QString::number( timeValue / 1000.0f,'g',2);

        QGraphicsTextItem * originTextItem = addText(timeValueString, font);
        originTextItem->setPos( QPointF( currentX - 12, 0 ) );

        currentX += intervalX;

    }while(currentX <= maxX);


    float currentY = minY;
    do
    {
        linePen.setWidth(1);
        linePen.setColor(QColor(0, 0, 0, 255));

        addLine(minX, currentY, maxX, currentY, linePen);
        currentY += intervalY;

    }while(currentY <= maxY);

    QPen pen = QPen();
    linePen.setWidth(1);
    linePen.setColor(QColor(30, 30, 30, 255));

    QBrush brush = QBrush();
    //brush.setStyle(Qt::SolidPattern);
    linePen.setColor( QColor( 170, 170, 170, 255));

    QRect upperRule = QRect(0.0f, 0.0f, width(),TIMELINE_UPPER_RULE_HEIGHT);
    //addRect( upperRule, pen, brush);

    */
}
