# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

# This is a reminder that you are using a generated .pro file.
# Remove it when you are finished editing this file.
message("You are running qmake on a generated .pro file. This may not work!")


HEADERS += ./resource.h \
    ./cocoseditor.h \
    ./ItemInfoTreeDelegate.h \
    ./timeTableItem.h \
    ./TimeLineWidget.h \
    ./TimeTable.h \
    ./FrameItemDelegate.h \
    ./FrameDataModel.h \
    ./ItemInfoList.h \
    ./ItemDataModel.h \
    ./TimeRuler.h \
    ../Classes/DataExchange.h \
    ../Classes/CocosQtPort/CCQGLView.h \
    ../Classes/CocosQtPort/CCQApplication.h \
    ../Classes/CocosQtPort/CCQGLWidget.h \
    ./ItemTreeWidget.h \
    ./ItemTreeDelegate.h \
    ./DataStruct.h \
    ./preInclude.h \
    ./ItemTreeBaseItem.h
SOURCES += ../Classes/DataExchange.cpp \
    ./cocoseditor.cpp \
    ./main.cpp \
    ./ItemInfoList.cpp \
    ./FrameItemDelegate.cpp \
    ./FrameDataModel.cpp \
    ./TimeLineWidget.cpp \
    ./TimeRuler.cpp \
    ./TimeTable.cpp \
    ./TimeTableItem.cpp \
    ../Classes/CocosQtPort/CCQApplication.cpp \
    ../Classes/CocosQtPort/CCQGLView.cpp \
    ../Classes/CocosQtPort/CCQGLWidget.cpp \
    ../Classes/AppDelegate.cpp \
    ../Classes/HelloWorldScene.cpp \
    ./ItemTreeWidget.cpp \
    ./ItemTreeDelegate.cpp \
    ./ItemTreeBaseItem.cpp
FORMS += ./cocoseditor.ui
RESOURCES += cocoseditor.qrc