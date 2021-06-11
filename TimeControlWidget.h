#ifndef TIMECONTROLWIDGET_H
#define TIMECONTROLWIDGET_H

#include <QDockWidget>

namespace Ui {
class TimeControlWidget;
}

class TimeControlWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit TimeControlWidget(QWidget *parent = nullptr);
    ~TimeControlWidget();

    void OnFirstFrameButtonClicked();
    void OnBackwardOneFrameButtonClicked();
    void OnPlayPauseButtonClicked();
    void OnForwardOneFrameButtonClicked();
    void OnLastFrameButtonClicked();

signals:
    void PlayPauseClicked();

private:
    Ui::TimeControlWidget *ui;
};

#endif // TIMECONTROLWIDGET_H
