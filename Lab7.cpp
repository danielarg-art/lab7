#include "Lab7.h"
#include "ui_Lab7.h"
#include <QAction>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStringList>
#include <QTableWidget>

Lab7::Lab7(QWidget* parent) : QMainWindow(parent), ui(new Ui::Lab7) {
    ui->setupUi(this);
    ui->tablaContactos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tablaContactos->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tablaContactos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tablaContactos->setSortingEnabled(false);
    ui->tablaContactos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(ui->btnGuardar, &QPushButton::clicked, this, &Lab7::guardarContacto);
    connect(ui->actionRegistrar, &QAction::triggered, this, &Lab7::mostrarRegistro);
    connect(ui->actionHistorial, &QAction::triggered, this, [this] { mostrarTabla(Modo::Historial); });
    connect(ui->actionEditar, &QAction::triggered, this, [this] { mostrarTabla(Modo::Editar); });
    connect(ui->actionEliminar, &QAction::triggered, this, [this] { mostrarTabla(Modo::Eliminar); });
    connect(ui->btnEditar, &QPushButton::clicked, this, &Lab7::editarContacto);
    connect(ui->btnEliminar, &QPushButton::clicked, this, &Lab7::eliminarContacto);
    connect(ui->tablaContactos, &QTableWidget::itemSelectionChanged, this, [this] {
        if (modo != Modo::Editar) return;
        const int fila = ui->tablaContactos->currentRow();
        if (fila < 0) return;
        ui->editNombre->setText(ui->tablaContactos->item(fila, 0)->text());
        ui->editTelefono->setText(ui->tablaContactos->item(fila, 1)->text());
        ui->editCorreo->setText(ui->tablaContactos->item(fila, 2)->text());
    });
    mostrarRegistro();
}
Lab7::~Lab7() { delete ui; }

bool Lab7::datosValidos(const Contacto& contacto) {
    for (const QString& dato : QStringList{contacto.nombre, contacto.telefono, contacto.correo}) {
        if (dato.trimmed().isEmpty() || dato.contains(';') || dato.contains('\n') || dato.contains('\r')) return false;
    }
    return true;
}

void Lab7::guardarContacto() {
    Contacto c{ui->txtNombre->text().trimmed(), ui->txtTelefono->text().trimmed(), ui->txtCorreo->text().trimmed()};
    if (!datosValidos(c)) {
        QMessageBox::warning(this, "Datos inválidos", "Completá los tres campos sin punto y coma ni saltos de línea.");
        return;
    }
    if (!archivoContactos.guardar(c.nombre, c.telefono, c.correo)) {
        QMessageBox::critical(this, "No se pudo guardar", "Revisá los permisos y el espacio disponible.\nArchivo: " + archivoContactos.ruta());
        return;
    }
    ui->txtNombre->clear(); ui->txtTelefono->clear(); ui->txtCorreo->clear();
    QMessageBox::information(this, "Guardado completo", "El contacto se guardó correctamente.\nArchivo: " + archivoContactos.ruta());
    ui->txtNombre->setFocus();
}

void Lab7::mostrarRegistro() { ui->paginas->setCurrentWidget(ui->paginaRegistro); ui->txtNombre->setFocus(); }

void Lab7::mostrarTabla(Modo nuevoModo) {
    modo = nuevoModo;
    ui->paginas->setCurrentWidget(ui->paginaHistorial);
    ui->btnEditar->setVisible(modo == Modo::Editar);
    ui->btnEliminar->setVisible(modo == Modo::Eliminar);
    ui->panelEdicion->setVisible(modo == Modo::Editar);
    ui->tituloHistorial->setText(modo == Modo::Historial ? "Historial de contactos" : modo == Modo::Editar ? "Editar contacto" : "Eliminar contacto");
    ui->editNombre->clear(); ui->editTelefono->clear(); ui->editCorreo->clear();
    refrescarTabla();
}

void Lab7::refrescarTabla() {
    QVector<Contacto> contactos;
    if (!archivoContactos.leer(contactos)) {
        QMessageBox::critical(this, "Error de lectura", "No se pudo leer el archivo de contactos.");
        return;
    }
    ui->tablaContactos->setRowCount(0);
    for (const Contacto& c : contactos) {
        int fila = ui->tablaContactos->rowCount();
        ui->tablaContactos->insertRow(fila);
        ui->tablaContactos->setItem(fila, 0, new QTableWidgetItem(c.nombre));
        ui->tablaContactos->setItem(fila, 1, new QTableWidgetItem(c.telefono));
        ui->tablaContactos->setItem(fila, 2, new QTableWidgetItem(c.correo));
    }
    ui->tablaContactos->clearSelection();
}

void Lab7::editarContacto() {
    const int fila = ui->tablaContactos->currentRow();
    if (fila < 0 || ui->tablaContactos->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Sin selección", "Seleccioná un contacto para editar."); return;
    }
    Contacto c{ui->editNombre->text().trimmed(), ui->editTelefono->text().trimmed(), ui->editCorreo->text().trimmed()};
    if (!datosValidos(c)) {
        QMessageBox::warning(this, "Datos inválidos", "Revisá los tres datos del contacto."); return;
    }
    if (!archivoContactos.actualizar(fila, c)) {
        QMessageBox::critical(this, "Error", "No se pudo actualizar el contacto."); return;
    }
    refrescarTabla();
    ui->editNombre->clear(); ui->editTelefono->clear(); ui->editCorreo->clear();
    QMessageBox::information(this, "Actualizado", "El contacto se actualizó correctamente.");
}

void Lab7::eliminarContacto() {
    const int fila = ui->tablaContactos->currentRow();
    if (fila < 0 || ui->tablaContactos->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Sin selección", "Seleccioná un contacto para eliminar."); return;
    }
    if (QMessageBox::question(this, "Confirmar eliminación", "¿Deseás eliminar el contacto seleccionado?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) return;
    if (!archivoContactos.eliminar(fila)) {
        QMessageBox::critical(this, "Error", "No se pudo eliminar el contacto."); return;
    }
    refrescarTabla();
    QMessageBox::information(this, "Eliminado", "El contacto se eliminó correctamente.");
}
