#include "TimeManager.h"

#include "Timeline/TimelineManager.h"
#include "FlowGraph/FlowGraphManager.h"

#include <QDebug>

TimeManager::TimeManager() :
    _time(0.0f),
    _state(PAUSE),
    _aimFps(60),
    _processingFrame(false)
{

    _updateTimer.setInterval(1000.0f / _aimFps);
    QObject::connect(&_updateTimer,&QTimer::timeout, this, &TimeManager::Update);
    _updateTimer.start();
}

float TimeManager::Time()
{
    return _time;
}

void TimeManager::SetTime(float timeMs)
{
    _time = timeMs;
}

void TimeManager::Play()
{
    if (_state == PLAY)
    {
        qDebug() << "Already playing";
        return;
    }

    _state = PLAY;
}

void TimeManager::Pause()
{
    if (_state == PAUSE)
    {
        qDebug() << "Already paused";
        return;
    }

    _state = PAUSE;
}

void TimeManager::ForwardOneFrame()
{
    qDebug() << "TODO";
}

void TimeManager::Update()
{
    if(_state == PAUSE)
    {
        return;
    }

    /*if(_processingFrame == true)
    {
        qDebug() << "Can't process fast enough to stick with " << _aimFps << " fps";
    }*/

    _updateTimer.stop();
    _updateDurationTimer.start();
    _processingFrame = true;

    TimelineManager & timelineManager = TimelineManager::Instance();
    timelineManager.SetTime(_time);

    FlowGraphManager & flowGraphManager = FlowGraphManager::Instance();
    flowGraphManager.Process();

    qDebug() << _updateDurationTimer.elapsed() << " ms per frame";



    _time += 1000.0f / _aimFps;

    std::pair<float,float> selectionBeginAndEndTimes = timelineManager.GetSectionBeginAndEndTimes();
    if(_time > selectionBeginAndEndTimes.second)
    {
        _time = selectionBeginAndEndTimes.first;
    }

    _updateTimer.start();
}
