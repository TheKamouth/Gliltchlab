#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CommonPreferences.h"
#include "preferencesdialog.h"

#include "openglwidget.h"
#include "TimeControlWidget.h"
#include "TimelineWidget.h"

#include "Nodes/FilterNodes/ScannerFilterNode.h"

#include "FlowGraph/ProcessorFlowDockWidget.h"

#include <QMainWindow>
#include <QImage>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShader>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QMovie>
#include <QDebug>
#include <QProcess>
#include <QTimer>

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

    void OnOutputProcessed();
    void OnViewFlowWidgetTriggered(bool checked);
    void OnViewTimeControlsTriggered(bool checked);
    void OnViewTimeLineTriggered(bool checked);
    void OnShowDebugConsoleTriggered(bool checked);

private slots:
    void on_actionPreferences_triggered();
    void OnNewFlowGraphFileTriggered();
    void OnLoadFlowGraphFileTriggered();
    void OnSaveFlowGraphFileTriggered();
    void OnReadTimerTimout();

private:
    Ui::MainWindow *ui;

    ProcessorFlowDockWidget * _flowGraphDockWidget;
    TimeControlWidget _timeControlWidget;
    TimelineWidget * _timeLineWidget;

    // Preferences
    CommonPreferences _preferences;
    PreferencesDialog _preferencesDialog;

    void OnPeakNode(Node * node);
    void OnNodeOutputChanged(Node * node);
    void OnFlowGraphProcessed();

    Node * _nodePeaked;

    // A widget per observable output type ?
    OpenGLWidget _glWidget;
    //QVideoWidget _videoWidget;
    //QAudioWidget _audioWidget;

    // TODO:

    // Main gliltchlab process.
    // Handling everything right now (UI and processing)
    QProcess _gliltchlabMainProcess;
    QTimer _readOutputTimer;

};
#endif // MAINWINDOW_H
