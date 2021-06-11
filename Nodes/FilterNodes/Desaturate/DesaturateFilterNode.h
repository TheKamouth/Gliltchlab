#ifndef CONTRASTFILTERNODE_H
#define CONTRASTFILTERNODE_H

#include "Nodes/Node.h"

#include "Nodes/GenericNode.h"
#include "Nodes/GenScatterHierarchy.h"

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

enum DesaturationMethod
{
    Lightness,
    Average,
    Min,
    Max,
    HslAverageToRgb,
    HslMinToRgb,
    HslMaxToRgb,
    DesaturationMethodCount
};

typedef TYPE_LIST_4(ImageInputPin, ImageOutputPin, IntInputPin, FloatInputPin) DesaturateFilterPinsTlist ;

class DesaturateFilterNode : public GenericNode<DesaturateFilterPinsTlist>
{
public:
    explicit DesaturateFilterNode();
    virtual ~DesaturateFilterNode();

    // INode
    virtual NodeType Type() override { return Desaturate;}
    virtual FlowData * MainOutput() override { return GetPinData<1>();}

    virtual bool BeforeProcessing() override;
    virtual bool ProcessInternal() override;
    virtual float MemoryConsumption() override;

    //
    void OnDesaturationMethodChanged(int index);
    void OnSaturationValueChanged(int value);
    void OnSaturationSpinBoxValueChanged(double value);

private:
    // Desaturation parameters
    DesaturationMethod _desaturationMode;
    float _desaturationValue;

    //
    void InitNodeTypeComboBox();
    QStringList AvailableDesaturationMethodNames();
    QString AvailableDesaturationMethodName(int index);
    QString AvailableDesaturationMethodName(DesaturationMethod desaturationMethod);

    // constants
    const QString VERTEX_SHADER_PATH =
            ":/shaders/Resources/Shaders/VertexShader.vert";
    const QString CONTRAST_FRAGMENT_SHADER_PATH =
            ":/shaders/Resources/Shaders/Desaturate.frag";

    struct VertexData
    {
        //
        QVector2D position;
        QVector2D texCoord;
    };

    const VertexData FULL_SCREEN_VERTICES_DATA[4] =
    {
        // vertex coord    / input tex coord
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
