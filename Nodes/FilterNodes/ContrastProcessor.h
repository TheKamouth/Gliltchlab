#ifndef CONTRASTPROCESSOR_H
#define CONTRASTPROCESSOR_H

#include "ImageProcessorBase.h"

#include <QString>
#include <QVector2D>

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

class ContrastParameters
{

};

class ContrastProcessor : public ImageProcessorBase
{
public:
    ContrastProcessor();

    virtual QString Name() override { return "Contrast";}
    // Node override
    void SetInput(QImage* input);

    // ImageProcessorBase / ImageFilterNode
    virtual void SetParameters() override;
    virtual bool BeforeProcessing() override;
    virtual bool AfterProcessing() override;
    virtual bool ProcessInternal() override;

private :

    //paramteres
    GLuint _desaturationMode;

    struct VertexData
    {
        QVector2D position;
        QVector2D texCoord;
    };

    enum DesaturationMode
    {
        NoSaturation,
        MaxRGB,
        MinRGB
    };

    // constants
    const QString VERTEX_SHADER_PATH = "D:\\5_PROJETS\\5_DEV\\VirtualScanner\\sources\\VirtualScanner\\ImageProcessors\\VertexShader.vert";
    const QString CONTRAST_FRAGMENT_SHADER_PATH = "D:\\5_PROJETS\\5_DEV\\VirtualScanner\\sources\\VirtualScanner\\ImageProcessors\\Contrast.frag";
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
    QString _inputFilename;

    QVector2D ToTexCoord(QVector2D position);
    QVector2D ToVertexCoord(QVector2D position);

};

#endif // CONTRASTPROCESSOR_H
