#include "FlowGraphScene.h"

#include <QDebug>
#include <QGraphicsTextItem>

FlowGraphScene::FlowGraphScene()
{
    // Draw grid
    float minX = -10000.0f;
    float maxX = 10000.0f;
    float minY = -10000.0f;
    float maxY = 10000.0f;

    float interval = 10.0f;

    //setForegroundBrush(QColor(5, 21, 21, 255));

    QPen linePen = QPen();
    linePen.setWidth(1);
    linePen.setColor(QColor(0, 0, 0, 255));

    QGraphicsTextItem * originTextItem = addText("(0,0)");
    originTextItem->setPos(QPointF(0,0));

    float currentX = minX;
    do
    {
        if(currentX == 0.0f)
        {
            linePen.setWidth(4);
            linePen.setColor(QColor(0, 0, 0, 255));
        }
        else if((int)currentX % 100 == 0)
        {
            linePen.setWidth(2);
            linePen.setColor(QColor(0, 0, 0, 255));
        }
        else
        {
            linePen.setWidth(1);
            linePen.setColor(QColor(0, 0, 0, 255));
        }

        addLine(currentX, minY, currentX, maxY, linePen);
        currentX += interval;
    }while(currentX <= maxX);

    float currentY = minY;
    do
    {
        if(currentY == 0.0f)
        {
            linePen.setWidth(4);
            linePen.setColor(QColor(0, 0, 0, 255));
        }
        else if((int)currentY % 100 == 0)
        {
            linePen.setWidth(2);
            linePen.setColor(QColor(0, 0, 0, 255));
        }
        else
        {
            linePen.setWidth(1);
            linePen.setColor(QColor(0, 0, 0, 255));
        }

        addLine(minX, currentY, maxX, currentY, linePen);
        currentY += interval;
    }while(currentY <= maxY);

}
