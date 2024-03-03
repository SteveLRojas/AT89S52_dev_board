// SDCC
#define uint8_t unsigned char
#define uint16_t unsigned short

void mDelay10uS(unsigned int n);              // Delay in uS
void mDelaymS(unsigned int n);              // Delay in mS

void timer2_init(void);

void rx_fifo_push(uint8_t data);
uint8_t rx_fifo_pop(void);
void tx_fifo_push(uint8_t  data);
uint8_t tx_fifo_pop(void);
void uart_init(void); //UART Initialization
void uart_send(uint8_t data); // Function to Transmit the Data
uint8_t uart_get(void); // Fuction to Recieve Data
uint8_t uart_write_string(const char* str);
uint8_t uart_get_string(char* buf, uint8_t size);

uint16_t get_hex();
uint16_t get_dec();
