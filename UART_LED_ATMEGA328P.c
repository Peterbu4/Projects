#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define F_CPU 16000000UL
#define BAUD 19200
#define MYUBRR F_CPU/16/BAUD-1

volatile uint32_t seconds = 0;

void UART_Init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void UART_Print(char *str) {
    while (*str) {
        UART_Transmit(*str++);
    }
}

void Timer1_Init() {
    TCCR1B |= (1 << WGM12); // CTC mode
    OCR1A = 15624; // 1 second at 16MHz with prescaler 1024
    TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
    TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt
}

ISR(TIMER1_COMPA_vect) {
    seconds++;
}

int main(void) {
    // Initialization of LED pin (PB0) as output
    DDRB |= (1 << PB0);
    
    // Initialization of button pin (PD2) as input with pull-up
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);
    
    // Initialization of UART
    UART_Init(MYUBRR);
    
    // Initialization of Timer1 for seconds counting
    Timer1_Init();
    
    // Enabling global interrupts
    sei();
    
    UART_Print("Program started. Send commands:\r\n");
    UART_Print("'a' - LED1 ON\r\n");
    UART_Print("'s' - LED1 OFF\r\n");
    UART_Print("'d' - Check button state\r\n");
    UART_Print("'f' - Show seconds count\r\n");
    
    while (1) {
        // Waiting for receiving data
        while (!(UCSR0A & (1 << RXC0)));
        
        // Reading received data
        char received = UDR0;
        
        switch(received) {
            case 'a':
                PORTB |= (1 << PB0); // LED ON
                UART_Print("LED1 ON\r\n");
                break;
                
            case 's':
                PORTB &= ~(1 << PB0); // LED OFF
                UART_Print("LED1 OFF\r\n");
                break;
                
            case 'd':
                if (PIND & (1 << PD2)) {
                    UART_Print("Button is NOT pressed\r\n");
                } else {
                    UART_Print("Button is pressed\r\n");
                }
                break;
                
            case 'f': {
                char buffer[20];
                sprintf(buffer, "Seconds: %lu\r\n", seconds);
                UART_Print(buffer);
                break;
            }
            
            default:
                UART_Print("Brak funkcji\r\n");
                break;
        }
    }
    
    return 0;
}
