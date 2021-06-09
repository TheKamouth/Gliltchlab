#ifndef FLOWDATA_H
#define FLOWDATA_H

#include "FlowGraphConstants.h"

#include <QString>
#include <QImage>

enum FlowDataType
{
    Int,
    Float,
    String,
    Image
    //Audio,
    //Video,
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

    void SetType(FlowDataType type){ _type = type; }
    FlowDataType GetType() { return _type; }
    int GetInt()
    {
        return _data.INT;
    }
    float GetFloat() { return _data.FLOAT; }
    QString * GetString() { return _data.STRING; }
    QImage * GetImage()
    {
        return _data.IMAGE; }

    QString TypeString()
    {
        switch(_type)
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
