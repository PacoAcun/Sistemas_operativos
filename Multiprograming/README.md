# Lab002 Calculator

## Building a Bare-Metal ARM Application with UART Communication

### Objetivo

El objetivo principal de este laboratorio es familiarizar a los estudiantes con los fundamentos de la programación bare-metal en arquitecturas ARM. Al analizar y comprender el código proporcionado, los estudiantes aprenderán a estructurar aplicaciones en diferentes capas de abstracción, implementar funcionalidades básicas de entrada/salida e interactuar directamente con componentes de hardware ARM utilizando comunicación UART.

### Resumen

En este laboratorio, trabajarás con una aplicación simple de bare-metal ARM diseñada para sumar dos enteros recibidos vía UART y mostrar el resultado. El código proporcionado está organizado en múltiples archivos, cada uno con un propósito específico dentro de la arquitectura de la aplicación. Tu tarea es analizar, comprender y reestructurar el código en tres capas distintas:

1. **Nivel de Usuario**
2. **Nivel de Biblioteca de Lenguaje**
3. **Nivel de Sistema Operativo (Interfaz con el Hardware ARM)**

Además, mejorarás las capacidades de entrada/salida de la aplicación para soportar cadenas formateadas similares a `printf` y `scanf`, manejando tipos de datos como cadenas, enteros y flotantes.

### Estructura del Proyecto

El proyecto consta de los siguientes archivos:

1. `main.c`: Contiene la lógica principal de la aplicación para sumar dos números.
2. `root.s`: Archivo de inicio en ensamblador responsable de inicializar la pila e invocar la función `main`.
3. `string.c` y `string.h`: Implementación mínima de funciones de manipulación de cadenas.
4. `build_and_run.sh`: Script de shell para automatizar el proceso de construcción y ejecución utilizando QEMU dentro de un contenedor Docker.
5. `linker.ld`: Script de enlace que define la disposición de la memoria y los puntos de entrada.
6. `stdio.h` y `stdio.c` (a ser creados): Para implementar funcionalidades de impresión y lectura similares a `printf` y `scanf`.

### Desglose por Capas

Comprender la separación de responsabilidades es crucial para desarrollar software escalable y mantenible. El código proporcionado se puede dividir lógicamente en tres capas:

1. **Nivel de Usuario**

   **Descripción:**
   Esta es la capa de mayor abstracción donde reside la lógica de la aplicación. Interactúa con la biblioteca de lenguaje para realizar operaciones de alto nivel sin profundizar en detalles específicos del hardware.

   **Componentes:**
   - `main.c`: Implementa la funcionalidad central de la aplicación. Maneja interacciones con el usuario, procesa entradas, realiza cálculos y muestra salidas.

   **Responsabilidades:**
   - Solicitar al usuario la entrada de datos.
   - Analizar y validar los datos de entrada.
   - Realizar operaciones aritméticas.
   - Mostrar resultados al usuario.

2. **Nivel de Biblioteca de Lenguaje**

   **Descripción:**
   Esta capa intermedia proporciona funciones de utilidad que facilitan operaciones de alto nivel. Abstrae tareas comunes como la manipulación de cadenas y la conversión de datos, haciéndolas reutilizables en diferentes partes de la aplicación.

   **Componentes:**
   - `string.c` y `string.h`: Proporcionan una implementación mínima de la función `strncpy` (`my_strncpy`), permitiendo una copia segura de cadenas sin depender de la biblioteca estándar de C.
   - `stdio.c` y `stdio.h` (a ser creados): Implementarán funciones de impresión y lectura análogas a `printf` y `scanf`, manejando entrada y salida formateada.

   **Responsabilidades:**
   - Implementar funciones de manipulación de cadenas.
   - Manejar entrada y salida formateada.
   - Convertir tipos de datos (por ejemplo, de cadena a entero y viceversa).

3. **Nivel de Sistema Operativo (Interfaz con el Hardware ARM)**

   **Descripción:**
   Esta capa más baja interactúa directamente con el hardware ARM, gestionando protocolos de comunicación y operaciones específicas del hardware. Abstrae las complejidades de las interacciones con el hardware, proporcionando funciones simples que las capas superiores pueden utilizar.

   **Componentes:**
   - `os.c` y `os.h`: Gestionan la comunicación UART implementando funciones como `uart_putc`, `uart_getc`, `uart_puts` y `uart_gets_input`. También manejan conversiones de datos con funciones como `uart_atoi` y `uart_itoa`. La API expuesta será mediante los métodos `WRITE` y `READ`.
   - `root.s`: Código en ensamblador responsable de inicializar el puntero de pila e invocar la función `main`.

   **Responsabilidades:**
   - Inicializar componentes de hardware (por ejemplo, UART).
   - Enviar y recibir datos vía UART.
   - Convertir datos entre diferentes formatos.
   - Configurar el entorno de ejecución (inicialización de la pila).

### Tareas de la Asignación

1. **Analizar el Código Proporcionado:**
   - Comprender el propósito y la funcionalidad de cada archivo.
   - Identificar cómo interactúan las capas entre sí.

2. **Implementar la Capa de Biblioteca de Lenguaje (`stdio.c` y `stdio.h`):**
   - Desarrollar funciones de impresión y lectura que imiten el comportamiento de `printf` y `scanf`.
   - Asegurarse de que estas funciones puedan manejar cadenas formateadas con especificadores para cadenas (`%s`), enteros (`%d`) y flotantes (`%f`).

3. **Mejorar el Nivel de Usuario (`main.c`):**
   - Modificar `main.c` para utilizar las nuevas funciones de impresión y lectura implementadas para interacciones con el usuario.
   - Asegurarse de que la aplicación pueda:
     - Solicitar al usuario que ingrese dos números.
     - Leer los números de entrada.
     - Calcular su suma.
     - Mostrar el resultado de manera formateada.

4. **Validar la Comunicación UART:**
   - Asegurarse de que los mensajes enviados mediante la función de impresión aparezcan correctamente en la terminal.
   - Probar la recepción de entrada mediante la función de lectura y verificar el procesamiento preciso de los datos.

5. **Documentar la Arquitectura en Capas:**
   - Proporcionar explicaciones claras de cómo interactúa cada capa.
   - Resaltar el flujo de datos desde la entrada del usuario hasta la comunicación con el hardware y viceversa.

6. **Construir y Ejecutar la Aplicación:**
   - Utilizar el script `build_and_run.sh` proporcionado para compilar y ejecutar la aplicación en QEMU.
   - Observar la comunicación UART para asegurar la funcionalidad correcta.

### Entregables

1. **Nuevos y Actualizados Archivos Fuente:**
   - Implementación de funciones de impresión y lectura.
   - `main.c`: Utilizando la biblioteca de lenguaje para interacciones con el usuario.

2. **Documentación:**
   - Un breve informe explicando la separación de capas.
   - Descripciones de cómo funciona cada función dentro de su respectiva capa.

3. **Demostración:**
   - Capturas de pantalla o registros de terminal que muestran operaciones de entrada y salida exitosas.
   - Evidencia de operaciones aritméticas correctas y salida formateada.