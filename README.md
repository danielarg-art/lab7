# Lab 7 — Contactos (Programación I)

Aplicación de escritorio en **C++ / Qt 6 Widgets**. Registra, muestra, edita y elimina contactos almacenados en un archivo de texto.

## Abrir en Visual Studio 2022

Visual Studio debe tener habilitada la carga de trabajo **Desarrollo para el escritorio con C++** y Qt 6 con CMake disponible. Abrí **Archivo > Abrir > Carpeta** y seleccioná esta carpeta que contiene `CMakeLists.txt`. Elegí la configuración x64 e indicá la ubicación de Qt 6 en `CMAKE_PREFIX_PATH` si Visual Studio no lo detecta (por ejemplo, `C:/Qt/6.12.0/msvc2022_64`). Compilá y ejecutá el destino `Lab7`.

Alternativamente: `cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.12.0/msvc2022_64"` y `cmake --build build --config Debug`.

## Funcionamiento

El menú **Navegación** tiene Registrar, Historial, Editar y Eliminar. La selección en tabla es de una sola fila, sin ordenamiento; el número de fila coincide con el registro del archivo. El botón Guardar incorpora el contacto sin borrar los anteriores, muestra confirmación y limpia los campos **solo cuando pudo guardarse**. Editar actualiza únicamente el seleccionado. Eliminar pregunta antes de modificar el archivo. Ambas operaciones usan QSaveFile para escribir de forma segura.

**Ruta del archivo:** `%USERPROFILE%/Lab7Datos/contactos.txt` en Windows (`QDir::homePath() + "/Lab7Datos/contactos.txt"`). Se crea automáticamente al guardar el primer contacto. El archivo `contactos_ejemplo.txt` es solo una muestra para la entrega, no se carga automáticamente ni contiene datos personales reales.

## Comprobaciones recomendadas en Visual Studio

1. Registrar tres contactos y verificar el mensaje de confirmación y campos vacíos.
2. Abrir Historial repetidas veces: la tabla siempre debe contener las mismas tres filas, sin duplicados.
3. Ir a Editar, seleccionar la segunda fila, cambiar el teléfono y confirmar: debe conservar los demás datos.
4. Ir a Eliminar, seleccionar la tercera fila, cancelar: no cambia nada. Repetir y confirmar: solo se elimina la tercera.
5. Cerrar y abrir la aplicación: deben quedar dos contactos con el teléfono modificado.
6. Probar historial vacío, editar sin selección, campos vacíos y símbolos `;`.

**Estado de pruebas:** código y formulario preparados. Requiere compilación y capturas reales en Windows con Qt instalado. No se incluyen capturas fabricadas.
