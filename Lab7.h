#pragma once
#include <QMainWindow>
#include "ArchivoContactos.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Lab7; }
QT_END_NAMESPACE

class Lab7 : public QMainWindow {
    Q_OBJECT
public:
    explicit Lab7(QWidget* parent = nullptr);
    ~Lab7() override;
private:
    enum class Modo { Historial, Editar, Eliminar };
    Ui::Lab7* ui;
    ArchivoContactos archivoContactos;
    Modo modo = Modo::Historial;
    void guardarContacto();
    void mostrarRegistro();
    void mostrarTabla(Modo nuevoModo);
    void refrescarTabla();
    void editarContacto();
    void eliminarContacto();
    static bool datosValidos(const Contacto& contacto);
};
