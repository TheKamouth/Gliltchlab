#ifndef IMAGEOUTPUTNODE_H
#define IMAGEOUTPUTNODE_H

#include "ImagePeakNode.h"
#include "NodeCommonWidget.h"

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
    virtual QWidget * Widget() override { return this; };
    virtual QWidget * NodeUiBaseWidgetInForm() override;
    virtual QLayout* NodeUiBaseLayoutInForm() override;
    virtual QWidget * SpecificUI() override;

    virtual bool TryProcess() override;

private:
    Ui::ImageOutputNode *ui;

    QDir _outputFolder;
    QString _outputDirName;

    void OnChoseSaveDirClicked();
    void OnSaveClicked();
};

#endif // IMAGEOUTPUTNODE_H
