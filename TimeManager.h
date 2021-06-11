#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

enum TimeManagerState
{
    PLAY,
    PAUSE,
    UNDEFINED
};

class TimeManager : public QObject
{
    Q_OBJECT;

public:
    TimeManager();
    TimeManager(TimeManager const &) = delete;
    void operator=(TimeManager const &) = delete;

    static TimeManager & Instance()
    {
        static TimeManager instance;
        return instance;
    }

    float Time();
    void SetTime(float timeMs);
    void Play();
    void Pause();
    void ForwardOneFrame();

private:
    float _time;
    TimeManagerState _state;
    int _aimFps;

    bool _processingFrame;
    QTimer _updateTimer;
    QElapsedTimer _updateDurationTimer;

    void Update();
};

#endif // TIMEMANAGER_H
