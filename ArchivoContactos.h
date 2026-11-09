#pragma once
#include <QString>
#include <QVector>

struct Contacto {
    QString nombre;
    QString telefono;
    QString correo;
};

class ArchivoContactos {
public:
    QString ruta() const;
    bool guardar(const QString& nombre, const QString& telefono, const QString& correo) const;
    bool leer(QVector<Contacto>& contactos) const;
    bool actualizar(int posicion, const Contacto& contacto) const;
    bool eliminar(int posicion) const;
private:
    bool reescribir(const QVector<Contacto>& contactos) const;
};
