#include "ImageOutputNode.h"
#include "ui_ImageOutputNode.h"

#include <QFileDialog>
#include <QDebug>
#include <QDateTime>
#include <QDir>

ImageOutputNode::ImageOutputNode() :
    ui(new Ui::ImageOutputNode)
{
    ui->setupUi(this);

    QObject::connect(ui->pb_loadInput, &QPushButton::clicked, this, &ImageOutputNode::OnChoseSaveDirClicked);
    QObject::connect(ui->pb_save, &QPushButton::clicked, this, &ImageOutputNode::OnSaveClicked);

    _outputDirName = QDir::currentPath();
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

    SetOutputFilePath(outputDir);
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

    QString timeStamp = QDateTime::currentDateTime().toString("_dd_MM_yyyy_hh_mm_ss");
    _input->save(_outputDirName + "/gliltched_" + timeStamp + ".png");
    qDebug() << _outputDirName+"/gliltched_" + timeStamp + ".png saved.";
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

bool ImageOutputNode::AfterProcessing()
{
    OnSaveClicked();

    return true;
}

void ImageOutputNode::SetInput(QImage *input)
{
    _input = input;
}


void ImageOutputNode::SetOutputFilePath(QString filePath)
{
    _outputDirName = filePath;
    _outputFolder = QDir(_outputDirName);

    ui->label->setText(_outputDirName);
}
