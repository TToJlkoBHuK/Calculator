cmake_minimum_required(VERSION 3.10)

# Название проекта
project(MathParserCalculator)

# Стандарт C++ (используем 17, так как это современный стандарт)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Говорим CMake, где искать заголовочные файлы (.h)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Собираем список всех .cpp файлов из папки src
file(GLOB SOURCES "src/*.cpp")

# Создаем исполняемый файл
add_executable(Calculator ${SOURCES})

# --- Настройка для Windows ---
if(WIN32)
    # Если используются специфичные либы Windows, их можно добавить здесь. 
    # Обычно kernel32 (где LoadLibrary) линкуется автоматически.
    # Но на всякий случай можно оставить пустым, если MinGW/MSVC сами подтягивают.
endif()

# --- Копирование плагинов ---
# Чтобы программа нашла DLL, они должны лежать рядом с .exe
# Эта команда копирует папку plugins в папку сборки после компиляции.
add_custom_command(TARGET Calculator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/plugins
    $<TARGET_FILE_DIR:Calculator>/plugins
    COMMENT "Copying plugins to output directory..."
)