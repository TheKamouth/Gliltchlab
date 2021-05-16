#include "editmaskwidget.h"
#include "ui_editmaskwidget.h"

EditMaskWidget::EditMaskWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditMaskWidget),
    maskFileName( QString(""))
{
    ui->setupUi(this);

    this->setMaximumSize( 250, 160);
    this->setMinimumSize( 250, 160);

    ui->textBrowser->setMaximumSize(180, 30);
    ui->textBrowser->setMinimumSize(180, 30);
}

EditMaskWidget::~EditMaskWidget()
{
    delete ui;
}

int EditMaskWidget::GetMaskTypeAsInt(){

    return ui->maskTypeComboBox->currentIndex();
}

void EditMaskWidget::EnableMaskEditing(bool checked)
{
    ui->label_2->setEnabled( checked);
    ui->importMaskButton->setEnabled( checked);    
}

void EditMaskWidget::on_importMaskButton_clicked()
{
    SendOpenMask();
}

void EditMaskWidget::on_maskTypeComboBox_currentIndexChanged(int index)
{
    if( index == 0){
        EnableMaskEditing( false);
        ui->refreshButton->setEnabled( false);
    }
    else{
        EnableMaskEditing( true);

        if( maskFileName != QString("")){
            ui->refreshButton->setEnabled( true);
        }
    }
}

void EditMaskWidget::onMaskLoaded( QImage* maskImage, QString* maskName){

    int lastSlashIndex = maskName->lastIndexOf('/');

    maskFileName = *maskName;

    ui->textBrowser->setText( maskName->right( maskName->size() - lastSlashIndex - 1));

    ui->refreshButton->setEnabled( true);
}

void EditMaskWidget::on_refreshButton_clicked()
{
    SendRefreshMask();
}

void EditMaskWidget::onResetMaskSettings(){

    ui->maskTypeComboBox->setCurrentIndex( 0);

    EnableMaskEditing( false);

    maskFileName = QString("");
    ui->refreshButton->setEnabled( false);

    ui->textBrowser->setText(  QString( "no mask loaded"));
}
