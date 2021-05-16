#include "editfunctionwidget.h"
#include "ui_editfunctionwidget.h"

EditFunctionWidget::EditFunctionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditFunctionWidget)
{
    ui->setupUi(this);
}

EditFunctionWidget::~EditFunctionWidget()
{
    delete ui;
}

int EditFunctionWidget::GetSortingRuleId(){

    return ui->comboBox->currentIndex();
}

bool EditFunctionWidget::isInvertedRule(){

    return ui->checkBox->isChecked();
}
