#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

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

private:
    Ui::TimelineWidget *ui;
};

#endif // TIMELINEWIDGET_H
