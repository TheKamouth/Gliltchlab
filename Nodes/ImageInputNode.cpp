#include "ImageInputNode.h"
#include "ui_ImageInputNode.h"

#include <QFileDialog>
#include <QDebug>

ImageInputNode::ImageInputNode(QWidget * parent) :
    Node(),
    ui(new Ui::ImageInputNode),
    _previewPixMap(nullptr)
{
    ui->setupUi(this);

    Initialize();

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

QWidget *ImageInputNode::GetPeakWidget()
{
    // TODO
    // return glWidget with filter output shown;
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
        _input = nullptr;
        return;
    }

    _inputFileName = inputFilename;
    _input = new QImage(_inputFileName);

    ui->label->setText(_inputFileName);
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
    _previewPixMap->convertFromImage(*_input);
    ui->lb_preview->setPixmap(*_previewPixMap);
}
