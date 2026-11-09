#ifndef LIBRO_H
#define LIBRO_H

#include <QString>

class Libro
{
private:
    int id;
    int idCategoria;
    QString titulo;
    QString autor;
    int year;
    QString editorial;
    QString isbn;
    bool activo;

public:
    Libro();
    Libro(int id, int idCategoria, const QString &titulo, const QString &autor,
          int year, const QString &editorial, const QString &isbn, bool activo = true);

    int getId() const;
    void setId(int id);

    int getIdCategoria() const;
    void setIdCategoria(int idCategoria);

    QString getTitulo() const;
    void setTitulo(const QString &titulo);

    QString getAutor() const;
    void setAutor(const QString &autor);

    int getYear() const;
    void setYear(int year);

    QString getEditorial() const;
    void setEditorial(const QString &editorial);

    QString getIsbn() const;
    void setIsbn(const QString &isbn);

    bool isActivo() const;
    void setActivo(bool activo);
};

#endif // LIBRO_H#endif // LIBRO_H