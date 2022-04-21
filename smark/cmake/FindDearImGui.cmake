include(FetchContent)

FetchContent_Declare(DearImGui
  GIT_REPOSITORY https://github.com/ocornut/imgui/
  GIT_TAG v1.87
)

FetchContent_MakeAvailable(DearImGui)

add_library(dearimgui INTERFACE)
target_sources(dearimgui INTERFACE
  ${dearimgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
  ${dearimgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${dearimgui_SOURCE_DIR}/imgui.cpp
  ${dearimgui_SOURCE_DIR}/imgui_widgets.cpp
  ${dearimgui_SOURCE_DIR}/imgui_tables.cpp
  ${dearimgui_SOURCE_DIR}/imgui_draw.cpp
  ${dearimgui_SOURCE_DIR}/imgui_demo.cpp)

target_include_directories(dearimgui INTERFACE ${dearimgui_SOURCE_DIR})
