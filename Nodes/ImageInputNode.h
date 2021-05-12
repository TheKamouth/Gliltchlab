#ifndef IMAGEINPUTNODE_H
#define IMAGEINPUTNODE_H

#include "Node.h"

#include "NodeCommonWidget.h"

#include <QWidget>
#include <QPixmap>

namespace Ui {
class ImageInputNode;
}

class ImageInputNode : public Node
{
    Q_OBJECT

public:
    explicit ImageInputNode(QWidget *parent = nullptr);
    ~ImageInputNode();


    virtual QString Name() override { return "Image input"; }
    virtual QWidget * Widget() override { return this; };
    virtual QWidget * NodeUiBaseWidgetInForm() override;
    virtual QLayout* NodeUiBaseLayoutInForm() override;
    virtual QWidget * SpecificUI() override;
    virtual QWidget * GetPeakWidget() override;

    void OnLoadInputClicked();
    void OnReloadClicked();

signals:

    void InputLoaded(QImage* input, int flowIndex);

private:
    Ui::ImageInputNode *ui;

    //NodeUiBase * _nodeCommonUi;

    QImage * _input;
    QString _inputFileName;
    QPixmap * _previewPixMap;

    void UpdatePreview();
};

#endif // IMAGEINPUTNODE_H
