#ifndef FLOWDATA_H
#define FLOWDATA_H

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

    FlowData(){_data.INT    = 666; _type = Int;};
    FlowData(int   e) { _data.INT    = e; _type = Int;}
    FlowData(float e) { _data.FLOAT  = e; _type = Float;}
    FlowData(QString * e) { _data.STRING = e; _type = String;}
    FlowData(QImage * e) { _data.IMAGE = e; _type = Image;}

    void SetType(FlowDataType type){ _type = type; }
    FlowDataType GetType() const { return _type; }
    int GetInt() const { return _data.INT; }
    float GetFloat() const { return _data.FLOAT; }
    QString * GetString() const { return _data.STRING; }
    QImage * GetImage() const { return _data.IMAGE; }

private:
    FlowDataType _type;

    // use QVariant instead ?
    union
    {
        int   INT;
        float FLOAT;
        QString * STRING;
        QImage * IMAGE;
        //QAudio * AUDIO;
        //QVideo * VIDEO;

    }_data;
};
#endif // FLOWDATA_H
