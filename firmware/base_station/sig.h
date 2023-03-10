#ifndef __SIG_H__
#define __SIG_H__

#include <msp430.h>

#if 0
#define sig0_on              P1OUT |= BIT0
#define sig0_off             P1OUT &= ~BIT0
#define sig0_switch          P1OUT ^= BIT0
#endif

#define sig1_on              P3OUT |= BIT0
#define sig1_off             P3OUT &= ~BIT0
#define sig1_switch          P3OUT ^= BIT0

#define sig2_on              P3OUT |= BIT1
#define sig2_off             P3OUT &= ~BIT1
#define sig2_switch          P3OUT ^= BIT1

#define sig3_on              P3OUT |= BIT2
#define sig3_off             P3OUT &= ~BIT2
#define sig3_switch          P3OUT ^= BIT2

#define sig4_on              P3OUT |= BIT3
#define sig4_off             P3OUT &= ~BIT3
#define sig4_switch          P3OUT ^= BIT3

#endif
