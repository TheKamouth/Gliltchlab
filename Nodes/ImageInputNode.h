#ifndef IMAGEINPUTNODE_H
#define IMAGEINPUTNODE_H

#include "Nodes/Node.h"
#include "Nodes/GenericNode.h"
#include "Nodes/GenericNodePin.h"
#include "Nodes/GenScatterHierarchy.h"
#include "Nodes/TypeList.h"
#include "Nodes/IDataPin.h"

#include <QWidget>
#include <QPixmap>

typedef TYPE_LIST_2(StringInputPin, ImageOutputPin) ImageIntpuPinsTlist ;

class ImageInputNode : public GenericNode<ImageIntpuPinsTlist>
{
    Q_OBJECT

    static const QString DEFAULT_IMAGE_PATH;

public:
    explicit ImageInputNode();
    virtual ~ImageInputNode();

    // INode
    virtual NodeType Type() override { return ImageInput;}
    virtual QString Name() override { return "ImageInput";}
    virtual FlowData * MainOutput() override { return GetPinData<1>();}

    void OnLoadInputClicked();
    void SetInputFilePath(QString filePath);

protected:
    virtual bool BeforeProcessing() override;
    virtual bool ProcessInternal() override;

private:

    QString _inputFileName;
    QImage _outputImage;
};
#endif // IMAGEINPUTNODE_H
