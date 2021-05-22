#ifndef EDITDIRECTIONWIDGET_H
#define EDITDIRECTIONWIDGET_H

#include <QWidget>

namespace Ui {
class EditDirectionWidget;
}

class EditDirectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditDirectionWidget(QWidget *parent = 0);
    ~EditDirectionWidget();

    float GetSortingAngle();
    bool IsCircularSorting();

    QPointF sortingCenter;

private slots:
    void on_circularButton_toggled(bool checked);

signals:
    void CircularSortingEnabled(bool enable);

private:
    Ui::EditDirectionWidget *ui;
};

#endif // EDITDIRECTIONWIDGET_H
