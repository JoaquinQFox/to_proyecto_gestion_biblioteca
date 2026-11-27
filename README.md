# Sistema de Gestión de Biblioteca

Aplicación de escritorio para administrar categorías, libros, ejemplares, lectores y préstamos, con un módulo de recomendaciones. Está desarrollada en C++17 con Qt Widgets y guarda la información en SQLite.

Proyecto desarrollado para el curso **TECNOLOGÍA DE OBJETOS**.

## Integrantes

- Choque Sánchez Alejandra Camila
- Davila Flores Mathias Dario
- Quispe Bedregal Joaquin Alejandro
- Rojas Condori Fabiana Paola
- Velazque Quispe Flor de Liz

## Funcionalidades

- Administración de categorías, libros y ejemplares.
- Administración de lectores y control de su estado.
- Registro y devolución de préstamos.
- Consulta de préstamos activos y vencidos.
- Panel con métricas generales y préstamos recientes.
- Recomendaciones bibliográficas por popularidad, categoría e historial.
- Persistencia de datos en SQLite.
- Cálculo de recomendaciones en un hilo secundario para evitar bloquear la interfaz.

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

En Windows, inicia el ejecutable desde una terminal:

```bash
.\build\GestionBiblioteca.exe
```

En Linux o macOS se puede iniciar con:

```bash
./build/GestionBiblioteca
```

Si tienes más de una instalación de Qt o CMake no encuentra Qt, indica la ruta de la instalación. Por ejemplo:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="$HOME/Qt/6.8.0/gcc_64"
cmake --build build
```

Sustituye esa ruta por la ubicación de Qt instalada en tu equipo. Si cambias la versión de Qt o el compilador y CMake conserva la configuración anterior, vuelve a configurar en una carpeta de compilación limpia.

### CMake desde Windows o WSL

Los archivos de compilación son específicos del entorno y no pueden compartir la misma carpeta `build` entre Windows y WSL. Si CMake informa que `CMakeCache.txt` fue creado en una ruta diferente, elimina la carpeta de compilación y vuelve a generarla desde el entorno actual.

Desde PowerShell o CMD de Windows:

```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Desde WSL o Linux:

```bash
rm -rf build
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Usar Qt Creator

Abre `CMakeLists.txt` en Qt Creator, selecciona un kit que tenga Qt y un compilador compatibles, configura el proyecto y ejecuta el objetivo `GestionBiblioteca`.

## Pruebas

El proyecto incluye pruebas Qt Test para la fábrica de estrategias y los repositorios. Después de compilar, desde la carpeta raíz ejecuta:

```bash
ctest --test-dir build --output-on-failure
```

Las pruebas crean una base de datos SQLite en memoria y verifican operaciones de creación, consulta, actualización y eliminación.

## Patrones de diseño aplicados

### Modelo-Vista-Controlador (MVC) y Repository

Las entidades del dominio están en `src/models`, las interfaces en `src/views`, las reglas de operación y validación en `src/controllers`, y las consultas de persistencia en `src/repositories`. Los repositorios implementan la interfaz genérica `IRepository<T>`.

Esta separación mejora el mantenimiento y la reutilización. Permite modificar las vistas sin mezclar reglas de negocio en la interfaz y mantiene centralizado el acceso a datos en una capa independiente.

### Strategy

Las estrategias `PopularityStrategy`, `CategoryStrategy` y `HistoryStrategy` implementan `IRecommendationStrategy`.

Cada estrategia encapsula un algoritmo de recomendación distinto. Agregar un nuevo criterio no obliga a duplicar toda la lógica de la interfaz o del controlador: solo se crea una nueva estrategia y se registra en la fábrica.

### Simple Factory

`RecommendationFactory` recibe un valor de `TipoEstrategia` e instancia la estrategia correspondiente mediante `std::unique_ptr`.

Centraliza la creación de objetos y evita que el controlador seleccione directamente clases concretas. En este proyecto corresponde llamarlo *Simple Factory*, no *Factory Method* clásico de GoF.

### Observer mediante señales y slots de Qt

Los controladores `PrestamoController` y `RecomendacionController` emiten señales ante cambios o resultados. Las vistas se suscriben a esas señales mediante `connect`.

De esta manera, el controlador no necesita conocer directamente qué pantallas existen. Permite que las vistas reaccionen a eventos y actualicen su información sin fuertes dependencias entre clases.

### Administración centralizada de conexiones

La clase `Database` centraliza la configuración de la conexión SQLite principal y proporciona conexiones propias para hilos secundarios.

Esta solución facilita usar una única ruta de base de datos y administrar correctamente las conexiones usadas por operaciones concurrentes. `Database` se describe como administrador centralizado de conexiones, no como Singleton clásico.

## Organización del proyecto

```text
src/
├── models/        Entidades del dominio
├── views/         Interfaces gráficas Qt
├── controllers/   Operaciones y validaciones
├── repositories/  Acceso y persistencia de datos
├── interfaces/    Contratos reutilizables
├── strategies/    Algoritmos de recomendación
├── factories/     Creación de estrategias
├── database/      Conexión y inicialización SQLite
└── tests/         Pruebas con Qt Test
```
