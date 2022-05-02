#ifndef UNITTEST_TRANSPORT_H
#define UNITTEST_TRANSPORT_H

#ifdef __cplusplus
    extern "C"{
#endif

void unittest_uart_begin(void);
void unittest_uart_putchar(char c);
void unittest_uart_flush(void);
void unittest_uart_end(void);

#ifdef __cplusplus
    }
#endif


#endif
