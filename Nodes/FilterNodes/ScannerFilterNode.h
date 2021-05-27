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

// Get rid of one og those:
enum ScanMode
{
    Rotation360,
    RotationCenteredWide180,
    AnimationFile,
    Count
};

enum SensorAnimationMethod
{
    RotateAroundSensorEdge,
    RotateAroundSensorCentered,

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


class ScannerFilterNode : public Node
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
    virtual bool TryReadInputs() override;
    virtual bool BeforeProcessing() override;
    virtual bool ProcessInternal() override;

    // Node override
    virtual void SetInput(QImage* input) override;

    //
    void ScanOneDrawCall();

    void OnSensorAnimationPresetCurrentIndexChanged(int index);

private :
    Ui::ScannerFilterNode *ui;

    SensorAnimationMethod _sensorAnimationMethod;

    // constants
    const QString VERTEX_SHADER_PATH = ":/shaders/Resources/Shaders/VertexShader.vert";
    const QString FRAGMENT_SHADER_PATH = ":/shaders/Resources/Shaders/Scanner.frag";

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
