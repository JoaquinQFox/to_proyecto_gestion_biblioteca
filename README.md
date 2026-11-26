# Sistema de Gestión de Biblioteca

Aplicación de escritorio para administrar categorías, libros, ejemplares, lectores y préstamos, con un módulo de recomendaciones. Está desarrollada en C++17 con Qt Widgets y guarda la información en SQLite.

## Requisitos

- CMake 3.16 o superior.
- Un compilador de C++17, como GCC, Clang o MSVC.
- Qt 5 o Qt 6 con los módulos Widgets, Sql, Concurrent y Test.
- El controlador SQLite de Qt (`QSQLITE`).

## Compilar y ejecutar

Desde la carpeta raíz del proyecto, configura y compila:

```bash
cmake -S . -B build
cmake --build build
```

Inicia la aplicación:

```bash
./build/GestionBiblioteca
```

Si tienes más de una instalación de Qt o CMake no encuentra Qt, indica la ruta de la instalación. Por ejemplo:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="$HOME/Qt/6.8.0/gcc_64"
cmake --build build
```

Sustituye esa ruta por la ubicación de Qt instalada en tu equipo. Si cambias la versión de Qt o el compilador y CMake conserva la configuración anterior, vuelve a configurar en una carpeta de compilación limpia.

## Usar Qt Creator

Abre `CMakeLists.txt` en Qt Creator, selecciona un kit que tenga Qt y un compilador compatibles, configura el proyecto y ejecuta el objetivo `GestionBiblioteca`.
