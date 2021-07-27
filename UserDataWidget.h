#ifndef USERDATAWIDGET_H
#define USERDATAWIDGET_H

#include <QWidget>

namespace Ui {
class UserDataWidget;
}

class UserDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserDataWidget(QWidget *parent = nullptr);
    ~UserDataWidget();

private:
    Ui::UserDataWidget *ui;
};

#endif // USERDATAWIDGET_H
