cmake_minimum_required(VERSION 3.5)

project(FairBudget VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "/Users/adamploof/Qt/6.6.0/macos/lib/cmake")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql Core)

set(PROJECT_SOURCES
        main.cpp
        app.cpp
        app.h
        app.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FairBudget
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        services/entity_manager.h services/entity_manager.cpp
        services/persistence_manager.h services/persistence_manager.cpp
        services/table_formatter.h services/table_formatter.cpp
        widgets/table_widget_interface.h
        widgets/expense_table.h widgets/expense_table.cpp
        widgets/person_table.h widgets/person_table.cpp
        widgets/payment_table.h widgets/payment_table.cpp
        models/model_interface.h
        models/expense.h models/expense.cpp
        models/person.h models/person.cpp
        models/payment.h models/payment.cpp
        repositories/entity_repository.h repositories/entity_repository.cpp
        repositories/expense_repository.h repositories/expense_repository.cpp
        repositories/person_repository.h repositories/person_repository.cpp
        repositories/payment_repository.h repositories/payment_repository.cpp
        forms/add_expense_form.h forms/add_expense_form.cpp forms/add_expense_form.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FairBudget APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(FairBudget SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FairBudget
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(FairBudget PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(FairBudget PRIVATE Qt6::Core)
target_link_libraries(FairBudget PRIVATE Qt6::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FairBudget)
endif()
set_target_properties(FairBudget PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS FairBudget
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FairBudget)
endif()
