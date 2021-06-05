#include "ImageInputNode.h"

#include <QFileDialog>
#include <QImage>
#include <QDebug>

ImageInputNode::ImageInputNode()
{
    //QObject::connect( ui->pb_loadInput, &QPushButton::clicked, this, &ImageInputNode::OnLoadInputClicked);
    //QObject::connect( ui->pb_reload, &QPushButton::clicked, this, &ImageInputNode::OnReloadClicked);
}

ImageInputNode::~ImageInputNode()
{
}

void ImageInputNode::OnLoadInputClicked()
{
    //Dialog box
    QString directory = QString("Open file");

    QString* filter = new QString("..");
    // TODO
    // QString filter = _preferences.InputFolder();

    QString selectedFilter = QString("Images (*.png *.xpm *.jpg)");
    QString inputFilename = QFileDialog::getOpenFileName( nullptr, directory, *filter, selectedFilter);

    if( inputFilename == "" )
    {
        qDebug() << "Error loading file: " << inputFilename << Qt::endl;
        GetDataPinAt<1>()->SetData(nullptr);
        return;
    }

    SetInputFilePath(inputFilename);
}

void ImageInputNode::SetInputFilePath(QString filePath)
{
    _inputFileName = filePath;
    emit NodeOutputChanged(this);
}

bool ImageInputNode::BeforeProcessing()
{


    return true;
}

bool ImageInputNode::ProcessInternal()
{
    QString * inputFilename = GetDataPinAt<0>()->GetData()->GetString();

    if(inputFilename->isEmpty())
        return false;

    QImage * outputImage = new QImage(*inputFilename);
    FlowData * outputImageData = new FlowData(outputImage);
    GetDataPinAt<1>()->SetData(outputImageData);

    return true;
}
