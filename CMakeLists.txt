cmake_minimum_required(VERSION 3.16)
project(editor)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# This assumes the SDL_ttf source is available in vendored/SDL_ttf
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

# Create the editor executable
add_executable(main WIN32 main.c text_buffer.c document.c)

# Link to SDL3 and SDL3_ttf
target_link_libraries(main PRIVATE SDL3_ttf::SDL3_ttf SDL3::SDL3)
