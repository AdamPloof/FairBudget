include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
  expense_test
  entities/expense_test.cpp
  ${CMAKE_SOURCE_DIR}/src/entities/expense.h
  ${CMAKE_SOURCE_DIR}/src/entities/expense.cpp
)
target_link_libraries(
  expense_test
  GTest::gtest_main
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt6::Core
  Qt6::Sql
)

include(GoogleTest)
gtest_discover_tests(expense_test)
