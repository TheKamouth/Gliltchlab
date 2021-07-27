#ifndef COMMONPREFERENCES_H
#define COMMONPREFERENCES_H

#include <QString>


class CommonPreferences
{
public:
    CommonPreferences();

    void Init();
    void ResetDefaultPreferences();
    void SavePreferences();

    QString InputFolder(){ return dataFolder;}
    void SetInputFolder( QString folderName){ dataFolder = folderName;}

    QString OutputFolder(){ return outputFolder;}
    void SetOutputFolder( QString folderName){ outputFolder = folderName;}

    void SetFastSave( bool value){ fastSave = value;}
    bool FastSave(){ return fastSave;}

private:

    QString dataFolder;
    QString outputFolder;

    bool fastSave;
    bool keepImageRatio;
    bool displayFramingLines;
};

#endif // COMMONPREFERENCES_H
