#include "os.h"
#include "uart.h"

// Función principal
int main(void) {
    // Inicializar UART
    uart_init();
    uart_puts("-----Sistema iniciado-----\n");

    // Configurar el callback para la interrupción del temporizador
    timer_callback = timer_irq_handler;

    // Inicializar el temporizador para que genere una interrupción cada 2 segundos
    uart_puts("Configurando temporizador... \n");
    timer_init(2);

    // Habilitar interrupciones
    uart_puts("Habilitando interrupciones... \n");
    irq_enable();

    // Bucle principal
    uart_puts("Entrando en bucle principal... \n");
    while (1) {
        // Esperar en un bucle infinito
        delay(1000000);  // Pequeño retardo para evitar uso excesivo de CPU
    }

    return 0;  // Nunca se alcanzará este punto
}
