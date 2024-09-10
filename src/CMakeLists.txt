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
        services/unit_of_work.h services/unit_of_work.cpp
        services/table_formatter.h services/table_formatter.cpp
        services/report_builder.h services/report_builder.cpp
        services/budget_calculator.h services/budget_calculator.cpp
        services/utils.h services/utils.cpp
        entities/entity_interface.h 
        entities/income_period.h entities/income_period.cpp
        entities/expense.h entities/expense.cpp
        entities/person.h entities/person.cpp
        entities/payment.h entities/payment.cpp
        entities/personal_budget.h entities/personal_budget.cpp
        models/expense_model.h models/expense_model.cpp
        models/person_model.h models/person_model.cpp
        models/payment_model.h models/payment_model.cpp
        forms/add_expense_form.h forms/add_expense_form.cpp forms/add_expense_form.ui
        forms/add_person_form.h forms/add_person_form.cpp forms/add_person_form.ui
        forms/add_payment_form.h forms/add_payment_form.cpp forms/add_payment_form.ui
        delegates/entity_choice_delegate.h delegates/entity_choice_delegate.cpp
        delegates/entity_options_loader.h delegates/entity_options_loader.cpp
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
