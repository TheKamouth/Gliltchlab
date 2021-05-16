#ifndef PROCESSPROGRESSBAR_H
#define PROCESSPROGRESSBAR_H

#include <QWidget>

namespace Ui {
class ProcessProgressBar;
}

class ProcessProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessProgressBar(QWidget *parent = 0);
    ~ProcessProgressBar();

    void SetValueAndUpdate( int value);

private slots:
    void on_pushButton_clicked();

signals:
    void SendProcessAbortToMainWindow();

private:
    Ui::ProcessProgressBar *ui;
};

#endif // PROCESSPROGRESSBAR_H
