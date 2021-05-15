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
#include <QList>
#include <QVector>

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


class ScannerFilterNode : public ImagePeakNode
{
public:
    ScannerFilterNode();

    // Node
    virtual QString Name() override { return "Scanner"; }
    virtual QWidget * Widget() override { return this; };
    virtual QWidget * NodeUiBaseWidgetInForm() override;
    virtual QLayout* NodeUiBaseLayoutInForm() override;
    virtual QWidget * SpecificUI() override;

    // ImageProcessorBase / Filter
    virtual void SetParameters() override;
    virtual bool BeforeProcessing() override;
    virtual bool AfterProcessing() override;
    virtual bool ProcessInternal() override;

    // Node override
    virtual void SetInput(QImage* input) override;

    //
    void ScanOneDrawCall();

private :
    Ui::ScannerFilterNode *ui;

    SensorAnimationMethod _sensorAnimationMethod;

    // constants
    const QString VERTEX_SHADER_PATH = "D:\\5_PROJETS\\5_DEV\\VirtualScanner\\sources\\VirtualScanner\\Shaders\\VertexShader.vert";
    const QString FRAGMENT_SHADER_PATH = "D:\\5_PROJETS\\5_DEV\\VirtualScanner\\sources\\VirtualScanner\\Shaders\\Scanner.frag";
    const GLuint ONE_LINE_INDEXES[2] = {0, 1};
    const GLuint FULL_SCREEN_VERTICES_INDEXES[4] =
    {
        0,
        1,
        2,
        3
    };
    const VertexData FULL_SCREEN_VERTICES_DATA[4] =
    {
        {{ -1.0f, +1.0f }, { 0.0f, 1.0f }}, // top-left
        {{ +1.0f, +1.0f }, { 1.0f, 1.0f }}, // top-right
        {{ -1.0f, -1.0f }, { 0.0f, 0.0f }}, // bottom-left
        {{ +1.0f, -1.0f }, { 1.0f, 0.0f }}  // bottom-right
    };

    QVector<VertexData*> * _vertexData;
    QList<GLuint> * _vertexindexes;

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
    QOpenGLBuffer _glVertexBuffer;
    QOpenGLBuffer _glFragmentBuffer;

    // internal methods
    void ScanLine(QVector2D lineOrigin, QVector2D lineEnd);

    QVector2D ToTexCoord(QVector2D position);
    QVector2D ToVertexCoord(QVector2D position);

    void InitSensorAnimationMethodComboBox();
    QStringList AvailableSensorAnimationMethodNames();
    QString AvailableSensorAnimationMethodName(int index);
    QString AvailableSensorAnimationMethodName(SensorAnimationMethod animationMethod);
    void _CheckGLError(const char *file, int line);
};

#endif // SCANNER_H
