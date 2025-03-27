QT += core gui widgets sql

CONFIG += c++17

SOURCES += \
    main.cpp \
    company_add_window.cpp \
    company_window.cpp \
    first_window.cpp \
    num_issue_window.cpp \
    type_qualification_window.cpp \
    write_resume_window.cpp

HEADERS += \
    company_add_window.h \
    company_window.h \
    first_window.h \
    num_issue_window.h \
    type_qualification_window.h \
    write_resume_window.h \
    company_model.h \
    database_utils.h

TARGET = JobApplicationManager
TEMPLATE = app
