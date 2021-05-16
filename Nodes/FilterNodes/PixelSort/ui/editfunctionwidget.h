#ifndef EDITFUNCTIONWIDGET_H
#define EDITFUNCTIONWIDGET_H

#include <QWidget>

namespace Ui {
class EditFunctionWidget;
}

class EditFunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditFunctionWidget(QWidget *parent = 0);
    ~EditFunctionWidget();

    int GetSortingRuleId();

    bool isInvertedRule();

private:
    Ui::EditFunctionWidget *ui;
};

#endif // EDITFUNCTIONWIDGET_H
