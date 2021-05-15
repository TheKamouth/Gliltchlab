#ifndef IMAGEINPUTNODE_H
#define IMAGEINPUTNODE_H

#include "Node.h"
#include "ImagePeakNode.h"

#include "NodeCommonWidget.h"

#include <QWidget>
#include <QPixmap>

namespace Ui {
class ImageInputNode;
}

class ImageInputNode : public ImagePeakNode
{
    Q_OBJECT


public:
    explicit ImageInputNode(QWidget *parent = nullptr);
    virtual ~ImageInputNode();

    virtual QString Name() override { return "Image input"; }
    virtual QWidget * Widget() override { return this; };
    virtual QWidget * NodeUiBaseWidgetInForm() override;
    virtual QLayout* NodeUiBaseLayoutInForm() override;
    virtual QWidget * SpecificUI() override;

    void OnLoadInputClicked();
    void OnReloadClicked();
    void SetInputFilePath(QString filePath);

//signals:
    //void InputLoaded(QImage* input, int flowIndex);

private:
    Ui::ImageInputNode *ui;

    //NodeUiBase * _nodeCommonUi;

    QString _inputFileName;
    QString _outputFileName;

    QPixmap * _previewPixMap;

    void UpdatePreview();
};

#endif // IMAGEINPUTNODE_H
