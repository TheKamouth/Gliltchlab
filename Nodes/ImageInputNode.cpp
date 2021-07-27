#include "ImageInputNode.h"

#include <QFileDialog>
#include <QImage>
#include <QDebug>

const QString ImageInputNode::DEFAULT_IMAGE_PATH = ":/images/Resources/Images/default.png";

ImageInputNode::ImageInputNode()
{
    //QObject::connect( ui->pb_loadInput, &QPushButton::clicked, this, &ImageInputNode::OnLoadInputClicked);
    //QObject::connect( ui->pb_reload, &QPushButton::clicked, this, &ImageInputNode::OnReloadClicked);

    // TMP : easing tests

    // Setting default value for pin 0:
    IDataPin * inputFileNamePin = GetDataPinAt<0>();
    QString * defaultValue = new QString(DEFAULT_IMAGE_PATH);
    QString * pinData = inputFileNamePin->GetData()->GetString();
    pinData = defaultValue;

    ProcessInternal();
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
    IDataPin * inputFileNamePin = GetDataPinAt<0>();
    QString * inputFilename = inputFileNamePin->GetData()->GetString();

    if(inputFilename == nullptr
            || inputFilename->isEmpty()
            || inputFilename->compare("EmptyString") == 0)
    {
        *(inputFileNamePin->GetData()->GetString()) = QString(DEFAULT_IMAGE_PATH);
    }

    QImage * outputImage = new QImage(*inputFilename);
    FlowData * outputImageData = new FlowData(outputImage);
    GetDataPinAt<1>()->SetData(outputImageData);

    return true;
}
