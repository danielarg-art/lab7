#include "ArchivoContactos.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSaveFile>
#include <QStringList>

QString ArchivoContactos::ruta() const {
    return QDir::homePath() + "/Lab7Datos/contactos.txt";
}

bool ArchivoContactos::guardar(const QString& nombre, const QString& telefono, const QString& correo) const {
    if (!QDir().mkpath(QFileInfo(ruta()).absolutePath())) return false;
    QFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Append)) return false;
    QByteArray datos = (nombre + ";" + telefono + ";" + correo + "\n").toUtf8();
    bool completo = archivo.write(datos) == datos.size();
    bool vaciado = archivo.flush();
    archivo.close();
    return completo && vaciado;
}

bool ArchivoContactos::leer(QVector<Contacto>& contactos) const {
    contactos.clear();
    QFile archivo(ruta());
    if (!archivo.exists()) return true;
    if (!archivo.open(QIODevice::ReadOnly)) return false;
    while (!archivo.atEnd()) {
        QString linea = QString::fromUtf8(archivo.readLine()).trimmed();
        if (linea.isEmpty()) continue;
        QStringList partes = linea.split(';');
        if (partes.size() != 3) return false;
        contactos.append({partes[0], partes[1], partes[2]});
    }
    return archivo.error() == QFile::NoError;
}

bool ArchivoContactos::reescribir(const QVector<Contacto>& contactos) const {
    if (!QDir().mkpath(QFileInfo(ruta()).absolutePath())) return false;
    QSaveFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly)) return false;
    for (const Contacto& contacto : contactos) {
        QByteArray linea = (contacto.nombre + ";" + contacto.telefono + ";" + contacto.correo + "\n").toUtf8();
        if (archivo.write(linea) != linea.size()) {
            archivo.cancelWriting();
            return false;
        }
    }
    return archivo.commit();
}

bool ArchivoContactos::actualizar(int posicion, const Contacto& contacto) const {
    QVector<Contacto> contactos;
    if (!leer(contactos) || posicion < 0 || posicion >= contactos.size()) return false;
    contactos[posicion] = contacto;
    return reescribir(contactos);
}

bool ArchivoContactos::eliminar(int posicion) const {
    QVector<Contacto> contactos;
    if (!leer(contactos) || posicion < 0 || posicion >= contactos.size()) return false;
    contactos.removeAt(posicion);
    return reescribir(contactos);
}
