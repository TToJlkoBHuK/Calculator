cmake_minimum_required(VERSION 3.10)

# Название проекта
project(MathParserCalculator)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNICODE _UNICODE)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/includes)

file(GLOB SOURCES_MAIN "src/*.cpp")
add_executable(Calculator ${SOURCES_MAIN})

# Создаем папку plugins рядом с exe (куда будем складывать DLL)
add_custom_command(TARGET Calculator PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Calculator>/plugins
)

# Эта функция автоматически настраивает сборку для каждой DLL
function(add_math_plugin PLUGIN_NAME FOLDER_NAME)
    # Путь к исходникам плагина
    set(PLUGIN_DIR "${CMAKE_SOURCE_DIR}/plugins_view/${FOLDER_NAME}")

    # Проверяем, существует ли папка (чтобы CMake не ругался, если плагина нет)
    if(EXISTS "${PLUGIN_DIR}")
        file(GLOB PLUGIN_SOURCES "${PLUGIN_DIR}/*.cpp")

        # Создаем библиотеку (DLL)
        add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES})

        target_include_directories(${PLUGIN_NAME} PRIVATE "${PLUGIN_DIR}")

        target_include_directories(${PLUGIN_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")

        set_target_properties(${PLUGIN_NAME} PROPERTIES OUTPUT_NAME "${PLUGIN_NAME}")

        add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:${PLUGIN_NAME}>
            $<TARGET_FILE_DIR:Calculator>/plugins/
            COMMENT "Copying ${PLUGIN_NAME}.dll to plugins directory..."
        )
        
        message(STATUS "Plugin configured: ${PLUGIN_NAME} from ${FOLDER_NAME}")
    else()
        message(WARNING "Plugin source folder not found: ${PLUGIN_DIR}")
    endif()
endfunction()

# Первый аргумент - имя DLL (без .dll), Второй - имя папки в plugins_view

add_math_plugin("funccos" "funccos")
add_math_plugin("funcsin" "funcsin")
add_math_plugin("funcpow" "funcpow") 
add_math_plugin("funcln"  "funcln")