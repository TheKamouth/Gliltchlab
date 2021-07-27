#ifndef TRACKGRAPHICSITEM_H
#define TRACKGRAPHICSITEM_H

#include "Track.h"

#include "ControlPointgraphicsItem.h"

#include <QGraphicsItem>

class CurveGraphicsItem;

class TrackGraphicsItem : public QGraphicsItem
{
public:
    TrackGraphicsItem(int index, Track * track);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

    void update();

    int TimelineTrackIndex();
    Track * GetTrack();
    ControlPointgraphicsItem * GetClosestControlPoint(QPointF sceneClickPos);
    bool IsTrackInfoRect(QPointF pos);

    void SetCurveGraphicsItem(CurveGraphicsItem * curveGraphicsItem);
    CurveGraphicsItem * GetCurveGraphicsItem( );

protected:
    virtual void dropEvent(QGraphicsSceneDragDropEvent * event) override;

private :
    Track * _track;
    CurveGraphicsItem * _curveGraphicsItem;

    int _timelineTrackIndex;
};

#endif // TRACKGRAPHICSITEM_H
