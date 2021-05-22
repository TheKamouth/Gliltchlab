#include "editdirectionwidget.h"
#include "ui_editdirectionwidget.h"

EditDirectionWidget::EditDirectionWidget(QWidget *parent) :
    QWidget(parent),
    sortingCenter(0.5, 0.5),
    ui(new Ui::EditDirectionWidget)
{
    ui->setupUi(this);
}

EditDirectionWidget::~EditDirectionWidget()
{
    delete ui;
}

float EditDirectionWidget::GetSortingAngle(){

    if( ui->verticalButton->isChecked()){
        return 90.0;
    }
    else if( ui->horizontalButton->isChecked()){
        return 0.0;
    }
    else{
        return 0.0;//return ui->doubleSpinBox->value();
    }
}

bool EditDirectionWidget::IsCircularSorting(){

    return ui->circularButton->isChecked();
}

void EditDirectionWidget::on_circularButton_toggled(bool checked)
{
    emit CircularSortingEnabled(checked);
}
