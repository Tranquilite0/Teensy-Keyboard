#ifndef phantom_h__
#define phantom_h__

void update_leds(void);
void setup_leds(void);
void setup_bounce_timer(void);
void toggle_leds(void);

/* NROW number of rows
   NCOL number of columns
   NKEY = NROW*NCOL 
   NFUN number of function layers */
#define NA    0
#define NROW  6
#define NCOL  17
#define NKEY  102
#define NFUN  7
//LED assignment values
#define LED_A 0b01000000
#define LED_B 0b10000000
#define KANA  3
#define CMPS  4
#define SCRL  5
#define CAPS  6
#define NUM   7

#define INTERRUPT_FUNCTION TIMER0_COMPA_vect

//Switch Values specific to Phantom PCB layout

#define SW0_0                     0
#define SW1_0                     1
#define SW2_0                     2
#define SW3_0                     3
#define SW4_0                     4
#define SW5_0                     5
#define SW0_1                     6
#define SW1_1                     7
#define SW2_1                     8
#define SW3_1                     9
#define SW4_1                    10
#define SW5_1                    11
#define SW0_2                    12
#define SW1_2                    13
#define SW2_2                    14
#define SW3_2                    15
#define SW4_2                    16
#define SW5_2                    17
#define SW0_3                    18
#define SW1_3                    19
#define SW2_3                    20
#define SW3_3                    21
#define SW4_3                    22
#define SW5_3                    23
#define SW0_4                    24
#define SW1_4                    25
#define SW2_4                    26
#define SW3_4                    27
#define SW4_4                    28
#define SW5_4                    29
#define SW0_5                    30
#define SW1_5                    31
#define SW2_5                    32
#define SW3_5                    33
#define SW4_5                    34
#define SW5_5                    35
#define SW0_6                    36
#define SW1_6                    37
#define SW2_6                    38
#define SW3_6                    39
#define SW4_6                    40
#define SW5_6                    41
#define SW0_7                    42
#define SW1_7                    43
#define SW2_7                    44
#define SW3_7                    45
#define SW4_7                    46
#define SW5_7                    47
#define SW0_8                    48
#define SW1_8                    49
#define SW2_8                    50
#define SW3_8                    51
#define SW4_8                    52
#define SW5_8                    53
#define SW0_9                    54
#define SW1_9                    55
#define SW2_9                    56
#define SW3_9                    57
#define SW4_9                    58
#define SW5_9                    59
#define SW0_10                   60
#define SW1_10                   61
#define SW2_10                   62
#define SW3_10                   63
#define SW4_10                   64
#define SW5_10                   65
#define SW0_11                   66
#define SW1_11                   67
#define SW2_11                   68
#define SW3_11                   69
#define SW4_11                   70
#define SW5_11                   71
#define SW0_12                   72
#define SW1_12                   73
#define SW2_12                   74
#define SW3_12                   75
#define SW4_12                   76
#define SW5_12                   77
#define SW0_13                   78
#define SW1_13                   79
#define SW2_13                   80
#define SW3_13                   81
#define SW4_13                   82
#define SW5_13                   83
#define SW0_14                   84
#define SW1_14                   85
#define SW2_14                   86
#define SW3_14                   87
#define SW4_14                   88
#define SW5_14                   89
#define SW0_15                   90
#define SW1_15                   91
#define SW2_15                   92
#define SW3_15                   93
#define SW4_15                   94
#define SW5_15                   95
#define SW0_16                   96
#define SW1_16                   97
#define SW2_16                   98
#define SW3_16                   99
#define SW4_16                  100
#define SW5_16                  101

#endif
