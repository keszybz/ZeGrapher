#-------------------------------------------------
#
# Project created by QtCreator 2011-03-25T16:51:48
#
#-------------------------------------------------


QT += widgets printsupport webkitwidgets

TARGET = ZeGrapher
TEMPLATE = app
TRANSLATIONS = ZeGrapher_fr.ts
CONFIG += c++11
QMAKE_LFLAGS_RELEASE = -s

OBJECTS_DIR = .obj
MOC_DIR = .moc
UI_DIR = .ui

INCLUDEPATH += .

SOURCES += \
    main.cpp \
    information.cpp \
    Windows/about.cpp \
    Widgets/tangentwidget.cpp \
    Widgets/straightlinewidget.cpp \
    Widgets/seqwidget.cpp \
    Widgets/qcolorbutton.cpp \
    Widgets/pareqwidget.cpp \
    Widgets/pareqcontroller.cpp \
    Widgets/parconfwidget.cpp \
    Widgets/keyboard.cpp \
    Widgets/integrationwidget.cpp \
    Widgets/funcwidget.cpp \
    Widgets/abstractfuncwidget.cpp \
    ValuesTable/valuestableconf.cpp \
    ValuesTable/valuestable.cpp \
    ValuesTable/seqtable.cpp \
    ValuesTable/pareqtable.cpp \
    ValuesTable/functable.cpp \
    ValuesTable/abstracttable.cpp \
    GraphDraw/printpreview.cpp \
    GraphDraw/maingraph.cpp \
    GraphDraw/imagepreview.cpp \
    GraphDraw/graphdraw.cpp \
    Export/print.cpp \
    Export/imagesave.cpp \
    DataPlot/rowselectorwidget.cpp \
    DataPlot/rowactionswidget.cpp \
    DataPlot/datawindow.cpp \    
    DataPlot/datatable.cpp \
    DataPlot/columnselectorwidget.cpp \
    DataPlot/columnactionswidget.cpp \
    MathObjects/seqcalculator.cpp \
    MathObjects/funcvaluessaver.cpp \
    MathObjects/colorsaver.cpp \
    Widgets/datawidget.cpp \
    DataPlot/csvhandler.cpp \
    MathObjects/polynomial.cpp \
    MathObjects/polynomialregression.cpp \
    MathObjects/regression.cpp \
    MathObjects/regressionvaluessaver.cpp \
    DataPlot/modelchoicewidget.cpp \
    DataPlot/modelwidget.cpp \
    DataPlot/polynomialmodelwidget.cpp \
    Widgets/numberlineedit.cpp \
    Windows/rangeadjustments.cpp \
    Windows/mathobjectsinput.cpp \
    Windows/settings.cpp \
    Windows/values.cpp \
    Windows/updatecheck.cpp \
    Windows/mainwindow.cpp \
    MathObjects/ZeExpression.cpp \
    MathObjects/ZeSet.cpp \
    MathObjects/ZeFunction.cpp \
    MathObjects/ZeConstant.cpp

HEADERS  += \
    Structures.h \
    information.h \
    Windows/about.h \
    Widgets/tangentwidget.h \
    Widgets/straightlinewidget.h \
    Widgets/seqwidget.h \
    Widgets/qcolorbutton.h \
    Widgets/pareqwidget.h \
    Widgets/pareqcontroller.h \
    Widgets/parconfwidget.h \
    Widgets/keyboard.h \
    Widgets/integrationwidget.h \
    Widgets/funcwidget.h \
    Widgets/abstractfuncwidget.h \
    ValuesTable/valuestableconf.h \
    ValuesTable/valuestable.h \
    ValuesTable/seqtable.h \
    ValuesTable/pareqtable.h \
    ValuesTable/functable.h \
    ValuesTable/abstracttable.h \
    GraphDraw/printpreview.h \
    GraphDraw/maingraph.h \
    GraphDraw/imagepreview.h \
    GraphDraw/graphdraw.h \
    Export/print.h \
    Export/imagesave.h \
    DataPlot/rowselectorwidget.h \
    DataPlot/rowactionswidget.h \
    DataPlot/datawindow.h \   
    DataPlot/datatable.h \
    DataPlot/columnselectorwidget.h \
    DataPlot/columnactionswidget.h \
    MathObjects/seqcolorssaver.h \
    MathObjects/seqcalculator.h \
    MathObjects/funcvaluessaver.h \
    MathObjects/colorsaver.h \
    MathObjects/MathObjectsdefines.h \
    Widgets/datawidget.h \
    DataPlot/csvhandler.h \
    MathObjects/polynomial.h \
    MathObjects/polynomialregression.h \
    MathObjects/regression.h \
    MathObjects/regressionvaluessaver.h \
    DataPlot/modelchoicewidget.h \
    DataPlot/modelwidget.h \
    DataPlot/polynomialmodelwidget.h \
    Widgets/numberlineedit.h \
    Windows/rangeadjustments.h \
    Windows/mathobjectsinput.h \
    Windows/settings.h \
    Windows/values.h \
    Windows/updatecheck.h \
    Windows/mainwindow.h \
    MathObjects/ZeExpression.h \
    MathObjects/ZeSet.h \
    MathObjects/ZeFunction.h \
    MathObjects/ZeConstant.h


FORMS    += \
    Windows/about.ui \
    Widgets/datawidget.ui \
    Export/print.ui \
    Export/imagesave.ui \
    DataPlot/startingactions.ui \
    DataPlot/sortoptions.ui \
    DataPlot/filloptions.ui \
    DataPlot/datawindow.ui \   
    DataPlot/confirmdelete.ui \
    DataPlot/csvconfig.ui \
    DataPlot/modelchoicewidget.ui \
    DataPlot/polynomialmodelwidget.ui \
    Widgets/keyboard.ui \
    Windows/rangeadjustments.ui \
    Windows/mathobjectsinput.ui \
    Windows/settings.ui


RESOURCES += \
    ressources.qrc

RC_ICONS = appicon.ico

OTHER_FILES += \
    TODO.txt


