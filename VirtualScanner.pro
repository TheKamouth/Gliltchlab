QT       += core gui opengl multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml gui multimedia

CONFIG += c++17
#QMAKE_CXXFLAGS += -Werror

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	CommonPreferences.cpp \
	FlowGraph/ConnectionGraphicsItem.cpp \
	FlowGraph/FlowGraph.cpp \
	FlowGraph/FlowGraphScene.cpp \
	FlowGraph/FlowGraphSceneView.cpp \
	FlowGraph/NodeConnection.cpp \
	FlowGraph/NodeGraphicsItem.cpp \
	Nodes/FilterNodes/Desaturate/DesaturateFilterNode.cpp \
	Nodes/FilterNodes/ImageFilterExample.cpp \
	Nodes/FilterNodes/PixelSort/PixelSortFilterNode.cpp \
	Nodes/FilterNodes/PixelSort/core/glilcthlabcore.cpp \
	Nodes/FilterNodes/PixelSort/core/glitchmask.cpp \
	Nodes/FilterNodes/PixelSort/core/glitchparameters.cpp \
	Nodes/FilterNodes/PixelSort/core/glitchsegmentationrule.cpp \
	Nodes/FilterNodes/PixelSort/core/glitchsortingrule.cpp \
	Nodes/FilterNodes/PixelSort/ui/editdirectionwidget.cpp \
	Nodes/FilterNodes/PixelSort/ui/editfunctionwidget.cpp \
	Nodes/FilterNodes/PixelSort/ui/editmaskwidget.cpp \
	Nodes/FilterNodes/PixelSort/ui/editsegmentationwidget.cpp \
	Nodes/FilterNodes/PixelSort/ui/processprogressbar.cpp \
	Nodes/FilterNodes/RgbShiftFilterNode.cpp \
	Nodes/FilterNodes/ScannerFilterNode.cpp \
	Nodes/GenericNode.cpp \
	Nodes/GenericNodePin.cpp \
	Nodes/GenericNodeTest.cpp \
	Nodes/Node.cpp \
	Nodes/NodeCommonWidget.cpp \
	Nodes/NodeFactory.cpp \
	Nodes/ImageInputNode.cpp \
	Nodes/ImageOutputNode.cpp \
	FlowGraph/ProcessorFlowDockWidget.cpp \
	Nodes/TypeList.cpp \
	TimeControlWidget.cpp \
	TimelineWidget.cpp \
	main.cpp \
	mainwindow.cpp \
	openglwidget.cpp \
	preferencesdialog.cpp \


HEADERS += \
	CommonPreferences.h \
	FlowGraph/ConnectionGraphicsItem.h \
	FlowGraph/FlowGraph.h \
	FlowGraph/FlowGraphScene.h \
	FlowGraph/FlowGraphSceneView.h \
	FlowGraph/FlowGraphXmlConstants.h \
	FlowGraph/NodeConnection.h \
	FlowGraph/NodeGraphicsItem.h \
	Nodes/FilterNodes/Desaturate/DesaturateFilterNode.h \
	Nodes/FilterNodes/DesaturateFilterNode.h \
	Nodes/FilterNodes/ImageFilterExample.h \
	Nodes/FilterNodes/PixelSort/PixelSortFilterNode.h \
	Nodes/FilterNodes/PixelSort/core/glilcthlabcore.h \
	Nodes/FilterNodes/PixelSort/core/glitchmask.h \
	Nodes/FilterNodes/PixelSort/core/glitchparameters.h \
	Nodes/FilterNodes/PixelSort/core/glitchsegmentationrule.h \
	Nodes/FilterNodes/PixelSort/core/glitchsortingrule.h \
	Nodes/FilterNodes/PixelSort/ui/editdirectionwidget.h \
	Nodes/FilterNodes/PixelSort/ui/editfunctionwidget.h \
	Nodes/FilterNodes/PixelSort/ui/editmaskwidget.h \
	Nodes/FilterNodes/PixelSort/ui/editsegmentationwidget.h \
	Nodes/FilterNodes/PixelSort/ui/processprogressbar.h \
	Nodes/FilterNodes/RgbShiftFilterNode.h \
	Nodes/FilterNodes/ScannerFilterNode.h \
	Nodes/GenScatterHierarchy.h \
	Nodes/GenericNode.h \
	Nodes/GenericNodePin.h \
	Nodes/GenericNodeTest.h \
	Nodes/ImageInputNode.h \
	Nodes/ImageOutputNode.h \
	Nodes/Node.h \
	Nodes/NodeCommonWidget.h \
	Nodes/NodeFactory.h \
	FlowGraph/ProcessorFlowDockWidget.h \
	Nodes/TypeList.h \
	TimeControlWidget.h \
	TimelineWidget.h \
	mainwindow.h \
	openglwidget.h \
	preferencesdialog.h

FORMS += \
	Nodes/FilterNodes/Desaturate/DesaturateFilterNode.ui \
	Nodes/FilterNodes/PixelSort/PixelSortFilterNode.ui \
	Nodes/FilterNodes/PixelSort/ui/editdirectionwidget.ui \
	Nodes/FilterNodes/PixelSort/ui/editfunctionwidget.ui \
	Nodes/FilterNodes/PixelSort/ui/editmaskwidget.ui \
	Nodes/FilterNodes/PixelSort/ui/editsegmentationwidget.ui \
	Nodes/FilterNodes/PixelSort/ui/processprogressbar.ui \
	Nodes/FilterNodes/RgbShiftFilterNode.ui \
	Nodes/FilterNodes/ScannerFilterNode.ui \
	FlowGraph/ProcessorFlowDockWidget.ui \
	TimeControlWidget.ui \
	TimelineWidget.ui \
	mainwindow.ui \
	preferencesdialog.ui \
	Nodes/ImageInputNode.ui \
	Nodes/ImageOutputNode.ui \
	Nodes/NodeCommonWidget.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
	FlowGraph/FlowGraphFileIO/flow_0.fgf \
	Resources/Images/default.png \
	Shaders/AllRed.frag \
	Shaders/Contrast.frag \
	Shaders/Desaturate.frag \
	Shaders/FullScreenTextureCopy.frag \
	Shaders/Scanner.frag \
	Shaders/VertexShader.vert \
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

RESOURCES += \
	resources.qrc
