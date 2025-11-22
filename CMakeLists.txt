cmake_minimum_required(VERSION 3.10)

# Название проекта
project(MathParserCalculator)

# Стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_definitions(UNICODE _UNICODE) 

# 1. Указываем, где лежат заголовки (.h)
include_directories(${CMAKE_SOURCE_DIR}/include)

# 2. Автоматически находим все исходники (.cpp) в папке src
file(GLOB SOURCES "src/*.cpp")

# 3. Создаем исполняемый файл
add_executable(Calculator ${SOURCES})

# 4. Копирование DLL плагинов после сборки
if(WIN32)
    if(EXISTS "${CMAKE_SOURCE_DIR}/plugins")
        add_custom_command(TARGET Calculator POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/plugins
            $<TARGET_FILE_DIR:Calculator>/plugins
            COMMENT "Копирование папки plugins к исполняемому файлу..."
        )
    endif()
endif()
