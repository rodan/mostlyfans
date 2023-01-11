
#include <msp430.h>
#include <stdio.h>
#include <string.h>

#include "proj.h"
#include "driverlib.h"
#include "glue.h"
#include "intertechno.h"
#include "rf1a.h"
#include "radio.h"
#include "ui.h"
#include "sig.h"

uart_descriptor bc; // backchannel uart interface

static void uart_bcl_rx_irq(uint32_t msg)
{
    parse_user_input();
    uart_set_eol(&bc);
}

void check_events(void)
{
    uint16_t msg = SYS_MSG_NULL;

    // uart RX
    if (uart_get_event(&bc) & UART_EV_RX) {
        msg |= SYS_MSG_UART_BCL_RX;
        uart_rst_event(&bc);
    }
    // transceiver
    if (radio_get_event() & RADIO_TX_IRQ) {
        msg |= SYS_MSG_CC_TX;
        radio_rst_event();
    } else if (radio_get_event() & RADIO_RX_IRQ) {
        msg |= SYS_MSG_CC_RX;
        radio_rst_event();
    }

    eh_exec(msg);
}

int main(void)
{
    // stop watchdog
    WDTCTL = WDTPW | WDTHOLD;
    msp430_hal_init(HAL_GPIO_DIR_OUTPUT | HAL_GPIO_OUT_LOW);
#ifdef USE_SIG
    sig0_on;
#endif
    
    // enable GDO1
    P3SEL |= BIT6;

    PMM_setVCore(2);
    clock_pin_init();
    clock_init();

    bc.baseAddress = USCI_A0_BASE;
    bc.baudrate = BAUDRATE_57600;
    uart_pin_init(&bc);
    uart_init(&bc);
#if defined UART_RX_USES_RINGBUF
    uart_set_rx_irq_handler(&bc, uart_rx_ringbuf_handler);
#else
    uart_set_rx_irq_handler(&bc, uart_rx_simple_handler);
#endif

    ResetRadioCore();
    InitRadio();

    it_handler_init();
    it_rx_on();
    radio_rx_on();

#ifdef USE_SIG
    sig0_off;
    sig1_off;
    sig2_off;
    sig3_off;
    sig4_off;
    //sig5_off;
#endif

    eh_init();
    eh_register(&uart_bcl_rx_irq, SYS_MSG_UART_BCL_RX);
    _BIS_SR(GIE);

    display_version();

    while (1) {
        // sleep
//#ifdef USE_SIG
        sig4_off;
//#endif
        _BIS_SR(LPM0_bits + GIE);
//#ifdef USE_SIG
        sig4_on;
//#endif
        __no_operation();
//#ifdef USE_WATCHDOG
//        WDTCTL = (WDTCTL & 0xff) | WDTPW | WDTCNTCL;
//#endif
        check_events();
        check_events();
        check_events();

        if (radio_get_state() == RADIO_STATE_IDLE) {
            it_rx_on();
            radio_rx_on();
        }
    }

}


