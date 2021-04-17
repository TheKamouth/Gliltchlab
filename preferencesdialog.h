#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QFileDialog>

#include "CommonPreferences.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget * parent, CommonPreferences * preferences);
    ~PreferencesDialog();

    CommonPreferences *ptrPreferences;
    CommonPreferences _temporaryPreferences;

    //OpenGLWidget *ptrGlWidget;

    void UpdateUI();

    void GetTmpPref();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_2_toggled(bool checked);

    //Save preferences
    void on_pushButton_3_clicked();

//Cancel
    void on_pushButton_4_clicked();

private:
    Ui::PreferencesDialog *ui;


};

#endif // PREFERENCESDIALOG_H
