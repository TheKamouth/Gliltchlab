#include "Scanner.h"

#include <QElapsedTimer>
#include <QDateTime>

#include <math.h>       /* cos */
#define PI 3.14159265
Scanner::Scanner()
{
    _scanMode = Rotation360;
}


void Scanner::Scan()
{
    if(_inputFrame->isNull() == true)
    {
        qDebug() << "No input." << Qt::endl;
        return;
    }

    if(_inputFrame->valid(0,0) == false || _inputFrame->isNull() == true)
    {
        qDebug()<< "Failed to load image" << Qt::endl;
    }

    //Init();

    QElapsedTimer timer;
    timer.start();

    float r = _fboSize.height()/2.0f;
    float theta = 0.0f;
    float deltaTheta = 2.0 * PI / _fboSize.height();

    QVector2D lineOrigin;
    QVector2D lineEnd;

    if(_scanMode == Rotation360)
    {
        lineOrigin = QVector2D(_fboSize.height()/2.0f,_fboSize.height()/2.0f);
        lineEnd = QVector2D(r * cos(theta)+ r,r * sin(theta)+ r);

        for(int i = 0; i < _fboSize.height(); i++)
        {
            lineEnd = QVector2D(r * cos(theta) + r,r * sin(theta) + r);
            ScanLine(lineOrigin, lineEnd);
            theta += deltaTheta;
        }

        lineEnd = QVector2D(r * cos(theta) + r,r * sin(theta) + r);
    }
    else if(_scanMode == RotationCenteredWide180)
    {
        deltaTheta =2.0* PI / _fboSize.height();
        float thetaOrigin = 0.0f;
        float thetaEnd = -PI;

        for(int i = 0; i < _fboSize.height(); i++)
        {
            lineOrigin = QVector2D(r * cos(thetaOrigin) + r,r * sin(thetaOrigin) + r);
            lineEnd = QVector2D(r * cos(thetaEnd) + r,r * sin(thetaEnd) + r);

            ScanLine(lineOrigin, lineEnd);

            thetaOrigin += deltaTheta;
            thetaEnd += deltaTheta;
        }

        lineOrigin = QVector2D(r * cos(thetaOrigin) + r,r * sin(thetaOrigin) + r);
        lineEnd = QVector2D(r * cos(thetaEnd) + r,r * sin(thetaEnd) + r);
    }

    ScanLine(lineOrigin, lineEnd);

    qDebug() << "Took " << timer.elapsed() << " ms to scan." << Qt::endl;

    QString timeStamp = QDateTime::currentDateTime().toString();
    QString inputFilenameNoExtension = _inputFilename.left(_inputFilename.length()-4);
    QString outputFilename = QString(inputFilenameNoExtension + "_scanned" + timeStamp + ".png");
    _glFrameBufferObject->toImage(false).save(outputFilename);
    qDebug() << "saved in "<< outputFilename << Qt::endl;

    qDebug() << "DONE ! " << Qt::endl;
}

void Scanner::SetInput(QImage* input)
{
    _inputFrame = input;
    _fboSize = _inputFrame->size();

    Init();
}

void Scanner::SetParameters()
{

}

void Scanner::BeforeProcessing()
{

}

void Scanner::AfterProcessing()
{

}

void Scanner::ProcessInternal()
{
    ScanOneDrawCall();
}

QImage * Scanner::Output()
{
    return _output;
}

void Scanner::ScanLine(QVector2D lineOrigin, QVector2D lineEnd)
{

    QString vertexPosVar("aPosition");
    QString textureCoordVar("aTexCoord");
    QString textureVar("texture");
    QString texturePFVar("texturePF");

    // Copy previous buffer
    _glVertexBuffer.write(0,FULL_SCREEN_VERTICES_DATA, 4 * sizeof(VertexData));
    _glFragmentBuffer.write(0,FULL_SCREEN_VERTICES_INDEXES, 4 * sizeof(GLuint));

    int offset = 0;
    _glShaderProgram->enableAttributeArray(vertexPosVar.toLatin1().data());
    _glShaderProgram->enableAttributeArray(textureCoordVar.toLatin1().data());
    _glShaderProgram->setAttributeBuffer(vertexPosVar.toLatin1().data(), GL_FLOAT, offset, 2, sizeof(VertexData));
    offset += sizeof(QVector2D);
    _glShaderProgram->setAttributeBuffer(textureCoordVar.toLatin1().data(), GL_FLOAT, offset, 2, sizeof(VertexData));
    _glShaderProgram->setUniformValue(textureVar.toLatin1().data(), 1);
    _glContext.functions()->glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, Q_NULLPTR);

    // TODO : draw multiple lines at once
    // Sample line and draw it
    VertexData lineData[2];
    lineData[0].position = ToVertexCoord(QVector2D(currentOutputIndex, 0.0f));
    lineData[0].texCoord = ToTexCoord(lineOrigin);
    lineData[1].position = ToVertexCoord(QVector2D(currentOutputIndex, _fboSize.height()));
    lineData[1].texCoord = ToTexCoord(lineEnd);

    _glVertexBuffer.write(0,lineData, 2 * sizeof(VertexData));
    _glFragmentBuffer.write(0,ONE_LINE_INDEXES, 2 * sizeof(GLuint));

    offset = 0;
    _glShaderProgram->enableAttributeArray(vertexPosVar.toLatin1().data());
    _glShaderProgram->enableAttributeArray(textureCoordVar.toLatin1().data());
    _glShaderProgram->setAttributeBuffer(vertexPosVar.toLatin1().data(), GL_FLOAT, offset, 2, sizeof(VertexData));
    offset += sizeof(QVector2D);
    _glShaderProgram->setAttributeBuffer(textureCoordVar.toLatin1().data(), GL_FLOAT, offset, 2, sizeof(VertexData));
    _glShaderProgram->setUniformValue(textureVar.toLatin1().data(), 0);

    _glContext.functions()->glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, Q_NULLPTR);

    currentOutputIndex++;

    _output = new QImage(_glFrameBufferObject->toImage(false));
    qDebug() << _output->format();
    qDebug() << _previousOutputTexture->format();

    _previousOutputTexture->setData(*_output);
}

void Scanner::ScanOneDrawCall()
{
    if(_inputFrame->isNull() == true)
    {
        qDebug() << "No input." << Qt::endl;
        return;
    }

    if(_inputFrame->valid(0,0) == false || _inputFrame->isNull() == true)
    {
        qDebug()<< "Failed to load image" << Qt::endl;
        return;
    }

    QString vertexPosVar("aPosition");
    QString textureCoordVar("aTexCoord");
    QString textureVar("texture");
    QString texturePFVar("texturePF");

    // Copy previous buffer
    /*
    _glVertexBuffer.write(0,FULL_SCREEN_VERTICES_DATA, 4 * sizeof(VertexData));
    _glFragmentBuffer.write(0,FULL_SCREEN_VERTICES_INDEXES, 4 * sizeof(GLuint));

    int offset = 0;

    _glShaderProgram->enableAttributeArray(vertexPosVar.toLatin1().data());
    _glShaderProgram->enableAttributeArray(textureCoordVar.toLatin1().data());
    _glShaderProgram->setAttributeBuffer(vertexPosVar.toLatin1().data(), GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    _glShaderProgram->setAttributeBuffer(textureCoordVar.toLatin1().data(), GL_FLOAT, offset, 2, sizeof(VertexData));

    _glShaderProgram->setUniformValue(textureVar.toLatin1().data(), 1);

    _glContext.functions()->glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, Q_NULLPTR);
    */

    // TODO : draw multiple lines at once
    float r = _fboSize.height()/2.0f;
    float theta = 0.0f;
    float deltaTheta = 2.0 * PI / _fboSize.height();

    QVector2D lineOrigin;
    QVector2D lineEnd;
    int lineCount = _fboSize.height();

    lineOrigin = QVector2D(_fboSize.height()/2.0f,_fboSize.height()/2.0f);
    lineEnd = QVector2D(r * cos(theta)+ r,r * sin(theta)+ r);

    // Sample line and draw it
    VertexData lineData[2 * lineCount];
    GLuint indexData[2 * lineCount];

    for(int i = 0; i < lineCount; i++)
    {
        lineEnd = QVector2D(r * cos(theta) + r,r * sin(theta) + r);
        lineData[i].position = ToVertexCoord(QVector2D(currentOutputIndex, 0.0f));
        lineData[i].texCoord = ToTexCoord(lineOrigin);
        indexData[i] = i;

        lineData[i+1].position = ToVertexCoord(QVector2D(currentOutputIndex, _fboSize.height()));
        lineData[i+1].texCoord = ToTexCoord(lineEnd);
        indexData[i+1] = i+1;

        theta += deltaTheta;
        currentOutputIndex++;
    }

    _glVertexBuffer.write(0,lineData, 2 * sizeof(VertexData) * lineCount);
    _glFragmentBuffer.write(0,indexData, 2 * sizeof(GLuint) * lineCount);

    int offset = 0;
    _glShaderProgram->enableAttributeArray(vertexPosVar.toLatin1().data());
    _glShaderProgram->enableAttributeArray(textureCoordVar.toLatin1().data());
    _glShaderProgram->setAttributeBuffer(vertexPosVar.toLatin1().data(), GL_FLOAT, offset, 2 * lineCount, sizeof(VertexData));

    offset += sizeof(QVector2D);

    _glShaderProgram->setAttributeBuffer(textureCoordVar.toLatin1().data(), GL_FLOAT, offset, 2 * lineCount, sizeof(VertexData));
    _glShaderProgram->setUniformValue(textureVar.toLatin1().data(), 0);

    _glContext.functions()->glDrawElements(GL_LINES,2, GL_UNSIGNED_INT, Q_NULLPTR);

    QImage currentOutput(_glFrameBufferObject->toImage(false));
    qDebug() << currentOutput.format();
    qDebug() << _previousOutputTexture->format();

    _previousOutputTexture->setData(currentOutput);

    QString timeStamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_z");
    QString inputFilenameNoExtension = _inputFilename.left(_inputFilename.length()-4);
    QString outputFilename = QString(inputFilenameNoExtension + "_scanned" + timeStamp + ".png");

    _glFrameBufferObject->toImage(false).save(outputFilename);

    qDebug() << "saved in "<< outputFilename << Qt::endl;

    qDebug() << "DONE ! " << Qt::endl;
}

void Scanner::Init()
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

    _glShaderProgram = new QOpenGLShaderProgram();
    _glShaderProgram->setParent(&_glContext);
    if (!_glShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, VERTEX_SHADER_PATH))
    {
        qDebug() << "Can't add vertex shader.";
    }
    if (!_glShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, FRAGMENT_SHADER_PATH))
    {
        qDebug() << "Can't add fragment shader.";
    }
    if (!_glShaderProgram->link())
    {
        qDebug() << "Can't link program.";
    }
    if (!_glShaderProgram->bind())
    {
        qDebug() << "Can't bind program.";
    }

    _inputTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    _inputTexture->setData(*_inputFrame);
    _inputTexture->bind(0);
    if(!_inputTexture->isBound())
    {
        qDebug() << "Texture not bound.";
    }

    //previous outputImage
    _previousOutputTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    QImage *blackImage = new QImage(_fboSize,QImage::Format_ARGB32_Premultiplied);
    blackImage->fill(Qt::black);
    _previousOutputTexture->setData(*blackImage);
    _previousOutputTexture->bind(1);
    //_glContext.functions()->glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,1,0);

    if(!_previousOutputTexture->isBound())
    {
        qDebug() << "texturePF not bound.";
    }
    if(!_previousOutputTexture->isCreated() || !_previousOutputTexture->isStorageAllocated())
    {
        qDebug() << "texturePF error.";
    }


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

    _glVertexBuffer.allocate(4 * sizeof(VertexData));
    _glFragmentBuffer.allocate(4 * sizeof(GLuint));

    //    _glVertexBuffer.allocate(4 * sizeof(VertexData));
    //    _glFragmentBuffer.allocate(4 * sizeof(GLuint));
}

void Scanner::LoadFile(QString filename)
{
    _inputFilename = filename;

    _inputFrame = new QImage( _inputFilename);
    _fboSize = _inputFrame->size();

    Init();
}

QVector2D Scanner::ToTexCoord(QVector2D position)
{
    QVector2D normalizedPosition = QVector2D(position);
    normalizedPosition /= QVector2D(_fboSize.width(),_fboSize.height());
    return normalizedPosition;
}

QVector2D Scanner::ToVertexCoord(QVector2D position)
{
    QVector2D normalizedPosition = QVector2D(position);
    normalizedPosition -= QVector2D(_fboSize.width()/2.0f,_fboSize.height()/2.0f);
    normalizedPosition /= QVector2D(_fboSize.width()/2.0f,_fboSize.height()/2.0f);
    return normalizedPosition;
}
