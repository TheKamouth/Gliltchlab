#include "ScannerFilterNode.h"
#include"ui_ScannerFilterNode.h"

#include <QElapsedTimer>
#include <QDateTime>

#include <algorithm>
#include <math.h>       /* cos */
#define PI 3.14159265

#define CheckGLError() _CheckGLError(__FILE__, __LINE__)

void ScannerFilterNode::_CheckGLError(const char* file, int line)
{
    GLenum err ( _glContext.functions()->glGetError() );

    while ( err != GL_NO_ERROR )
    {
        std::string error;
        switch ( err )
        {
            case GL_INVALID_OPERATION:  error="INVALID_OPERATION";      break;
            case GL_INVALID_ENUM:       error="INVALID_ENUM";           break;
            case GL_INVALID_VALUE:      error="INVALID_VALUE";          break;
            case GL_OUT_OF_MEMORY:      error="OUT_OF_MEMORY";          break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
        }
        std::cout << "GL_" << error.c_str() <<" - " << file << ":" << line << std::endl;
        err = _glContext.functions()->glGetError();
    }

    return;
}

ScannerFilterNode::ScannerFilterNode() :
    ui(new Ui::ScannerFilterNode),
    _glContext(nullptr),
    _glShaderProgram(nullptr),
    _glFrameBufferObject(nullptr)
{
    ui->setupUi(this);

    _scanMode = Rotation360;
    _sensorAnimationMethod = RotateAroundSensorCentered;

    InitSensorAnimationMethodComboBox();
}

QWidget *ScannerFilterNode::NodeUiBaseWidgetInForm()
{
    return ui->NodeUiBase;
}

QLayout *ScannerFilterNode::NodeUiBaseLayoutInForm()
{
    return ui->NodeCommonWidgetLayout;
}

QWidget *ScannerFilterNode::SpecificUI()
{
    return ui->widget;
}

void ScannerFilterNode::SetInput(QImage* input)
{
    Node::SetInput(input);

    _fboSize = _input->size();
}

void ScannerFilterNode::SetParameters()
{

}

bool ScannerFilterNode::BeforeProcessing()
{
    _fboSize = _input->size();

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

    if(_glFrameBufferObject != nullptr)
    {
        delete _glFrameBufferObject ;
    }

    _glFrameBufferObject = new QOpenGLFramebufferObject(_fboSize);
    _glContext.functions()->glViewport(0, 0,_fboSize.width(), _fboSize.height());
    CheckGLError();

    if(_glShaderProgram != nullptr)
    {   // Not necessary after the realease of parent glContext
        //_glShaderProgram->release();

        //delete _glShaderProgram;
    }

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

    if(_inputTexture != nullptr)
    {
        //_inputTexture->release();
        //delete _inputTexture;
    }

    _inputTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    _inputTexture->setData(*_input);
    _inputTexture->bind(0);
    if(!_inputTexture->isBound())
    {
        qDebug() << "Texture not bound.";
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
}

bool ScannerFilterNode::ProcessInternal()
{
    //Scan();

    ScanOneDrawCall();
}

bool ScannerFilterNode::AfterProcessing()
{
    qDebug() << "Scanner node processed, " ;
}

void ScannerFilterNode::ScanOneDrawCall()
{
    if(_input->isNull() == true)
    {
        qDebug() << "No input." << Qt::endl;
        return;
    }

    if(_input->valid(0,0) == false || _input->isNull() == true)
    {
        qDebug()<< "Failed to load image" << Qt::endl;
        return;
    }

    QString vertexPosVar("aPosition");
    QString textureCoordVar("aTexCoord");
    QString textureVar("texture");
    QString texturePFVar("texturePF");

    // TODO : draw multiple lines at once
    // r should be min( height, widht)
    float r = std::max(_fboSize.width(), _fboSize.height())/2.0f;
    float theta = 0.0f;
    float deltaTheta = 2.0 * PI / _fboSize.height();

    QVector2D lineOrigin;
    QVector2D lineEnd;
    lineOrigin = QVector2D( _fboSize.width()/2.0f, _fboSize.height()/2.0f);
    lineEnd = QVector2D( r * cos(theta) + _fboSize.width()/2.0f, r * sin(theta) + _fboSize.height()/2.0f);

    int lineCount = _fboSize.height();

    _glVertexBuffer.allocate(lineCount * 2 * sizeof(VertexData));
    _glFragmentBuffer.allocate(lineCount * 2 * sizeof(GLuint));

    CheckGLError();

    // Sample line and draw it
    VertexData lineData[2 * lineCount];
    GLuint indexData[2 * lineCount];
    for(int i = 0; i < lineCount; i++)
    {
        lineEnd = QVector2D( r * cos(theta) + _fboSize.width()/2.0f, r * sin(theta) + _fboSize.height()/2.0f);
        theta += deltaTheta;

        lineData[i].position = ToVertexCoord(QVector2D(i, 0.0f));
        lineData[i].texCoord = ToTexCoord(lineOrigin);
        indexData[i] = i;
        //_vertexData.at(i).texCoord = ToTexCoord(lineOrigin);

        lineData[i+1].position = ToVertexCoord(QVector2D(i, _fboSize.height()));
        lineData[i+1].texCoord = ToTexCoord(lineEnd);
        indexData[i+1] = i+1;

        // Debug output
        qDebug() << "##########" ;
        qDebug() << i ;
        qDebug() << lineData[i].position.x() << " " << lineData[i].position.y();
        qDebug() << lineData[i].texCoord.x() << " " << lineData[i].texCoord.y();

        qDebug() << "##########" ;
        qDebug() << i +1;
        qDebug() << lineData[i+1].position.x() << " " << lineData[i+1].position.y();
        qDebug() << lineData[i+1].texCoord.x() << " " << lineData[i+1].texCoord.y();
    }

    _glVertexBuffer.write( 0, lineData, 2 * lineCount * sizeof(VertexData));
    CheckGLError();
    _glFragmentBuffer.write( 0, indexData, 2 * lineCount * sizeof(GLuint));

    //_glContext.functions()->glVertexAttribPointer( 0, 1, GL_INT,GL_FALSE, 0, 0);
    CheckGLError();

    _glShaderProgram->enableAttributeArray( vertexPosVar.toLatin1().data());
    CheckGLError();
    _glShaderProgram->enableAttributeArray( textureCoordVar.toLatin1().data());
    CheckGLError();

    _glShaderProgram->setAttributeBuffer(vertexPosVar.toLatin1().data(), GL_FLOAT, offsetof(VertexData, position), 2, sizeof(VertexData));
    CheckGLError();
    _glShaderProgram->setAttributeBuffer(textureCoordVar.toLatin1().data(), GL_FLOAT, offsetof(VertexData, texCoord), 2 , sizeof(VertexData));
    CheckGLError();

    // input bind to 0
    _glShaderProgram->setUniformValue(textureVar.toLatin1().data(), 0);

    _glContext.functions()->glDrawElements(GL_LINES, 2 * lineCount, GL_UNSIGNED_INT, Q_NULLPTR);
    CheckGLError();

    QImage currentOutput(_glFrameBufferObject->toImage(false));
    qDebug() << currentOutput.format();

    QString tempImageOutputFilePath = GetTempImageOutputFilePath();
    _glFrameBufferObject->toImage(false).save(tempImageOutputFilePath);

    qDebug() << "saved in "<< tempImageOutputFilePath << Qt::endl;

    _output = new QImage(_glFrameBufferObject->toImage());
}

QVector2D ScannerFilterNode::ToTexCoord(QVector2D position)
{
    QVector2D normalizedPosition = QVector2D(position);
    normalizedPosition /= QVector2D(_fboSize.width(),_fboSize.height());
    return normalizedPosition;
}

QVector2D ScannerFilterNode::ToVertexCoord(QVector2D position)
{
    QVector2D normalizedPosition = QVector2D(position);
    normalizedPosition -= QVector2D(_fboSize.width()/2.0f,_fboSize.height()/2.0f);
    normalizedPosition /= QVector2D(_fboSize.width()/2.0f,_fboSize.height()/2.0f);
    return normalizedPosition;
}

void ScannerFilterNode::InitSensorAnimationMethodComboBox()
{
    QStringList availableDesaturationMethodNames = AvailableSensorAnimationMethodNames();
    ui->cb_sensorAnimationPreset->addItems(availableDesaturationMethodNames);
}

QStringList ScannerFilterNode::AvailableSensorAnimationMethodNames()
{
    QStringList availableNodeTypesNames;

    for(int i = 0 ; i < SensorAnimationMethodCount ; i++)
    {
        availableNodeTypesNames.append(AvailableSensorAnimationMethodName(i));
    }

    return availableNodeTypesNames;
}

QString ScannerFilterNode::AvailableSensorAnimationMethodName(int index)
{
    return AvailableSensorAnimationMethodName((SensorAnimationMethod)index);
}

QString ScannerFilterNode::AvailableSensorAnimationMethodName(SensorAnimationMethod animationMethod)
{
    switch(animationMethod)
    {
        case RotateAroundSensorCentered:
            return "RotateAroundSensorCentered";

        case RotateAroundSensorEdge:
            return "RotateAroundSensorEdge";

        case TimelineAnimation:
            return "TODO";

        case SensorAnimationMethodCount:
            return "this should not happen";

        default:
            return "Default sensor animation method name";
    }
}
