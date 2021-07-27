#include "TrackGraphicsItem.h"

#include "TimelineConstants.h"
#include "ColorPalette.h"
#include "TimelineManager.h"
#include "CurveGraphicsItem.h"

// Add a ClipFactory ?
#include "AudioClip.h"

#include "FlowGraph/FlowData.h"

#include <QPainter>
#include <QDebug>
#include <QMimeData>
#include <QUrl>

#include <math.h>

TrackGraphicsItem::TrackGraphicsItem(int index, Track * track) :
    _track(track),
    _curveGraphicsItem(nullptr),
    _timelineTrackIndex(index)
{
    setZValue(1.0f);

    setAcceptDrops(true);
}

QRectF TrackGraphicsItem::boundingRect() const
{
    QRect wholeTrackRect = QRect(0,
                                 _timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                                 TIMELINE_TRACK_MAX_DURATION_MS + TIMELINE_TRACK_INFO_WIDTH,
                                 TIMELINE_TRACK_HEIGHT);

    //Timebar height depends on view
    QPointF timeBarOffsetPos = -1.0f * TimelineManager::Instance().GetViewPosition();
    //qDebug() << timeBarOffsetPos;
    timeBarOffsetPos = QPointF(0.0f, 0.0f);
    // Fixed position top left corner
    QRect fixextrackInfoRect = QRect(timeBarOffsetPos.x() + TimelineManager::Instance().GetViewPosition().x(),
                                     timeBarOffsetPos.y() + TIMELINE_UPPER_RULE_HEIGHT + _timelineTrackIndex * TIMELINE_TRACK_HEIGHT,
                                     TIMELINE_TRACK_MAX_DURATION_MS + TIMELINE_TRACK_INFO_WIDTH,
                                     TIMELINE_TRACK_HEIGHT);
    return fixextrackInfoRect;
}

void TrackGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Timebar height depends on view
    QPointF timeBarOffsetPos = -1.0f * TimelineManager::Instance().GetViewPosition();
    //qDebug() << timeBarOffsetPos;
    timeBarOffsetPos = QPointF(0.0f, 0.0f);

    QRect wholeTrackRect = QRect(0,
                                 _timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                                 TIMELINE_TRACK_MAX_DURATION_MS + TIMELINE_TRACK_INFO_WIDTH,
                                 TIMELINE_TRACK_HEIGHT);

    QBrush painterBrush = painter->brush();
    painterBrush.setColor(BLACK);
    painterBrush.setStyle(Qt::NoBrush);

    painter->setBrush(painterBrush);
    painter->drawRect(wholeTrackRect);

    QRect trackInfoRect = QRect(0,
                                _timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                                TIMELINE_TRACK_INFO_WIDTH,
                                TIMELINE_TRACK_HEIGHT);

    painter->setRenderHint( QPainter::Antialiasing, true);

    painterBrush.setColor(GREY);
    painterBrush.setStyle(Qt::SolidPattern);

    painter->setBrush(painterBrush);
    //painter->drawRect(trackInfoRect);

    painterBrush.setColor(BLACK);
    //painter->drawText( trackInfoRect, _track->Name() );

    FlowDataType type = _track->Type();
    QColor typeTextColor = FlowData::TypeToColor(type);
    QString typeText = "\n" + FlowData::TypeString(type);

    painterBrush.setColor(typeTextColor);
    //painter->drawText( trackInfoRect, typeText );

    // Fixed position top left corner
    QRect fixextrackInfoRect = QRect(timeBarOffsetPos.x() + TimelineManager::Instance().GetViewPosition().x(),
                                     timeBarOffsetPos.y() + TIMELINE_UPPER_RULE_HEIGHT + _timelineTrackIndex * TIMELINE_TRACK_HEIGHT,
                                     TIMELINE_TRACK_INFO_WIDTH,
                                     TIMELINE_TRACK_HEIGHT);

    painterBrush.setColor(LIGHT_GREY);
    painterBrush.setStyle(Qt::SolidPattern);
    painter->setBrush(painterBrush);
    painter->drawRect(fixextrackInfoRect);

    painterBrush.setColor(BLACK);
    painter->drawText( fixextrackInfoRect, _track->Name() );
}

void TrackGraphicsItem::update()
{
    QGraphicsItem::update();

    if(_curveGraphicsItem != nullptr)
        _curveGraphicsItem->update();
}

int TrackGraphicsItem::TimelineTrackIndex()
{
    return _timelineTrackIndex;
}

Track *TrackGraphicsItem::GetTrack()
{
    return _track;
}

ControlPointgraphicsItem *TrackGraphicsItem::GetClosestControlPoint(QPointF sceneClickPos)
{
    if(_curveGraphicsItem == nullptr)
        return nullptr;

    QList<ControlPointgraphicsItem*> * controlPointsGraphicItems = _curveGraphicsItem->GetControlPointsGraphicItem();

    ControlPointgraphicsItem * closestPoint = nullptr;
    float closestDistance = 0.0f;

    float time = sceneClickPos.x() - TIMELINE_TRACK_INFO_WIDTH;
    float value = sceneClickPos.y();

    float lowerBoundY = _timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT + TIMELINE_TRACK_HEIGHT;
    float upperBoundY = _timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT;

    // alpha [0,1]
    float alpha = std::clamp( ( value - _curveGraphicsItem->GetCurve()->LowerBound() ) / _curveGraphicsItem->GetCurve()->Range(), 0.0f, 1.0f);
    float y = alpha * upperBoundY + ( 1.0f - alpha ) * lowerBoundY;

    QPointF clickTrackPosition = QPointF(time, y);

    for(int i = 0 ; i < controlPointsGraphicItems->count() ; i++)
    {
        ControlPointgraphicsItem * controlPointsGraphicItem = controlPointsGraphicItems->at(i);
        ControlPoint * cp = controlPointsGraphicItem->GetControlPoint();


        float distanceToClick = std::sqrt( std::pow(clickTrackPosition.x() - cp->Time(), 2.0f)
                                           + std::pow(clickTrackPosition.x() - cp->Value(), 2.0f));

        if(closestPoint == nullptr || distanceToClick < closestDistance)
        {
            closestDistance = distanceToClick;
            closestPoint = controlPointsGraphicItem;
        }
    }

    return closestPoint;
}

void TrackGraphicsItem::SetCurveGraphicsItem(CurveGraphicsItem *curveGraphicsItem)
{
    if(_curveGraphicsItem != nullptr)
        _curveGraphicsItem = curveGraphicsItem;
}

CurveGraphicsItem *TrackGraphicsItem::GetCurveGraphicsItem()
{
    if(_curveGraphicsItem != nullptr)
        return _curveGraphicsItem;

    return nullptr;
}

void TrackGraphicsItem::dropEvent(QGraphicsSceneDragDropEvent * event)
{
    // What is dropped ?
    const QMimeData * mimeData = event->mimeData();
    QList<QUrl> url = mimeData->urls();

    QList<QString> supportedAudioFileExtensions = {".wav", ".mp3"};
    QList<QString> supportedVideoFileExtensions = {".mkv", ".mp4", ".avi"};
    QList<QString> supportedImageFileExtensions = {".bmp", ".jpg", ".jpeg"};

    FlowDataType droppedFileDataType = Unassigned;

    // Handling single file drag and drop
    //for(int i = 0 ; i < url.count(); i++)
    //{
        QUrl filePath = url.at(0);
        QString fileName = url.at(0).fileName();
        int dotPosition = fileName.lastIndexOf('.');
        int extensionLength = fileName.length() - dotPosition;
        QString extension = fileName.right(extensionLength);

        if( supportedAudioFileExtensions.contains(extension))
        {
            qDebug()<<"dropped audio file on track";
            droppedFileDataType = Audio;
        }
        else if( supportedVideoFileExtensions.contains(extension))
        {
            qDebug()<<"dropped video file on track";
            droppedFileDataType = Video;
        }
        else if( supportedImageFileExtensions.contains(extension))
        {
            qDebug()<<"dropped image file on track";
            droppedFileDataType = Image;
        }
    //}

    if( droppedFileDataType == Unassigned)
    {
        qDebug() << "This file format is not supported.";
        return;
    }

    // If dropped item data type differs from track type, no effect
    if( _track->Type() == Unassigned)
    {
        _track->SetType(droppedFileDataType);

        // Create and add clip to track
        AudioClip * droppedAudio = new AudioClip(filePath);

        _track->AddClip(droppedAudio);
    }
    else if(_track->Type() == droppedFileDataType )
    {
        // Add clip to track
    }
}
