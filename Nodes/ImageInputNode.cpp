#include "ImageInputNode.h"
#include "ui_ImageInputNode.h"

#include <QFileDialog>
#include <QDebug>

ImageInputNode::ImageInputNode(QWidget * parent) :
    ImagePeakNode(),
    ui(new Ui::ImageInputNode),
    _previewPixMap(nullptr)
{
    ui->setupUi(this);

    QObject::connect( ui->pb_loadInput, &QPushButton::clicked, this, &ImageInputNode::OnLoadInputClicked);
    QObject::connect( ui->pb_reload, &QPushButton::clicked, this, &ImageInputNode::OnReloadClicked);

    ui->pb_reload->setEnabled(false);
}

ImageInputNode::~ImageInputNode()
{
    delete ui;
}

QWidget * ImageInputNode::NodeUiBaseWidgetInForm()
{
    return ui->NodeUiBase;
}

QLayout *ImageInputNode::NodeUiBaseLayoutInForm()
{
    return ui->NodeCommonWidgetLayout;
}

QWidget * ImageInputNode::SpecificUI()
{
    return ui->widget;
}

void ImageInputNode::OnLoadInputClicked()
{
    //Dialog box
    QString directory = QString("Open file");

    QString* filter = new QString("..");
    // TODO
    // QString filter = _preferences.InputFolder();

    QString selectedFilter = QString("Images (*.png *.xpm *.jpg)");
    QString inputFilename = QFileDialog::getOpenFileName(this, directory, *filter, selectedFilter);

    if( inputFilename == "" )
    {
        qDebug() << "Error loading file: " << inputFilename << Qt::endl;
        _output = nullptr;
        return;
    }

    // should input node be output only ?
    // weird vocabulary
    _output = new QImage(inputFilename);

    SetInputFilePath(inputFilename);
}

void ImageInputNode::SetInputFilePath(QString filePath)
{
    _inputFileName = filePath;
    _outputFileName = filePath;
    _output = new QImage(_outputFileName);

    ui->label->setText(_outputFileName);
    ui->pb_reload->setEnabled(true);

    UpdatePreview();

    // only one flow for now
    emit InputLoaded(_input, 0);
}

// Reload _inputFileName
void ImageInputNode::OnReloadClicked()
{
    if(_inputFileName.isEmpty())
    {
        qDebug() << "A file muste be opened";
        return;
    }

    _input = new QImage(_inputFileName);

    ui->label->setText(_inputFileName);

    UpdatePreview();

    // only one flow for now
    emit InputLoaded(_input, 0);
}

void ImageInputNode::UpdatePreview()
{
    if(_previewPixMap != nullptr)
    {
        delete _previewPixMap;
    }

    _previewPixMap = new QPixmap;
    _previewPixMap->convertFromImage(*_output);
    ui->lb_preview->setPixmap(*_previewPixMap);
}
