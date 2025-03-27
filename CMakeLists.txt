cmake_minimum_required(VERSION 3.14)

project(JobApplicationManager LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt Components
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Sql)

# Source files
set(PROJECT_SOURCES
    main.cpp
    first_window.cpp
    company_add_window.cpp
    company_window.cpp
    write_resume_window.cpp
    num_issue_window.cpp
    type_qualification_window.cpp
    
    # Header files
    first_window.h
    company_add_window.h
    company_window.h
    write_resume_window.h
    num_issue_window.h
    type_qualification_window.h
    company_model.h
    database_utils.h
)

# Create executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(JobApplicationManager
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(JobApplicationManager
        ${PROJECT_SOURCES}
    )
endif()

# Link Qt libraries
target_link_libraries(JobApplicationManager PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# Set platform specific settings
if(WIN32)
    set_target_properties(JobApplicationManager PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
elseif(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.jobapplicationmanager"
    )
endif()

# Finalize (required for Qt6)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_finalize_executable(JobApplicationManager)
endif()
