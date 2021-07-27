#ifndef FLOWDATA_H
#define FLOWDATA_H

#include "FlowGraphConstants.h"

#include <QString>
#include <QImage>

const QString DEFAULT_IMAGE_PATH = ":/images/Resources/Images/default.png";

enum FlowDataType
{
    Unassigned,
    Int,
    Float,
    String,
    Image,
    Audio,
    Video,
};

// A struct that wraps data types handled by Nodes
struct FlowData
{
    FlowData()
    {
        _data.INT    = 0;
        _type = Int;
    };
    FlowData(int   e)
    {
        _data.INT    = e;
        _type = Int;
    }
    FlowData(float e)
    {
        _data.FLOAT  = e;
        _type = Float;
    }
    FlowData(QString * e)
    {
        _data.STRING = e;
        _type = String;
    }
    FlowData(QImage * e)
    {
        _data.IMAGE = e;
        _type = Image;
    }

    void SetType(FlowDataType type, bool createStringAndImage = true)
    {
        _type = type;

        switch(_type)
        {
            case Int:
                _data.INT = 0;
                break;
            case Float:
                _data.FLOAT = 0.0f;
                break;
            case String:
                if(createStringAndImage)
                    _data.STRING = new QString("EmptyString");
                break;
            case Image:
                if(createStringAndImage)
                    _data.IMAGE = new QImage(DEFAULT_IMAGE_PATH);
                break;
            default :
                break;
        }
    }

    FlowDataType GetType()
    {
        return _type;
    }

    void SetInt(int value)
    {
        _data.INT = value;
    }
    int GetInt()
    {
        return _data.INT;
    }

    void SetFloat(float value) { _data.FLOAT = value; }
    float GetFloat() { return _data.FLOAT; }
    QString * GetString() { return _data.STRING; }
    void SetImage(QImage *  image)
    {
        _data.IMAGE = image;
    }
    QImage * GetImage()
    {
        return _data.IMAGE;
    }

    static QString TypeString(FlowDataType type)
    {
        switch(type)
        {
            case Int:
                return "int";
            case Float:
                return "float";
            case String:
                return "string";
            case Image:
                return "image";
            default :
                return "unknown flow data type";
        }
    }

    static QColor TypeToColor(FlowDataType type)
    {
        switch(type)
        {
            case Int:
                return PIN_INT_COLOR;
            case Float:
                return PIN_FLOAT_COLOR;
            case String:
                return PIN_STRING_COLOR;
            case Image:
                return PIN_IMAGE_COLOR;
            default :
                return PIN_INT_COLOR;
        }
    }

private:
    FlowDataType _type;

    // use QVariant instead ?
    union
    {
        int INT;
        float FLOAT;
        QString * STRING;
        QImage * IMAGE;
        //QAudio * AUDIO;
        //QVideo * VIDEO;

    }_data;
};
#endif // FLOWDATA_H
