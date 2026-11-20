#include "../database/Database.h"
#include "../database/DatabaseInitializer.h"
#include "../repositories/CategoriaRepository.h"
#include "../repositories/EjemplarRepository.h"
#include "../repositories/LectorRepository.h"
#include "../repositories/LibroRepository.h"
#include "../repositories/PrestamoRepository.h"

#include <QSqlQuery>
#include <QtTest>

class tst_Repositories : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanup();
    void categoriaCrud();
    void libroCrudYActivos();
    void ejemplarCrud();
    void lectorCrudYActivos();
    void prestamoCrudYEstadoEjemplar();
};

void tst_Repositories::initTestCase()
{
    QVERIFY2(Database::conectarEnMemoria(), "No se pudo abrir SQLite en memoria");
    QVERIFY2(DatabaseInitializer::incializar(), "No se pudo crear el esquema de prueba");
}

void tst_Repositories::cleanup()
{
    QSqlQuery query(Database::obtenerConexion());
    QVERIFY(query.exec("DELETE FROM prestamos"));
    QVERIFY(query.exec("DELETE FROM ejemplares"));
    QVERIFY(query.exec("DELETE FROM libros"));
    QVERIFY(query.exec("DELETE FROM lectores"));
    QVERIFY(query.exec("DELETE FROM categorias"));
}

void tst_Repositories::categoriaCrud()
{
    CategoriaRepository repo;
    const int id = repo.agregar(Categoria(0, "Ciencia"));
    QVERIFY(id > 0);
    QCOMPARE(repo.obtenerPorId(id).getNombre(), QString("Ciencia"));
    QCOMPARE(repo.obtenerTodos().size(), 1);
    QVERIFY(repo.actualizar(Categoria(id, "Historia")));
    QCOMPARE(repo.obtenerPorId(id).getNombre(), QString("Historia"));
    QVERIFY(repo.eliminar(id));
    QVERIFY(repo.obtenerTodos().isEmpty());
    QVERIFY(!repo.eliminar(id));
}

void tst_Repositories::libroCrudYActivos()
{
    CategoriaRepository categorias;
    const int idCategoria = categorias.agregar(Categoria(0, "Ciencia"));
    LibroRepository repo;
    const int id = repo.agregar(Libro(0, idCategoria, "Fisica", "Ana", 2020,
                                      "Editorial", "ISBN-1"));
    QVERIFY(id > 0);
    QCOMPARE(repo.obtenerPorId(id).getTitulo(), QString("Fisica"));
    QCOMPARE(repo.obtenerTodos().size(), 1);
    QVERIFY(repo.actualizar(Libro(id, idCategoria, "Fisica II", "Ana", 2021,
                                  "Editorial", "ISBN-1")));
    QCOMPARE(repo.obtenerPorId(id).getTitulo(), QString("Fisica II"));
    QVERIFY(repo.eliminar(id));
    QVERIFY(repo.obtenerActivos().isEmpty());
    QCOMPARE(repo.obtenerTodos().size(), 1); // Borrado lógico.
}

void tst_Repositories::ejemplarCrud()
{
    const int categoriaId = CategoriaRepository().agregar(Categoria(0, "Ciencia"));
    const int libroId = LibroRepository().agregar(
        Libro(0, categoriaId, "Fisica", "Ana", 2020, "Editorial", "ISBN-2"));
    EjemplarRepository repo;
    const int id = repo.agregar(Ejemplar(0, libroId, "E-001", Ejemplar::Estado::DISPONIBLE));
    QVERIFY(id > 0);
    QCOMPARE(repo.obtenerPorId(id).getCodigo(), QString("E-001"));
    QCOMPARE(repo.obtenerTodos().size(), 1);
    QVERIFY(repo.actualizar(Ejemplar(id, libroId, "E-001", Ejemplar::Estado::RETIRADO)));
    QCOMPARE(repo.obtenerPorId(id).getEstado(), Ejemplar::Estado::RETIRADO);
    QVERIFY(repo.eliminar(id));
    QVERIFY(repo.obtenerTodos().isEmpty());
}

void tst_Repositories::lectorCrudYActivos()
{
    LectorRepository repo;
    const int id = repo.agregar(Lector(0, "12345678", "Lucia", "Perez",
                                       "999111222", "Lima", true, "lucia@example.com"));
    QVERIFY(id > 0);
    QCOMPARE(repo.obtenerPorId(id).getCorreo(), QString("lucia@example.com"));
    QCOMPARE(repo.obtenerTodos().size(), 1);
    QVERIFY(repo.actualizar(Lector(id, "12345678", "Lucia", "Diaz",
                                   "999111222", "Lima", true, "lucia@example.com")));
    QCOMPARE(repo.obtenerPorId(id).getApellidos(), QString("Diaz"));
    QCOMPARE(repo.obtenerActivos().size(), 1);
    QVERIFY(repo.eliminar(id));
    QVERIFY(repo.obtenerActivos().isEmpty());
    QCOMPARE(repo.obtenerTodos().size(), 1); // Borrado lógico.
}

void tst_Repositories::prestamoCrudYEstadoEjemplar()
{
    const int categoriaId = CategoriaRepository().agregar(Categoria(0, "Ciencia"));
    const int libroId = LibroRepository().agregar(
        Libro(0, categoriaId, "Fisica", "Ana", 2020, "Editorial", "ISBN-3"));
    const int ejemplarId = EjemplarRepository().agregar(
        Ejemplar(0, libroId, "E-002", Ejemplar::Estado::DISPONIBLE));
    const int lectorId = LectorRepository().agregar(
        Lector(0, "87654321", "Luis", "Rojas", "999333444", "Lima"));

    PrestamoRepository repo;
    const QDate prestamo(2026, 11, 1);
    const QDate vencimiento(2026, 11, 15);
    const int id = repo.agregar(Prestamo(0, ejemplarId, lectorId, prestamo, vencimiento));
    QVERIFY(id > 0);
    QCOMPARE(repo.obtenerPorId(id).getFechaPrestamo(), prestamo);
    QVERIFY(!repo.obtenerPorId(id).getFechaDevolucion().isValid());
    QCOMPARE(repo.obtenerTodos().size(), 1);
    QCOMPARE(EjemplarRepository().obtenerPorId(ejemplarId).getEstado(), Ejemplar::Estado::PRESTADO);

    const QDate devolucion(2026, 11, 10);
    QVERIFY(repo.actualizar(Prestamo(id, ejemplarId, lectorId, prestamo, vencimiento, devolucion)));
    QCOMPARE(repo.obtenerPorId(id).getFechaDevolucion(), devolucion);
    QVERIFY(repo.eliminar(id));
    QVERIFY(repo.obtenerTodos().isEmpty());
}

QTEST_GUILESS_MAIN(tst_Repositories)
#include "tst_Repositories.moc"
