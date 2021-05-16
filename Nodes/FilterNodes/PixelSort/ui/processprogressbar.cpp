#include "processprogressbar.h"
#include "ui_processprogressbar.h"

#include <iostream>

ProcessProgressBar::ProcessProgressBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessProgressBar)
{
    ui->setupUi(this);   

    ui->progressBar->setValue(50.0);
}

ProcessProgressBar::~ProcessProgressBar()
{
    delete ui;
}

void ProcessProgressBar::SetValueAndUpdate( int value){


    ui->progressBar->setValue(value);

    this->update();
}

void ProcessProgressBar::on_pushButton_clicked()
{
    SendProcessAbortToMainWindow();
}
