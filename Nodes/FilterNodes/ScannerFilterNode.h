#ifndef SCANNER_H
#define SCANNER_H

#include "ImageProcessorBase.h"

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

enum ScanMode
{
    Rotation360,
    RotationCenteredWide180,
    AnimationFile,
    Count
};

enum SensorAnimationMethod
{
    RotateAroundSensorCentered,
    RotateAroundSensorEdge,

    // TODO
    TimelineAnimation,
    SensorAnimationMethodCount
};

struct VertexData
{
    QVector2D position;
    QVector2D texCoord;
};

namespace Ui {
class ScannerFilterNode;
}


class ScannerFilterNode : public ImageProcessorBase
{
public:
    ScannerFilterNode();

    // Node
    virtual QString Name() override { return "Scanner"; }
    virtual QWidget * Widget() override { return this; };
    virtual QWidget * NodeUiBaseWidgetInForm() override;
    virtual QLayout* NodeUiBaseLayoutInForm() override;
    virtual QWidget * SpecificUI() override;
    virtual bool TryProcess() override;

    // ImageProcessorBase / Filter
    virtual void SetParameters() override;
    virtual void BeforeProcessing() override;
    virtual void AfterProcessing() override;
    virtual void ProcessInternal() override;

    // Node override
    virtual void SetInput(QImage* input) override;

    //
    void Init();

    void Scan();
    void ScanOneDrawCall();

private :
    Ui::ScannerFilterNode *ui;

    SensorAnimationMethod _sensorAnimationMethod;

    // constants
    const QString VERTEX_SHADER_PATH = "D:\\5_PROJETS\\5_DEV\\VirtualScanner\\sources\\VirtualScanner\\Shaders\\VertexShader.vert";
    const QString FRAGMENT_SHADER_PATH = "D:\\5_PROJETS\\5_DEV\\VirtualScanner\\sources\\VirtualScanner\\Shaders\\FragmentShader.frag";
    const VertexData FULL_SCREEN_VERTICES_DATA[4] =
    {
        {{ -1.0f, +1.0f }, { 0.0f, 1.0f }}, // top-left
        {{ +1.0f, +1.0f }, { 1.0f, 1.0f }}, // top-right
        {{ -1.0f, -1.0f }, { 0.0f, 0.0f }}, // bottom-left
        {{ +1.0f, -1.0f }, { 1.0f, 0.0f }}  // bottom-right
    };
    const GLuint FULL_SCREEN_VERTICES_INDEXES[4] = { 0, 1, 2, 3 };
    const GLuint ONE_LINE_INDEXES[2] = {0, 1};

    // parameters
    ScanMode _scanMode;
    //Animation

    // opengl
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

    // internal methods
    void ScanLine(QVector2D lineOrigin, QVector2D lineEnd);

    QVector2D ToTexCoord(QVector2D position);
    QVector2D ToVertexCoord(QVector2D position);

    void InitSensorAnimationMethodComboBox();
    QStringList AvailableSensorAnimationMethodNames();
    QString AvailableSensorAnimationMethodName(int index);
    QString AvailableSensorAnimationMethodName(SensorAnimationMethod animationMethod);
};

#endif // SCANNER_H
