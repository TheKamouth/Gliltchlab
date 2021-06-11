#include "DesaturateFilterNode.h"
#include "ui_DesaturateFilterNode.h"

#include "Nodes/NodeCommonWidget.h"

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QWidget>
#include <QComboBox>
#include <QSlider>
#include <QDoubleSpinBox>

DesaturateFilterNode::DesaturateFilterNode() :
    _desaturationValue(0.5f)
{
    InitNodeTypeComboBox();
    //QObject::connect( ui->cb_desaturationMethod, QOverload<int>::of(&QComboBox::activated), this, &DesaturateFilterNode::OnDesaturationMethodChanged);
    //QObject::connect( ui->sl_desaturationValue, &QSlider::valueChanged, this, &DesaturateFilterNode::OnSaturationValueChanged);
    //QObject::connect( ui->dsb_desaturationValue, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &DesaturateFilterNode::OnSaturationSpinBoxValueChanged);
}

DesaturateFilterNode::~DesaturateFilterNode()
{
}

void DesaturateFilterNode::InitNodeTypeComboBox()
{
    QStringList availableDesaturationMethodNames = AvailableDesaturationMethodNames();
    //ui->cb_desaturationMethod->addItems(availableDesaturationMethodNames);
}

void DesaturateFilterNode::OnDesaturationMethodChanged(int index)
{
    _desaturationMode = (DesaturationMethod)index;

     emit NodeInputChanged(this);
}

void DesaturateFilterNode::OnSaturationValueChanged(int value)
{
    //float sliderValue = ui->sl_desaturationValue->value();
    _desaturationValue = value != 0.0f ? value / 100.0f : 0.0f;
    //ui->dsb_desaturationValue->setValue( _desaturationValue);
    qDebug() << _desaturationValue;

    emit NodeInputChanged(this);
}

void DesaturateFilterNode::OnSaturationSpinBoxValueChanged(double value)
{
    //_desaturationValue = ui->dsb_desaturationValue->value();

    //ui->sl_desaturationValue->setValue(value * 100.0f);
    qDebug() << _desaturationValue;

    emit NodeInputChanged(this);
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

        case HslAverageToRgb:
            return "HslAverageToRgb";

        case HslMinToRgb:
            return "HslMinToRgb";

        case HslMaxToRgb:
            return "HslMaxToRgb";

        default:
            return "Default desaturation method name";
    }
}

bool DesaturateFilterNode::BeforeProcessing()
{
    //_fboSize = _input->size();
    FlowData * data = GetPinData<0>();
    QImage * inputImage = data->GetImage();
    _fboSize = inputImage->size();

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
    _inputTexture->setData(*inputImage);
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

    return true;
}

bool DesaturateFilterNode::ProcessInternal()
{
    QString vertexPosVar("aPosition");
    QString textureCoordVar("aTexCoord");
    QString textureVar("texture");
    QString texturePFVar("texturePF");
    QString desaturationMode("desaturationMode");
    QString desaturationValue("desaturationValue");

    // Copy previous buffer
    _glVertexBuffer.write(0, FULL_SCREEN_VERTICES_DATA, 4 * sizeof(VertexData));
    _glFragmentBuffer.write(0, FULL_SCREEN_VERTICES_INDEXES, 4 * sizeof(GLuint));

    //_glContext.functions()->glVertexAttribPointer( 0, 1, GL_INT,GL_FALSE, 0, &_desaturationMode);

    _glShaderProgram->enableAttributeArray(vertexPosVar.toLatin1().data());
    _glShaderProgram->enableAttributeArray(textureCoordVar.toLatin1().data());

    _glShaderProgram->setAttributeBuffer(vertexPosVar.toLatin1().data(), GL_FLOAT, offsetof(VertexData, position), 2/**/, sizeof(VertexData));
    _glShaderProgram->setAttributeBuffer(textureCoordVar.toLatin1().data(), GL_FLOAT, offsetof(VertexData, texCoord), 2/**/, sizeof(VertexData));

    // Getting parameters
    // This could be done in BeforeProcessing()
    _desaturationMode = (DesaturationMethod)GetDataPinAt<2>()->GetData()->GetInt();

    // ugly call
    _desaturationValue = GetDataPinAt<3>()->GetData()->GetFloat();

    // Setting uniforms/parameters
    _glShaderProgram->setUniformValue(textureVar.toLatin1().data(), 0);
    _glShaderProgram->setUniformValue(desaturationMode.toLatin1().data(), (int)_desaturationMode);
    _glShaderProgram->setUniformValue(desaturationValue.toLatin1().data(), _desaturationValue);

    _glContext.functions()->glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, Q_NULLPTR);


    IDataPin * outputDataPin = GetDataPinAt<1>();
    outputDataPin->SetData( new FlowData( new QImage( _glFrameBufferObject->toImage(false))));

    return true;
}

float DesaturateFilterNode::MemoryConsumption()
{
    // this does not include heap memory....
    return sizeof (DesaturateFilterNode);
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
