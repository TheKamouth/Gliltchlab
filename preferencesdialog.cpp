#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget * parent, CommonPreferences * preferences) :
    QDialog(parent),
    _temporaryPreferences( *preferences),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    ptrPreferences = new CommonPreferences();

    this->setMaximumSize(390, 235);
    this->setMinimumSize(390, 235);

    this->ui->pushButton->setMinimumWidth( 70);
    this->ui->pushButton->setMaximumWidth( 70);

    this->ui->pushButton_2->setMinimumWidth( 70);
    this->ui->pushButton_2->setMaximumWidth( 70);

    UpdateUI();
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::GetTmpPref(){

    _temporaryPreferences.SetInputFolder( ptrPreferences->InputFolder());
    _temporaryPreferences.SetOutputFolder( ptrPreferences->OutputFolder());
    _temporaryPreferences.SetFastSave( ptrPreferences->FastSave());
    //_temporaryPreferences.SetKeepImageRatio( ptrPreferences->KeepImageRatio());
    //_temporaryPreferences.SetDisplayFramingLines( ptrPreferences->DisplayFramingLines());

    UpdateUI();
}

void PreferencesDialog::on_buttonBox_accepted(){

}

void PreferencesDialog::on_buttonBox_rejected(){

}

void PreferencesDialog::on_pushButton_clicked()
{
    //Dialog box
    QString* directory = new QString("Choose default input directory");

    //QString* filter = new QString("..");
    QString* filter = new QString("..");

    QString* selectedFilter = new QString("Images (*.png *.xpm *.jpg)");

    QString chosenFolder = QFileDialog::getExistingDirectory(this, *directory);

    if( chosenFolder.isNull()){
        return;
    }


    _temporaryPreferences.SetInputFolder( chosenFolder);

    UpdateUI();
}

void PreferencesDialog::on_pushButton_2_clicked()
{
    //Dialog box
    QString* directory = new QString("Choose default output directory");
    QString chosenFolder = QFileDialog::getExistingDirectory(this, *directory);

    if( chosenFolder.isNull()){
        return;
    }

    _temporaryPreferences.SetOutputFolder( chosenFolder);

    UpdateUI();
}

void PreferencesDialog::UpdateUI(){

    ui->le_dataFolder->setText( _temporaryPreferences.InputFolder());

    ui->le_outputFolder->setText( _temporaryPreferences.OutputFolder());

    ui->checkBox_2->setChecked( _temporaryPreferences.FastSave());

    //ui->checkBox_3->setChecked( _temporaryPreferences.DisplayFramingLines());
}

void PreferencesDialog::on_checkBox_2_toggled(bool checked)
{
    _temporaryPreferences.SetFastSave( checked);
}

//Save preferences
void PreferencesDialog::on_pushButton_3_clicked()
{
    ptrPreferences->SetInputFolder( _temporaryPreferences.InputFolder());
    ptrPreferences->SetOutputFolder( _temporaryPreferences.OutputFolder());
    ptrPreferences->SetFastSave( _temporaryPreferences.FastSave());
    //ptrPreferences->SetKeepImageRatio( _temporaryPreferences.KeepImageRatio());
    //ptrPreferences->SetDisplayFramingLines( _temporaryPreferences.DisplayFramingLines());

    ptrPreferences->SavePreferences();

    //if(ptrGlWidget)
        //ptrGlWidget->ShowFramingLines( _temporaryPreferences.DisplayFramingLines());

    this->close();
}

void PreferencesDialog::on_pushButton_4_clicked()
{
    this->close();
}
