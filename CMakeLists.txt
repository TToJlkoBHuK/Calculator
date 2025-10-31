# Требуемая минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта (будет видно в Visual Studio)
project(CalculatorProjectView CXX)

# --- 1. Основное приложение ---
# Cобираем все .cpp и .h файлы из папок cpp и h

file(GLOB APP_SOURCES "cpp/*.cpp")
file(GLOB APP_HEADERS "h/*.h")

# Создаем "фиктивную" цель-приложение. 
# Преподаватель НЕ будет ее собирать, она нужна только для отображения файлов в VS.
add_executable(CalculatorApp ${APP_SOURCES} ${APP_HEADERS})

# Указываем, где лежат заголовочные файлы для этого приложения
target_include_directories(CalculatorApp PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/h"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes"
)

# --- 2. Общие файлы (includes) ---
# Cобираем все .cpp и .h файлы из папки includes

file(GLOB SHARED_SOURCES "includes/*.cpp")
file(GLOB SHARED_HEADERS "includes/*.h")

# Создаем "фиктивную" библиотеку для общего кода
# Это создаст отдельный проект "SharedCode" в решении VS
add_library(SharedCode ${SHARED_SOURCES} ${SHARED_HEADERS})

# Указываем папку 'includes'
target_include_directories(SharedCode PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/includes"
)

# --- 3. Плагины (только исходный код) ---
# Мы создадим "фиктивную" библиотеку для *каждого* плагина.
# Это создаст отдельные проекты в VS (plugin_funccos, plugin_funcln и т.д.),
# чтобы преподаватель мог легко просмотреть код каждого плагина.

# Плагин: funccos
file(GLOB PLUGIN_FUNCCOS_SOURCES "plugins/funccos/*.cpp")
file(GLOB PLUGIN_FUNCCOS_HEADERS "plugins/funccos/*.h")
add_library(plugin_funccos ${PLUGIN_FUNCCOS_SOURCES} ${PLUGIN_FUNCCOS_HEADERS})
target_include_directories(plugin_funccos PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/plugins/funccos")

# Плагин: funcln
file(GLOB PLUGIN_FUNCLN_SOURCES "plugins/funcln/*.cpp")
file(GLOB PLUGIN_FUNCLN_HEADERS "plugins/funcln/*.h")
add_library(plugin_funcln ${PLUGIN_FUNCLN_SOURCES} ${PLUGIN_FUNCLN_HEADERS})
target_include_directories(plugin_funcln PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/plugins/funcln")

# Плагин: funcpow
file(GLOB PLUGIN_FUNCPOW_SOURCES "plugins/funcpow/*.cpp")
file(GLOB PLUGIN_FUNCPOW_HEADERS "plugins/funcpow/*.h")
add_library(plugin_funcpow ${PLUGIN_FUNCPOW_SOURCES} ${PLUGIN_FUNCPOW_HEADERS})
target_include_directories(plugin_funcpow PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/plugins/funcpow")

# Плагин: funcsin
file(GLOB PLUGIN_FUNCSIN_SOURCES "plugins/funcsin/*.cpp")
file(GLOB PLUGIN_FUNCSIN_HEADERS "plugins/funcsin/*.h")
add_library(plugin_funcsin ${PLUGIN_FUNCSIN_SOURCES} ${PLUGIN_FUNCSIN_HEADERS})
target_include_directories(plugin_funcsin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/plugins/funcsin")

# --- 4. Сообщение-инструкция ---
# Добавляем специальную цель, которая просто выводит сообщение в консоль.
# Это напомнит преподавателю, что не нужно нажимать "Build" (Сборка).

add_custom_target(README_Instructions
    COMMAND ${CMAKE_COMMAND} -E echo "=========================================================="
    COMMAND ${CMAKE_COMMAND} -E echo "Этот проект предназначен только для просмотра кода."
    COMMAND ${CMAKE_COMMAND} -E echo "НЕ НАЖИМАЙТЕ 'Build' (Сборка)."
    COMMAND ${CMAKE_COMMAND} -E echo "Для запуска используйте Calculate.exe в корневой папке."
    COMMAND ${CMAKE_COMMAND} -E echo "=========================================================="
    COMMENT "Важные инструкции (см. консоль вывода)"
)