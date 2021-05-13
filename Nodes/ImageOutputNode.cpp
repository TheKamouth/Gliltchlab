#include "ImageOutputNode.h"
#include "ui_ImageOutputNode.h"

#include <QFileDialog>
#include <QDebug>
#include <QDateTime>

ImageOutputNode::ImageOutputNode(QWidget *parent) :
    ui(new Ui::ImageOutputNode)
{
    ui->setupUi(this);

    Initialize();

    QObject::connect(ui->pb_loadInput, &QPushButton::clicked, this, &ImageOutputNode::OnChoseSaveDirClicked);
    QObject::connect(ui->pb_save, &QPushButton::clicked, this, &ImageOutputNode::OnSaveClicked);

    _outputDirName = "";
}

ImageOutputNode::~ImageOutputNode()
{
    delete ui;
}

void ImageOutputNode::OnChoseSaveDirClicked()
{
    //Dialog box
    QString directory = QString("Open file");

    QString* filter = new QString("..");
    // TODO
    // QString filter = _preferences.InputFolder();

    QString selectedFilter = QString("Images (*.png *.xpm *.jpg)");
    QString outputDir = QFileDialog::getExistingDirectory(this, directory, *filter);

    if( outputDir == "" )
    {
        qDebug() << "Error loading file: " << outputDir << Qt::endl;
        _input = nullptr;
        return;
    }

    _outputDirName = outputDir;
    _outputFolder = QDir(_outputDirName);

    ui->label->setText(_outputDirName);
}

void ImageOutputNode::OnSaveClicked()
{
    if(_outputDirName.isEmpty())
    {
        qDebug() << "Cannot save: outputDir is empty";
        return;
    }

    if(_input == nullptr)
    {
        qDebug() << "Cannot save: _input is nullptr";
        return;
    }

    QString timeStamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_mm_ss");
    _input->save(_outputDirName + "/gliltched_" + timeStamp + ".png");
}

QWidget * ImageOutputNode::NodeUiBaseWidgetInForm()
{
    return ui->NodeUiBase;
}

QLayout *ImageOutputNode::NodeUiBaseLayoutInForm()
{
    return ui->NodeCommonWidgetLayout;
}

QWidget * ImageOutputNode::SpecificUI()
{
    return ui->widget;
}

bool ImageOutputNode::TryProcess()
{

    if(_input == nullptr)
    {
        qDebug() << "Cannot save: _input is null";
        return false;
    }

    OnSaveClicked();

    return true;
}
