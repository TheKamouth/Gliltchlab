#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _inputFrame = QImage("D:\\5_PROJETS\\5_DEV\\VirtualScanner\\input.jpg");
    _fboSize = _inputFrame.size();

    if(_inputFrame.valid(0,0) == false || _inputFrame.isNull() == true)
    {
       qDebug()<< "Failed to load image" << Qt::endl;
    }

    InitOpenGLContext();

    QVector2D lineOrigin(50,50);
    QVector2D lineEnd(750,750);
    QImage outputFrame = ScanLine(_inputFrame, lineOrigin, lineEnd);
    outputFrame.save(QString("D:\\5_PROJETS\\5_DEV\\VirtualScanner\\outputFrame.png"));

    qDebug() << "DONE ! " << Qt::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::InitOpenGLContext()
{
    if(!_glContext.create())
    {
        qDebug() << "Can't create GL context.";
    }
    if(_glContext.isValid() == false)
    {
        qDebug() << "GL context is not valid.";
    }

    _offScreenSurface.setFormat(_glContext.format());
    _offScreenSurface.create();
    if(!_offScreenSurface.isValid())
    {
        qDebug() << "Surface not valid.";
    }

    if(!_glContext.makeCurrent(&_offScreenSurface))
    {
        qDebug() << "Can't make context current.";
    }

    _glFrameBufferObject = new QOpenGLFramebufferObject(_fboSize);
    _glContext.functions()->glViewport(0, 0,_fboSize.width(), _fboSize.height());

    _glShaderProgram.setParent(&_glContext);
    if (!_glShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, VERTEX_SHADER_PATH))
    {
        qDebug() << "Can't add vertex shader.";
    }
    if (!_glShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, FRAGMENT_SHADER_PATH))
    {
        qDebug() << "Can't add fragment shader.";
    }
    if (!_glShaderProgram.link())
    {
        qDebug() << "Can't link program.";
    }
    if (!_glShaderProgram.bind())
    {
        qDebug() << "Can't bind program.";
    }

    _inputTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    _inputTexture->setData(_inputFrame);
    _inputTexture->bind(0);
    if(!_inputTexture->isBound())
    {
        qDebug() << "Texture not bound.";
    }

    //previous outputImage
    /*
    QOpenGLTexture texturePF(QOpenGLTexture::Target2D);
    texturePF.setData(previousFrame);
    texturePF.bind(1);
    if(!texturePF.isBound())
    {
        qDebug() << "texturePF not bound.";
    }
    if(!texturePF.isCreated() || !texturePF.isStorageAllocated())
    {
        qDebug() << "texturePF error.";
    }
    */

    _glVertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if(!_glVertexBuffer.create())
    {
        qDebug() << "Can't create vertex buffer.";
    }
    if(!_glVertexBuffer.bind())
    {
        qDebug() << "Can't bind vertex buffer.";
    }

    _glFragmentBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    if(!_glFragmentBuffer.create())
    {
        qDebug() << "Can't create index buffer.";
    }
    if(!_glFragmentBuffer.bind())
    {
        qDebug() << "Can't bind index buffer.";
    }

    _glVertexBuffer.allocate(2 * sizeof(VertexData));
    _glFragmentBuffer.allocate(2 * sizeof(GLuint));

//    _glVertexBuffer.allocate(4 * sizeof(VertexData));
//    _glFragmentBuffer.allocate(4 * sizeof(GLuint));
}

QImage MainWindow::ScanLine(const QImage& inputFrame, QVector2D lineOrigin, QVector2D lineEnd)
{

    QString vertexPosVar("aPosition");
    QString textureCoordVar("aTexCoord");
    QString textureVar("texture");
    QString texturePFVar("texturePF");

    //_inputTexture->setData(inputFrame);

    VertexData lineData[2];
    lineData[0].position = NormalizePosition(QVector2D(currentOutputIndex, 0.0f));
    lineData[0].texCoord = NormalizePosition(lineOrigin);
    lineData[1].position = NormalizePosition(QVector2D(currentOutputIndex, _fboSize.height()));
    lineData[1].texCoord = NormalizePosition(lineEnd);

    _glVertexBuffer.write(0,lineData, 2 * sizeof(VertexData));
    _glFragmentBuffer.write(0,ONE_LINE_INDEXES, 2 * sizeof(GLuint));

    //_glVertexBuffer.write(0,FULL_SCREEN_VERTICES_DATA, 4 * sizeof(VertexData));
    //_glFragmentBuffer.write(0,FULL_SCREEN_VERTICES_INDEXES, 4 * sizeof(GLuint));

    int offset = 0;

    _glShaderProgram.enableAttributeArray(vertexPosVar.toLatin1().data());
    _glShaderProgram.setAttributeBuffer(vertexPosVar.toLatin1().data(), GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    _glShaderProgram.enableAttributeArray(textureCoordVar.toLatin1().data());
    _glShaderProgram.setAttributeBuffer(textureCoordVar.toLatin1().data(), GL_FLOAT, offset, 2, sizeof(VertexData));
    _glShaderProgram.setUniformValue(textureVar.toLatin1().data(), 0);
    //_glShaderProgram.setUniformValue(texturePFVar.toLatin1().data(), 1);

    _glContext.functions()->glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, Q_NULLPTR);
    //_glContext.functions()->glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, Q_NULLPTR);

    currentOutputIndex++;

    return _glFrameBufferObject->toImage(false);
}

QVector2D MainWindow::NormalizePosition(QVector2D position)
{
    QVector2D normalizedPosition = QVector2D(position);
    normalizedPosition -= QVector2D(_fboSize.width()/2.0f,_fboSize.height()/2.0f);
    normalizedPosition /= QVector2D(_fboSize.width()/2.0f,_fboSize.height()/2.0f);
    return normalizedPosition;
}
