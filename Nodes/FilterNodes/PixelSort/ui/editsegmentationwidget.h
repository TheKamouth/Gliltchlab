#ifndef EDITSEGMENTATIONWIDGET_H
#define EDITSEGMENTATIONWIDGET_H

#include <QWidget>

namespace Ui {
class EditSegmentationWidget;
}

class EditSegmentationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditSegmentationWidget(QWidget *parent = nullptr);
    ~EditSegmentationWidget();

    int GetThreshold();
    bool IsThresholdSegmentationEnabled();
    bool IsFunctionSegmentationEnabled();
    int GetSegConstant();
    int GetSegSlope();
    int GetRandMin();
    int GetRandMax();

    void CircularSortingEnabled(bool enable);

private:
    Ui::EditSegmentationWidget *ui;
};

#endif // EDITSEGMENTATIONWIDGET_H
