#include "DesaturateFilterNode.h"
#include "ui_DesaturateFilterNode.h"

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QDir>

DesaturateFilterNode::DesaturateFilterNode(QWidget *parent) :
    ImageProcessorBase(),
    ui(new Ui::DesaturateFilterNode)
{
    ui->setupUi(this);

    InitNodeTypeComboBox();
}

DesaturateFilterNode::~DesaturateFilterNode()
{
    delete ui;
}

QWidget * DesaturateFilterNode::NodeUiBaseWidgetInForm()
{
    return ui->NodeUiBase;
}

QLayout *DesaturateFilterNode::NodeUiBaseLayoutInForm()
{
    return ui->NodeCommonWidgetLayout;
}

QWidget * DesaturateFilterNode::SpecificUI()
{
    return ui->widget;
}

bool DesaturateFilterNode::TryProcess()
{
    Process();

    return true;
}

void DesaturateFilterNode::SetParameters()
{

}

void DesaturateFilterNode::InitNodeTypeComboBox()
{
    QStringList availableDesaturationMethodNames = AvailableDesaturationMethodNames();
    ui->cb_desaturationMethod->addItems(availableDesaturationMethodNames);
}

QStringList DesaturateFilterNode::AvailableDesaturationMethodNames()
{
    QStringList availableNodeTypesNames;

    for(int i = 0 ; i < DesaturationMethodCount ; i++)
    {
        availableNodeTypesNames.append(AvailableDesaturationMethodName(i));
    }

    return availableNodeTypesNames;
}

QString DesaturateFilterNode::AvailableDesaturationMethodName(int index)
{
    return AvailableDesaturationMethodName((DesaturationMethod)index);
}

QString DesaturateFilterNode::AvailableDesaturationMethodName(DesaturationMethod desaturationMethod)
{
    switch(desaturationMethod)
    {
        case Lightness:
            return "Lightness";

        case Average:
            return "Average";

        case Min:
            return "Min";

        case Max:
            return "Max";

        default:
            return "Default desaturation method name";
    }
}

void DesaturateFilterNode::BeforeProcessing()
{
    _fboSize = _input->size();

    qDebug() << "with "<< Name();

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
    // do this once ?

    if (!_glShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, VERTEX_SHADER_PATH))
    {
        qDebug() << "Can't add vertex shader.";
    }
    if (!_glShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, CONTRAST_FRAGMENT_SHADER_PATH))
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
    _inputTexture->setData(*_input);
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

void DesaturateFilterNode::AfterProcessing()
{
    // Get a temporary dir somwhere ?
    QString dirPath= QDir::currentPath();
    QString timeStamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_z");
    QString outputFilenameNoExtension = "desaturationNodeOutput";

    QString outputFilename = dirPath + '/' + outputFilenameNoExtension + timeStamp + ".png";

    _glFrameBufferObject->toImage(false).save(outputFilename);

    _output = new QImage(outputFilename);

    qDebug() << "Desaturate node processed, "<< outputFilename << " wrote";
}

QImage *DesaturateFilterNode::Output()
{
    return _output;
}

void DesaturateFilterNode::ProcessInternal()
{
    QString vertexPosVar("aPosition");
    QString textureCoordVar("aTexCoord");
    QString textureVar("texture");
    QString texturePFVar("texturePF");

    // Copy previous buffer
    _glVertexBuffer.write(0,FULL_SCREEN_VERTICES_DATA, 4 * sizeof(VertexData));
    _glFragmentBuffer.write(0,FULL_SCREEN_VERTICES_INDEXES, 4 * sizeof(GLuint));

    _desaturationMode = (DesaturationMethod)0;

    _glContext.functions()->glVertexAttribPointer( 0, 1, GL_INT,GL_FALSE, 0, &_desaturationMode);

    int offset = 0;
    _glShaderProgram->enableAttributeArray(vertexPosVar.toLatin1().data());
    _glShaderProgram->enableAttributeArray(textureCoordVar.toLatin1().data());
    _glShaderProgram->setAttributeBuffer(vertexPosVar.toLatin1().data(), GL_FLOAT, offset, 2, sizeof(VertexData));
    offset += sizeof(QVector2D);
    _glShaderProgram->setAttributeBuffer(textureCoordVar.toLatin1().data(), GL_FLOAT, offset, 2, sizeof(VertexData));
    _glShaderProgram->setUniformValue(textureVar.toLatin1().data(), 0);
    _glContext.functions()->glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, Q_NULLPTR);

    _output = new QImage(_glFrameBufferObject->toImage(false));
}

QVector2D DesaturateFilterNode::ToTexCoord(QVector2D position)
{
    QVector2D normalizedPosition = QVector2D(position);
    normalizedPosition /= QVector2D(_fboSize.width(),_fboSize.height());
    return normalizedPosition;
}

QVector2D DesaturateFilterNode::ToVertexCoord(QVector2D position)
{
    QVector2D normalizedPosition = QVector2D(position);
    normalizedPosition -= QVector2D(_fboSize.width()/2.0f,_fboSize.height()/2.0f);
    normalizedPosition /= QVector2D(_fboSize.width()/2.0f,_fboSize.height()/2.0f);
    return normalizedPosition;
}
