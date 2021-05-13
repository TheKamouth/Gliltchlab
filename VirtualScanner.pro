QT       += core gui opengl multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CommonPreferences.cpp \
    FlowGraph.cpp \
    Nodes/DesaturateFilterNode.cpp \
    Nodes/ImageInputNode.cpp \
    Nodes/ImageOutputNode.cpp \
    Nodes/ImagePeakNode.cpp \
    Nodes/Node.cpp \
    Nodes/NodeCommonWidget.cpp \
    Nodes/NodeFactory.cpp \
    ProcessorCommonUI.cpp \
    ProcessorFlow.cpp \
    ProcessorFlowDockWidget.cpp \
    main.cpp \
    mainwindow.cpp \
    openglwidget.cpp \
    preferencesdialog.cpp \
    ImageProcessors/ContrastProcessor.cpp \
    ImageProcessors/ImageProcessorBase.cpp \
    ImageProcessors/Scanner.cpp

HEADERS += \
    CommonPreferences.h \
    FlowGraph.h \
    ImageProcessorWrapper.h \
    ImageProcessors/ContrastProcessor.h \
    ImageProcessors/ImageProcessorBase.h \
    ImageProcessors/Scanner.h \
    Nodes/DesaturateFilterNode.h \
    Nodes/ImageInputNode.h \
    Nodes/ImageOutputNode.h \
    Nodes/ImagePeakNode.h \
    Nodes/Node.h \
    Nodes/NodeCommonWidget.h \
    Nodes/NodeFactory.h \
    ProcessorCommonUI.h \
    ProcessorFlow.h \
    ProcessorFlowDockWidget.h \
    mainwindow.h \
    openglwidget.h \
    preferencesdialog.h

FORMS += \
    Nodes/DesaturateFilterNode.ui \
    Nodes/ImageInputNode.ui \
    Nodes/ImageOutputNode.ui \
    Nodes/NodeCommonWidget.ui \
    ProcessorCommonUI.ui \
    ProcessorFlowDockWidget.ui \
    mainwindow.ui \
    preferencesdialog.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../ImageProcessors/AllRed.frag \
    ../ImageProcessors/Contrast.frag \
    ../ImageProcessors/FragmentShader.frag \
    ../ImageProcessors/FullScreenTextureCopy.frag \
    ../ImageProcessors/VertexShader.vert \
    Icons/icons8-closed-eye-50.png \
    Icons/icons8-creek-50.png \
    Icons/icons8-delete-bin-50.png \
    Icons/icons8-eye-50-2.png \
    Icons/icons8-eye-50.png \
    Icons/icons8-folder-50.png \
    Icons/icons8-rotate-page-clockwise-50.png \
    Icons/icons8-save-50.png \
    Icons/icons8-settings-50-2.png \
    Icons/icons8-settings-50.png \
    Icons/icons8-update-50.png \
    ImageProcessors/AllRed.frag \
    ImageProcessors/Contrast.frag \
    ImageProcessors/FragmentShader.frag \
    ImageProcessors/FullScreenTextureCopy.frag \
    ImageProcessors/VertexShader.vert

RESOURCES += \
    icons.qrc
