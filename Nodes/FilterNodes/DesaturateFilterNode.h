#ifndef CONTRASTFILTERNODE_H
#define CONTRASTFILTERNODE_H

#include "Nodes/Node.h"

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
    HslAverageToRgb,
    HslMinToRgb,
    HslMaxToRgb,
    DesaturationMethodCount
};

class DesaturateFilterNode : public Node
{
    Q_OBJECT

public:
    explicit DesaturateFilterNode();
    virtual ~DesaturateFilterNode();

    // Node
    virtual QString Name() override { return "Desaturate"; }
    virtual QWidget * Widget() override { return this; };
    virtual QWidget * NodeUiBaseWidgetInForm() override;
    virtual QLayout* NodeUiBaseLayoutInForm() override;
    virtual QWidget * SpecificUI() override;
    virtual QLayout * SpecificUiLayout() override;

    // ImageProcessorBase / Filter
    virtual bool TryReadInputs() override;
    virtual bool BeforeProcessing() override;
    virtual bool ProcessInternal() override;

    //
    void OnDesaturationMethodChanged(int index);
    void OnSaturationValueChanged(int value);
    void OnSaturationSpinBoxValueChanged(double value);

private:
    Ui::DesaturateFilterNode * ui;

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
