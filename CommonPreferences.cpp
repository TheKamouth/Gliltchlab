#include "CommonPreferences.h"

#include <QCoreApplication>
#include <QDir>
#include <QSettings>

CommonPreferences::CommonPreferences(){

    Init();
}

void CommonPreferences::Init(){

    //
    QCoreApplication::setOrganizationName("Kamouth");
    QCoreApplication::setOrganizationDomain("kamouth.com");
    QCoreApplication::setApplicationName("Gliltch Lab");

    QSettings settings;

    bool settingsFound = settings.contains("keepImageRatio");

    if( !settingsFound){

        inputFolder = QDir::currentPath();
        outputFolder = QDir::currentPath();
        keepImageRatio = true;
        fastSave = false;
        displayFramingLines = true;

        SavePreferences();
    }
    else{

        inputFolder = settings.value( "inputFolder").toString();
        outputFolder = settings.value( "outputFolder").toString();
        keepImageRatio = settings.value( "keepImageRatio").toBool();
        fastSave = settings.value( "fastSave").toBool();
        displayFramingLines = settings.value( "displayFramingLines").toBool();
    }

}

void CommonPreferences::ResetDefaultPreferences(){

    inputFolder = QDir::currentPath();
    outputFolder = QDir::currentPath();
    keepImageRatio = true;
    fastSave = false;
    displayFramingLines = true;
}

void CommonPreferences::SavePreferences(){

    QCoreApplication::setOrganizationName("Kamouth");
    QCoreApplication::setOrganizationDomain("kamouth.com");
    QCoreApplication::setApplicationName("Gliltch Lab");

    QSettings settings;
    settings.setValue("inputFolder", inputFolder);
    settings.setValue("outputFolder", outputFolder);
    settings.setValue("keepImageRatio", keepImageRatio);
    settings.setValue("fastSave", fastSave);
    settings.setValue("displayFramingLines", displayFramingLines);
}
