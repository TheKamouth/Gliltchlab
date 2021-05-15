#ifndef IMAGEOUTPUTNODE_H
#define IMAGEOUTPUTNODE_H

#include "ImagePeakNode.h"
#include "NodeCommonWidget.h"
#include "Nodes/ImagePeakNode.h"

#include <QWidget>
#include <QDir>

namespace Ui {
class ImageOutputNode;
}

class ImageOutputNode : public ImagePeakNode
{
    Q_OBJECT

public:
    explicit ImageOutputNode(QWidget *parent = nullptr);
    ~ImageOutputNode();

    // Node
    virtual QString Name() override { return "Image output"; }
    //virtual NodeType Type() override { return NodeType.OuputImage; }

    virtual QWidget * Widget() override { return this; };
    virtual QWidget * NodeUiBaseWidgetInForm() override;
    virtual QLayout* NodeUiBaseLayoutInForm() override;
    virtual QWidget * SpecificUI() override;

    // ImageProcessor
    virtual bool AfterProcessing() override;
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
