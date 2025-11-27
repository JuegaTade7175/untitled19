cmake_minimum_required(VERSION 3.10)
project(untitled19)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(untitled19
    main.cpp
    src/Unidad.cpp
    src/Edificio.cpp
    src/Mapa.cpp
    src/Contexto.cpp
    src/Evento.cpp
    src/Controlador.cpp
)
