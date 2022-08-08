add_library(SpinningWheel STATIC
    SpinningWheel.cpp
)
target_link_libraries(SpinningWheel PRIVATE Qt6::Widgets)
target_include_directories(SpinningWheel INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
