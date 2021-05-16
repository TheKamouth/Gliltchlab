#ifndef EDITMASKWIDGET_H
#define EDITMASKWIDGET_H

#include <QWidget>

#include "openglwidget.h"

namespace Ui {
class EditMaskWidget;
}

class EditMaskWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditMaskWidget(QWidget *parent = 0);
    ~EditMaskWidget();

    int GetMaskTypeAsInt();

private slots:

    void on_importMaskButton_clicked();

    void onMaskLoaded( QImage*, QString*);

    void onResetMaskSettings();

    void on_maskTypeComboBox_currentIndexChanged(int index);

    void on_refreshButton_clicked();

signals:
    void SendOpenMask();

    void SendRefreshMask();

private:
    Ui::EditMaskWidget *ui;

    void EnableMaskEditing( bool);

    QString maskFileName;
};

#endif // EDITMASKWIDGET_H
