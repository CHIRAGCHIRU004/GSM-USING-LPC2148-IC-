#include <LPC214x.h>	

// === Delay Function ===
void delay(unsigned int time) {
    unsigned int i, j;
    for (i = 0; i < time; i++)
        for (j = 0; j < 500; j++);
}

// === UART0 Initialization ===
void Uart0Init(void) {
    PINSEL0 = 0x00000005;   // Enable TxD0 (P0.0) and RxD0 (P0.1)
    U0LCR = 0x83;           // Enable DLAB, 8-bit word length
    U0DLM = 0x00;
    U0DLL = 97;             // 9600 baud rate for 15 MHz PCLK
    U0LCR = 0x03;           // Disable DLAB
}

// === UART0 Send a Character ===
void UART0_PutChar(unsigned char ch) {
    while (!(U0LSR & 0x20));  // Wait until THR is empty
    U0THR = ch;
}

// === UART0 Send a String ===
void UART0_PutS(char *str) {
    while (*str) {
        UART0_PutChar(*str++);
    }
}

// === Main Function ===
int main(void) {
    Uart0Init();

    delay(1000);
    UART0_PutS("ATE0\r\n");                      // Echo off
    delay(1000);
    UART0_PutS("ATD6360214617;\r\n");            // Dial number
    delay(10000);
    UART0_PutS("ATH0\r");                        // Hang up
    delay(1000);
    UART0_PutS("AT+CMGF=1\r\n");                 // Set text mode
    delay(1000);
    UART0_PutS("AT+CMGS=\"6360214617\"\r\n");    // SMS to number
    delay(1000);
    UART0_PutS("Welcome to BIET\r");          // Message body
    delay(1000);
    UART0_PutChar(0x1A);                         // End SMS (CTRL+Z)

    while (1); // Infinite loop
}
