######################################################################
# Automatically generated by qmake (3.1) Thu Mar 27 12:14:22 2025
######################################################################

TEMPLATE = app
TARGET = qt_resume
INCLUDEPATH += .

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_UP_TO=0x060000 # disables all APIs deprecated in Qt 6.0.0 and earlier

# Input
HEADERS += app_utils.h \
           company_add_window.h \
           company_model.h \
           company_window.h \
           database_utils.h \
           first_window.h \
           num_issue_window.h \
           type_qualification_window.h \
           write_resume_window.h
SOURCES += company_add_window.cpp \
           company_window.cpp \
           first_window.cpp \
           main.cpp \
           num_issue_window.cpp \
           type_qualification_window.cpp \
           write_resume_window.cpp \
           cmake-build/JobApplicationManager_autogen/mocs_compilation.cpp \
           build/CMakeFiles/3.31.6/CompilerIdCXX/CMakeCXXCompilerId.cpp \
           cmake-build/CMakeFiles/3.31.6/CompilerIdCXX/CMakeCXXCompilerId.cpp
RESOURCES += resources.qrc
