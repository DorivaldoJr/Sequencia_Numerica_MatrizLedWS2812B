#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include "hardware/pio.h"
#include "pio_matrix.pio.h" 

#define NUM_PIXELS 25
#define OUT_PIN 7

uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R = r * 255;
    unsigned char G = g * 255;
    unsigned char B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

const uint led_red_pin = 13;
const uint button_A_pin = 5;
const uint button_B_pin = 6;

volatile int contador = 0; // contador para incremento e decremento dos botoes

// vetor com a representacao dos numeros 0-9(invertidos na horizontal para melhor visibilidade, algumas correcoes em pixels isolados)
double numeros[10][25] = {
    {0,1,1,1,0, 
     0,1,0,1,0,
     0,1,0,1,0,
     0,1,0,1,0,
     0,1,1,1,0}, // 0 

    {0,0,1,0,0,
     0,0,1,0,0,
     0,0,1,0,0,
     0,1,1,0,0,
     0,0,1,0,0}, // 1

    {0,1,1,1,0,
     0,1,0,0,0, 
     0,1,1,1,0, 
     0,0,0,1,0, 
     0,1,1,1,0}, // 2

    {0,1,1,1,0,
     0,0,0,1,0, 
     0,1,1,1,0, 
     0,0,0,1,0, 
     0,1,1,1,0}, // 3

    {0,1,0,0,0,
     0,0,0,1,0, 
     0,1,1,1,0, 
     0,1,0,1,0, 
     0,1,0,1,0}, // 4

    {0,1,1,1,0,
     0,0,0,1,0, 
     0,1,1,1,0, 
     0,1,0,0,0, 
     0,1,1,1,0}, // 5

    {0,1,1,1,0,
     0,1,0,1,0, 
     0,1,1,1,0, 
     0,1,0,0,0, 
     0,1,1,1,0}, // 6

    {0,0,0,0,1,
     0,1,0,0,0, 
     0,0,1,0,0, 
     0,0,0,1,0, 
     1,1,1,1,1}, // 7

    {0,1,1,1,0,
     0,1,0,1,0, 
     0,1,1,1,0, 
     0,1,0,1,0, 
     0,1,1,1,0}, // 8 

    {0,1,1,1,0,
     0,0,0,1,0, 
     0,1,1,1,0, 
     0,1,0,1,0, 
     0,1,1,1,0}  // 9
};

// Função para atualizar a matriz de LEDs
void atualiza_display(int numero, PIO pio, uint sm) {
    uint32_t valor_led;
    for (int i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(numeros[numero][i], 0.0, 0.0); // Assume cor vermelha 
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

// Função de interrupção para os botões(com tratamento de deboucing do wilton)
uint32_t last_time_A = 0;  // Variável de tempo para debouncing do botão A
uint32_t last_time_B = 0;  // Variável de tempo para debouncing do botão B
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time()); // Tempo atual em microssegundos

    if (gpio == button_A_pin) { // Se o botão A foi pressionado
        if (current_time - last_time_A > 200000) { // 200ms de debouncing
            last_time_A = current_time;
            contador++; // incrementa a variavel para mudanca de numero 
            if (contador > 9) contador = 0; // Reset ao chegar no 10
            atualiza_display(contador, pio0, 0);
        }
    } 
    else if (gpio == button_B_pin) { // Se o botão B foi pressionado
        if (current_time - last_time_B > 200000) { // 200ms de debouncing
            last_time_B = current_time;
            contador--;// decrementa a variavel para mudanca de numero 
            if (contador < 0) contador = 9; // Reset ao chegar no -1
            atualiza_display(contador, pio0, 0);
        }
    }
}

// Função principal
int main() {
    PIO pio = pio0;
    bool ok;
    uint sm = pio_claim_unused_sm(pio, true);
    uint offset = pio_add_program(pio, &pio_matrix_program);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);
   

    //incializa leds e botoes
    gpio_init(led_red_pin);
    gpio_set_dir(led_red_pin,GPIO_OUT);

    gpio_init(button_A_pin);
    gpio_set_dir(button_A_pin,GPIO_IN);
    gpio_pull_up(button_A_pin);

    gpio_init(button_B_pin);
    gpio_set_dir(button_B_pin,GPIO_IN);
    gpio_pull_up(button_B_pin);

    //interrupcao dos botoes
    gpio_set_irq_enabled_with_callback(button_A_pin,GPIO_IRQ_EDGE_FALL,true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_B_pin,GPIO_IRQ_EDGE_FALL,true, &gpio_irq_handler);
   
    stdio_init_all();  // Inicializa a comunicação serial (para debug)

    while (true) {
         //led acesso por 100ms e apagado por 100ms, completando 5 ciclos por segundo;
         gpio_put(led_red_pin,true);
         sleep_ms(100);
         gpio_put(led_red_pin,false);
         sleep_ms(100);
        
    }

}