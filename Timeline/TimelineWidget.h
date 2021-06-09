#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include "Track.h"

#include <QDockWidget>

namespace Ui {
class TimelineWidget;
}

class TimelineWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit TimelineWidget(QWidget *parent = nullptr);
    ~TimelineWidget();

    void SetTimeline(QWidget * timeline);

private:
    Ui::TimelineWidget *ui;
};

#endif // TIMELINEWIDGET_H
