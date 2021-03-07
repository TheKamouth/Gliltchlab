#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShader>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

#include <QImageReader> //shitty class
#include <QMovie>

#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT       

    struct VertexData
    {
        QVector2D position;
        QVector2D texCoord;
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage _inputFrame;
    int currentOutputIndex = 0;

    QSize _fboSize;
    QOpenGLContext _glContext;
    QOffscreenSurface _offScreenSurface;
    QOpenGLShaderProgram _glShaderProgram;
    QOpenGLFramebufferObject * _glFrameBufferObject;
    QOpenGLTexture * _inputTexture ;
    QOpenGLTexture * _previousOutputTexture ;
    QOpenGLBuffer _glVertexBuffer;
    QOpenGLBuffer _glFragmentBuffer;    

    const QString VERTEX_SHADER_PATH = "D:\\5_PROJETS\\5_DEV\\VirtualScanner\\sources\\VirtualScanner\\VertexShader.vert";
    const QString FRAGMENT_SHADER_PATH = "D:\\5_PROJETS\\5_DEV\\VirtualScanner\\sources\\VirtualScanner\\FragmentShader.frag";
    const VertexData FULL_SCREEN_VERTICES_DATA[4] =
    {
        {{ -1.0f, +1.0f }, { 0.0f, 1.0f }}, // top-left
        {{ +1.0f, +1.0f }, { 1.0f, 1.0f }}, // top-right
        {{ -1.0f, -1.0f }, { 0.0f, 0.0f }}, // bottom-left
        {{ +1.0f, -1.0f }, { 1.0f, 0.0f }}  // bottom-right
    };
    const GLuint FULL_SCREEN_VERTICES_INDEXES[4] = { 0, 1, 2, 3 };
    const GLuint ONE_LINE_INDEXES[2] = {0, 1};

    void InitOpenGLContext();
    QImage ScanLine(const QImage& inputFrame, QVector2D lineOrigin, QVector2D lineEnd);
    QVector2D ToTexCoord(QVector2D position);
    QVector2D ToVertexCoord(QVector2D position);
};
#endif // MAINWINDOW_H
