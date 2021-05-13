#ifndef CONTRASTFILTERNODE_H
#define CONTRASTFILTERNODE_H

#include "Node.h"

#include "ImageProcessors/ImageProcessorBase.h"

#include <QWidget>

#include <QOpenGLContext>
#include <QSize>
#include <QImage>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShader>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

namespace Ui {
class DesaturateFilterNode;
}

enum DesaturationMethod
{
    Lightness,
    Average,
    Min,
    Max,
    DesaturationMethodCount
};

class DesaturateFilterNode : public ImageProcessorBase
{
    Q_OBJECT

public:
    explicit DesaturateFilterNode(QWidget *parent = nullptr);
    ~DesaturateFilterNode();

    // Node
    virtual QString Name() override { return "Desaturate"; }
    virtual QWidget * Widget() override { return this; };
    virtual QWidget * NodeUiBaseWidgetInForm() override;
    virtual QLayout* NodeUiBaseLayoutInForm() override;
    virtual QWidget * SpecificUI() override;

    // ImageProcessorBase
    virtual bool TryProcess() override;
    virtual void SetParameters() override;
    virtual void BeforeProcessing() override;
    virtual void AfterProcessing() override;
    virtual QImage * Output() override;

    //
    void ProcessInternal() override;

private:
    Ui::DesaturateFilterNode *ui;

    DesaturationMethod _desaturationMode;

    void InitNodeTypeComboBox();

    QStringList AvailableDesaturationMethodNames();
    QString AvailableDesaturationMethodName(int index);
    QString AvailableDesaturationMethodName(DesaturationMethod desaturationMethod);

    // constants
    const QString VERTEX_SHADER_PATH = "D:\\5_PROJETS\\5_DEV\\VirtualScanner\\sources\\VirtualScanner\\ImageProcessors\\VertexShader.vert";
    const QString CONTRAST_FRAGMENT_SHADER_PATH = "D:\\5_PROJETS\\5_DEV\\VirtualScanner\\sources\\VirtualScanner\\ImageProcessors\\Contrast.frag";

    struct VertexData
    {
        QVector2D position;
        QVector2D texCoord;
    };

    const VertexData FULL_SCREEN_VERTICES_DATA[4] =
    {
        {{ -1.0f, +1.0f }, { 0.0f, 1.0f }}, // top-left
        {{ +1.0f, +1.0f }, { 1.0f, 1.0f }}, // top-right
        {{ -1.0f, -1.0f }, { 0.0f, 0.0f }}, // bottom-left
        {{ +1.0f, -1.0f }, { 1.0f, 0.0f }}  // bottom-right
    };
    const GLuint FULL_SCREEN_VERTICES_INDEXES[4] = { 0, 1, 2, 3 };
    const GLuint ONE_LINE_INDEXES[2] = {0, 1};

    // opengl
    bool _initialized;
    QSize _fboSize;
    QOpenGLContext _glContext;
    QOffscreenSurface _offScreenSurface;
    QOpenGLShaderProgram * _glShaderProgram;
    QOpenGLFramebufferObject * _glFrameBufferObject;
    QOpenGLTexture * _inputTexture ;
    QOpenGLTexture * _previousOutputTexture ;
    QOpenGLBuffer _glVertexBuffer;
    QOpenGLBuffer _glFragmentBuffer;

    // input / output
    int currentOutputIndex = 0;

    QVector2D ToTexCoord(QVector2D position);
    QVector2D ToVertexCoord(QVector2D position);
};

#endif // CONTRASTFILTERNODE_H
