#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CommonPreferences.h"
#include "preferencesdialog.h"

#include "openglwidget.h"

#include "ImageProcessors/Scanner.h"

#include "ProcessorFlow.h"
#include "ProcessorFlowDockWidget.h"

#include <QMainWindow>
#include <QImage>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShader>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

#include <QImageReader> //shitty class
#include <QMovie>

#include <QDebug>

/// Icons from _scanned2021_05_03_23_19_50_788

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT       

    struct VertexData
    {
        QVector2D position;
        QVector2D texCoord;
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void OnOutputProcessed(QImage * output);

private slots:
    void on_actionPlay_triggered();
    void on_actionPreferences_triggered();

private:
    Ui::MainWindow *ui;

    // UI
    //QOpenGLWidget _inputWidget;
    //
    //QOpenGLWidget _outputWidget;

    OpenGLWidget* _glWidget;

    ProcessorFlowDockWidget * _processorFlowDockWidget;


    // Preferences
    CommonPreferences _preferences;
    PreferencesDialog _preferencesDialog;

    ProcessorFlow _processorFlow;

    QWidget * _currentPeakWidget;
    void OnPeakNode(Node * node);
};
#endif // MAINWINDOW_H
