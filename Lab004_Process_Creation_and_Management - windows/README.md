# Laboratorio 004: Creación y Gestión de Procesos

Este laboratorio demuestra varios conceptos fundamentales de sistemas operativos relacionados con la creación de procesos, sincronización y mecanismos de comunicación entre procesos (IPC).

## Requisitos Previos

Para ejecutar este laboratorio necesitas:

- Sistema operativo Windows
- Compilador GCC instalado y configurado en el PATH
- Terminal que soporte comandos Bash (como Git Bash, MinGW, etc.)

## Ejecutando el Proyecto

Para compilar y ejecutar todos los programas:

1. Abre una terminal en la carpeta del proyecto
2. Ejecuta el archivo batch con el comando:
   ```
   ./run.bat
   ```

El script `run.bat` realiza las siguientes acciones:
- Limpia archivos compilados previos
- Compila cada uno de los programas fuente
- Ejecuta cada programa en secuencia
- Muestra los resultados en la terminal

## Descripción Detallada de los Problemas y Código

### Tarea 1: Creación de Procesos

#### Problema
Esta tarea demuestra el concepto fundamental de creación de procesos en sistemas operativos. Un proceso padre crea un proceso hijo y ambos ejecutan tareas separadas.

#### parent.c

**Explicación**:
- La función `fork()` crea un nuevo proceso duplicando el proceso actual
- Después de `fork()`, ambos procesos (padre e hijo) continúan ejecutándose
- Para el proceso padre, `fork()` devuelve el PID del hijo
- Para el proceso hijo, `fork()` devuelve 0
- El hijo utiliza `execl()` para reemplazar su imagen en memoria con el programa `child.exe`
- El padre espera a que el hijo termine usando `wait()`

#### child.c

**Explicación**:
- El programa hijo imprime su PID y el PID de su padre
- Simula realizar algún trabajo con `sleep(2)`
- Retorna un valor específico (42) para demostrar cómo el padre puede recoger el estado de salida

### Tarea 2: Sincronización de Procesos

#### Problema
Esta tarea demuestra cómo sincronizar procesos para coordinar su ejecución, asegurando que ciertas operaciones se completen en el orden correcto.

#### parent_sync.c

**Explicación**:
- Este código implementa un mecanismo simple de sincronización usando un archivo como señal
- El padre crea un archivo "sync.txt" antes de crear el proceso hijo
- El hijo verifica constantemente la existencia del archivo y espera hasta que desaparezca
- Cuando el padre completa su inicialización, elimina el archivo para "señalizar" al hijo
- Este es un ejemplo sencillo de sincronización; en sistemas reales se usarían semáforos, mutexes o variables de condición

#### child_sync.c

**Explicación**:
- Este programa simplemente ejecuta su tarea después de que la sincronización ha ocurrido
- Demuestra que el hijo espera correctamente la señal del padre antes de ejecutar su lógica principal

### Tarea 3: Comunicación por Pipes

#### Problema
Esta tarea demuestra la comunicación entre procesos mediante pipes, permitiendo que el proceso padre envíe datos al proceso hijo y viceversa.

#### parent_pipe.c

**Explicación**:
- Se crean dos pipes: uno para comunicación padre→hijo y otro para hijo→padre
- Cada pipe tiene dos extremos: uno para lectura (READ_END) y otro para escritura (WRITE_END)
- Después de `fork()`, cada proceso cierra los extremos de los pipes que no utilizará
- El padre envía un mensaje al hijo a través del pipe padre→hijo
- El hijo lee el mensaje, responde a través del pipe hijo→padre, y el padre lee la respuesta
- Los descriptores de archivo de los pipes se pasan como argumentos al programa hijo

#### child_pipe.c

**Explicación**:
- El programa hijo recibe los descriptores de archivo de los pipes como argumentos
- Lee el mensaje del padre a través del pipe padre→hijo
- Envía una respuesta al padre a través del pipe hijo→padre

### Tarea 4: Múltiples Procesos Hijos

#### Problema
Esta tarea demuestra cómo un proceso padre puede crear y gestionar múltiples procesos hijos, asignándoles diferentes identificadores para realizar tareas paralelas.

#### parent_multi.c

**Explicación**:
- El padre crea cuatro procesos hijos en un bucle utilizando `fork()`
- A cada hijo se le asigna un identificador (0-3) que se pasa como argumento
- El padre espera a que todos los hijos terminen usando `waitpid()` para cada uno
- `waitpid()` permite esperar a un hijo específico, a diferencia de `wait()` que espera a cualquiera

#### child_multi.c

**Explicación**:
- Cada hijo recibe su identificador como argumento de línea de comandos
- Para demostrar ejecución paralela, cada hijo duerme durante un tiempo proporcional a su ID
- Esto muestra cómo diferentes hijos pueden realizar trabajos independientes simultáneamente
- Cada hijo retorna su ID como estado de salida para que el padre pueda verificarlo

### Tarea 5: Memoria Compartida

#### Problema
Esta tarea demuestra cómo procesos independientes pueden comunicarse mediante segmentos de memoria compartida, permitiendo un acceso eficiente a datos comunes.

#### parent_shmem.c

**Explicación**:
- Se crea un segmento de memoria compartida con `shm_open()` y se establece su tamaño con `ftruncate()`
- La memoria se mapea al espacio de direcciones del proceso con `mmap()`
- Se define una estructura para organizar los datos compartidos y las banderas de sincronización
- El padre inicializa los datos, activa la bandera `ready_flag` para señalizar al hijo que puede leer
- El padre espera a que el hijo active la bandera `processed_flag` para saber que ha terminado
- Al final, el padre limpia los recursos con `munmap()` y `shm_unlink()`

#### child_shmem.c

**Explicación**:
- El hijo abre el segmento de memoria compartida existente con el nombre recibido como argumento
- Espera a que el padre active la bandera `ready_flag` para leer los datos
- Simula algún procesamiento y modifica el valor en la memoria compartida
- Activa la bandera `processed_flag` para indicar al padre que ha terminado
- Finalmente, libera los recursos con `munmap()` y `close()`