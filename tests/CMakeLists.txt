include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
  fairbudget_tests
  entities/expense_test.cpp
  forms/add_expense_test.cpp
  forms/add_person_test.cpp
  services/utils_test.cpp
  ${CMAKE_SOURCE_DIR}/src/entities/expense.h
  ${CMAKE_SOURCE_DIR}/src/entities/expense.cpp
  ${CMAKE_SOURCE_DIR}/src/entities/person.h
  ${CMAKE_SOURCE_DIR}/src/entities/person.cpp
  ${CMAKE_SOURCE_DIR}/src/entities/income_period.h
  ${CMAKE_SOURCE_DIR}/src/entities/income_period.cpp
  ${CMAKE_SOURCE_DIR}/src/entities/payment.h
  ${CMAKE_SOURCE_DIR}/src/entities/payment.cpp
  ${CMAKE_SOURCE_DIR}/src/forms/add_expense_form.cpp
  ${CMAKE_SOURCE_DIR}/src/forms/add_person_form.cpp
  ${CMAKE_SOURCE_DIR}/src/services/entity_manager.h
  ${CMAKE_SOURCE_DIR}/src/services/entity_manager.cpp
  ${CMAKE_SOURCE_DIR}/src/services/unit_of_work.h
  ${CMAKE_SOURCE_DIR}/src/services/unit_of_work.cpp
  ${CMAKE_SOURCE_DIR}/src/services/utils.h
  ${CMAKE_SOURCE_DIR}/src/services/utils.cpp
)
target_link_libraries(
  fairbudget_tests
  GTest::gtest_main
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt6::Core
  Qt6::Sql
)

include(GoogleTest)
gtest_discover_tests(fairbudget_tests)
