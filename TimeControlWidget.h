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

private:
    Ui::TimeControlWidget *ui;
};

#endif // TIMECONTROLWIDGET_H
