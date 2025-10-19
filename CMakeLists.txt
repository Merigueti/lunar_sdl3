cmake_minimum_required(VERSION 3.16)
project(app LANGUAGES CXX)

# Define o padrão do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Diretórios de include
set(INC_DIRS include)
set(SYS_INC_DIRS external)

# Diretório de código-fonte
set(SRC_DIR src)
set(CORE_DIR core)
set(BUILD_DIR ${CMAKE_BINARY_DIR}/build)

# Encontra SDL3 usando pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3)

# Compila todos os .cpp recursivamente
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

# Cria o executável
add_executable(${PROJECT_NAME} ${SOURCES})

# Inclui diretórios
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${INC_DIRS}
        ${SYS_INC_DIRS}
        ${SDL3_INCLUDE_DIRS}
        ${CORE_DIR}
)

# Inclui todos os diretórios dentro de "external"
file(GLOB_RECURSE EXTERNAL_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/external/*")
foreach(DIR ${EXTERNAL_DIRS})
    if(IS_DIRECTORY ${DIR})
        target_include_directories(${PROJECT_NAME}
            PRIVATE
                ${DIR}
        )
    endif()
endforeach()

# Flags de compilação adicionais
target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        ${SDL3_CFLAGS_OTHER}
)

# Liga as bibliotecas do SDL3
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${SDL3_LIBRARIES}
)

# Define onde o binário final será gerado
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}
)
