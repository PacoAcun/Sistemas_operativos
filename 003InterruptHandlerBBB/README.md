# Lab003: Interrupts and Exception Handling

Este laboratorio tiene como objetivo introducirte a la programación dirigida por interrupciones (interrupt-driven) en arquitecturas ARM, específicamente trabajando con **timer interrupts**. En lugar de usar bucles de espera (busy loops), vas a configurar un temporizador para que genere una interrupción cada 2 segundos y así manejar tareas a intervalos regulares.

---

## Objetivos

1. **Configurar el periférico de temporizador (Timer)** para una ejecución periódica.
2. **Entender la Tabla de Vectores de Excepciones (IVT)** y saber cómo manejar interrupciones en ensamblador.
3. **Implementar un flujo de ejecución basado en interrupciones**, dejando de depender de bucles de espera manuales.
4. **Obtener una visión de cómo funcionan las excepciones en sistemas ARM**, tanto a nivel bajo (ensamblador) como a nivel de “OS” (código C).

---

## Estructura del Proyecto

El código se divide en tres capas principales:

1. **Nivel de Usuario (Application Logic)**
   - **Archivo:** `main.c`
   - **Objetivo:** Contiene la lógica principal.
   - **Tareas Clave:**
     - Configurar y habilitar la interrupción del timer.
     - Imprimir mensajes antes y después de habilitar las interrupciones.
     - Monitorear periódicamente el valor del timer para verificar que decrece.
     - Asegurar que el timer se reinicie automáticamente.

2. **Nivel de OS (Manejo de Interrupciones & Hardware)**
   - **Archivo:** `os.c`
   - **Objetivo:** Interfaz de bajo nivel hacia el hardware, configuración del timer y el Vector Interrupt Controller (VIC).
   - **Tareas Clave:**
     - Implementar una función para configurar el periférico del timer.
     - Configurar el VIC para habilitar IRQs (en particular la línea del Timer).
     - Implementar el handler (en C) que atiende la interrupción y limpia la bandera correspondiente.
     - Opcionalmente, manejar múltiples interrupciones (software y timer).

3. **Nivel de Hardware Bajo (Exception Handling)**
   - **Archivo:** `root.s`
   - **Objetivo:** Gestionar la Vector Table de ARM y encaminar las IRQs al handler adecuado.
   - **Tareas Clave:**
     - Asegurar que la vector table apunte al handler de IRQ.
     - Preservar los registros del CPU dentro de la ISR antes de modificarlos.
     - Asegurar que la ISR reconozca/limpie la interrupción y reanude la ejecución normal.

---

## Pasos Principales de la Asignación

1. **Entender el Código Existente**
   - Revisa cómo se imprimen mensajes vía UART.
   - Observa cómo `os.c` interactúa con el hardware.
   - Ubica dónde insertar la nueva funcionalidad de interrupción de timer.

2. **Implementar la Inicialización del Timer (`os.c`)**
   - Función que cargue un valor de cuenta regresiva (2 segundos) en el timer.
   - Configurar el modo periódico.
   - Habilitar la generación de interrupciones.
   - Asegurar que el timer se recargue de manera automática al llegar a cero.

3. **Configurar el VIC para Manejo de Interrupciones (`os.c`)**
   - Habilitar la IRQ #4 (o la correspondiente al timer) en el VIC.
   - Confirmar que el VIC enrute la interrupción al CPU.
   - Ofrecer un mecanismo para limpiar/acknowledge la interrupción.

4. **Implementar el IRQ Handler (`root.s`)**
   - Asegurar que la tabla de vectores dirija el flujo al handler correcto.
   - Leer el VIC para saber la fuente de la interrupción.
   - Llamar al handler del timer en `os.c`.
   - Reconocer la interrupción en el VIC antes de retornar.

5. **Modificar `main.c` para Inicializar y Probar la Interrupción**
   - Configurar y habilitar el timer.
   - Imprimir mensajes de depuración antes y después de habilitar IRQs.
   - Imprimir periódicamente el valor del timer.
   - Verificar que la interrupción aparezca cada 2 segundos.

---

## Validación y Depuración

**Funcionamiento Esperado:**
- Se imprime un mensaje de inicialización.
- El valor del timer se ve decrementar constantemente.
- Cada 2 segundos aparece un mensaje (ej. `"Timer Interrupt Triggered"`).
- El timer se reinicia automáticamente.
- El sistema no se congela ni se cuelga.

**Problemas Comunes:**

| Problema                          | Causa Probable                               | Solución                                                 |
|-----------------------------------|----------------------------------------------|-----------------------------------------------------------|
| **La interrupción nunca ocurre**  | VIC no configurado correctamente             | Revisar `VIC_INTENABLE`                                  |
| **El sistema se cuelga**          | CPU no reconoce las interrupciones           | Asegurar que `VIC_VADDR` se escriba en el ISR            |
| **El timer no se reinicia**       | Falta modo periódico                         | Revisar bits de `TIMER_CONTROL`                          |
| **Varias interrupciones/segundo** | No se limpia la bandera de interrupción      | Escribir `TIMER_INTCLR = 1` después de manejar el timer   |

---

## Entregables

1. **Archivos Fuente Actualizados**
   - `main.c`, `os.c`, `root.s`
2. **Documentación**
   - Descripción del funcionamiento del timer y su interrupción.
   - Explicación de cada función modificada.
   - Descripción de cómo se gestionan las IRQs en el sistema.
3. **Logs de Ejecución**
   - Muestra la cuenta del timer y los disparos de interrupción.

---

## Resultados Esperados

- Comprender cómo los timers de hardware funcionan en ARM.
- Implementar un sistema basado en interrupciones en vez de bucles de espera.
- Enrutar correctamente IRQs desde el VIC hasta el CPU.
- Manejar excepciones de bajo nivel en ARM de manera funcional.

---

## Consejos para el Éxito

- **Pruebas Incrementales:**  
  1. Asegúrate primero de que el timer cuente regresivamente.  
  2. Después, valida que la IRQ se dispare.  
  3. Finalmente, verifica que se reconozca la interrupción y se borre la bandera correctamente.
  
- **Usa Mensajes de Depuración (prints)**:
  - Antes y después de habilitar IRQs.
  - Antes y después de limpiar la interrupción en el VIC o en el Timer.

- **Verifica los Valores de Registro**:
  - Si no salta la interrupción, imprime en consola el contenido del `TIMER_CONTROL`, `VIC_INTENABLE`, etc.

---

## Ejecución en QEMU

Ejemplo de comando para la **VersatilePB**:

```bash
qemu-system-arm -M versatilepb -m 128M -nographic -kernel lab003.elf
