#ifndef FLOWGRAPHSCENE_H
#define FLOWGRAPHSCENE_H

#include <QGraphicsScene>

class FlowGraphScene : public QGraphicsScene
{
public:
    FlowGraphScene();

    void SetSelectionArea(const QPainterPath & path, const QTransform & deviceTransform)
    {
        setSelectionArea(path, deviceTransform);
    }
};

#endif // FLOWGRAPHSCENE_H
