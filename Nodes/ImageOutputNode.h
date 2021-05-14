#ifndef IMAGEOUTPUTNODE_H
#define IMAGEOUTPUTNODE_H

#include "ImagePeakNode.h"
#include "NodeCommonWidget.h"
#include "FilterNodes/ImageProcessorBase.h"

#include <QWidget>
#include <QDir>

namespace Ui {
class ImageOutputNode;
}

class ImageOutputNode : public ImageProcessorBase
{
    Q_OBJECT

public:
    explicit ImageOutputNode(QWidget *parent = nullptr);
    ~ImageOutputNode();

    // Node
    virtual QString Name() override { return "Image output"; }
    virtual QWidget * Widget() override { return this; };
    virtual QWidget * NodeUiBaseWidgetInForm() override;
    virtual QLayout* NodeUiBaseLayoutInForm() override;
    virtual QWidget * SpecificUI() override;
    virtual bool TryProcess() override;

    // ImageProcessor
    virtual void AfterProcessing() override;
    void GetTempImageOutputFilePath();

    virtual void SetInput(QImage * input) override;

    void SetOutputFilePath(QString filePath);

private:
    Ui::ImageOutputNode *ui;

    QDir _outputFolder;
    QString _outputDirName;

    void OnChoseSaveDirClicked();
    void OnSaveClicked();
};

#endif // IMAGEOUTPUTNODE_H
