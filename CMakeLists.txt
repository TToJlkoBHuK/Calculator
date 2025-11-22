cmake_minimum_required(VERSION 3.10)

# 1. Название проекта
project(MathParserCalculator)

# 2. Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. Самое важное: указываем, где искать .h файлы
# Благодаря этому в коде можно писать #include "Header.h" вместо "include/Header.h"
include_directories(${CMAKE_SOURCE_DIR}/include)

# 4. Ищем файлы исходного кода в папке src
file(GLOB SOURCES "src/*.cpp")

# 5. (Опционально) Ищем хедеры, чтобы они красиво отображались в Visual Studio
file(GLOB HEADERS "include/*.h")

# 6. Создаем exe из всех найденных файлов
add_executable(Calculator ${SOURCES} ${HEADERS})

# --- Копирование плагинов ---
if(WIN32)
    # Копируем содержимое папки plugins (если она есть) в папку с .exe
    if(EXISTS "${CMAKE_SOURCE_DIR}/plugins")
        add_custom_command(TARGET Calculator POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/plugins
            $<TARGET_FILE_DIR:Calculator>/plugins
            COMMENT "Копирование плагинов..."
        )
    endif()
endif()