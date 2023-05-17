                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ANSI-C Compiler
                                      3 ; Version 4.2.0 #13081 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module main
                                      6 	.optsdcc -mmcs51 --model-small
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _main
                                     12 	.globl _uart_get
                                     13 	.globl _uart_send
                                     14 	.globl _uart_init
                                     15 	.globl _CY
                                     16 	.globl _AC
                                     17 	.globl _F0
                                     18 	.globl _RS1
                                     19 	.globl _RS0
                                     20 	.globl _OV
                                     21 	.globl _FL
                                     22 	.globl _P
                                     23 	.globl _TF2
                                     24 	.globl _EXF2
                                     25 	.globl _RCLK
                                     26 	.globl _TCLK
                                     27 	.globl _EXEN2
                                     28 	.globl _TR2
                                     29 	.globl _C_T2
                                     30 	.globl _CP_RL2
                                     31 	.globl _T2CON_7
                                     32 	.globl _T2CON_6
                                     33 	.globl _T2CON_5
                                     34 	.globl _T2CON_4
                                     35 	.globl _T2CON_3
                                     36 	.globl _T2CON_2
                                     37 	.globl _T2CON_1
                                     38 	.globl _T2CON_0
                                     39 	.globl _PT2
                                     40 	.globl _PS
                                     41 	.globl _PT1
                                     42 	.globl _PX1
                                     43 	.globl _PT0
                                     44 	.globl _PX0
                                     45 	.globl _RD
                                     46 	.globl _WR
                                     47 	.globl _T1
                                     48 	.globl _T0
                                     49 	.globl _INT1
                                     50 	.globl _INT0
                                     51 	.globl _TXD
                                     52 	.globl _RXD
                                     53 	.globl _P3_7
                                     54 	.globl _P3_6
                                     55 	.globl _P3_5
                                     56 	.globl _P3_4
                                     57 	.globl _P3_3
                                     58 	.globl _P3_2
                                     59 	.globl _P3_1
                                     60 	.globl _P3_0
                                     61 	.globl _EA
                                     62 	.globl _ET2
                                     63 	.globl _ES
                                     64 	.globl _ET1
                                     65 	.globl _EX1
                                     66 	.globl _ET0
                                     67 	.globl _EX0
                                     68 	.globl _P2_7
                                     69 	.globl _P2_6
                                     70 	.globl _P2_5
                                     71 	.globl _P2_4
                                     72 	.globl _P2_3
                                     73 	.globl _P2_2
                                     74 	.globl _P2_1
                                     75 	.globl _P2_0
                                     76 	.globl _SM0
                                     77 	.globl _SM1
                                     78 	.globl _SM2
                                     79 	.globl _REN
                                     80 	.globl _TB8
                                     81 	.globl _RB8
                                     82 	.globl _TI
                                     83 	.globl _RI
                                     84 	.globl _T2EX
                                     85 	.globl _T2
                                     86 	.globl _P1_7
                                     87 	.globl _P1_6
                                     88 	.globl _P1_5
                                     89 	.globl _P1_4
                                     90 	.globl _P1_3
                                     91 	.globl _P1_2
                                     92 	.globl _P1_1
                                     93 	.globl _P1_0
                                     94 	.globl _TF1
                                     95 	.globl _TR1
                                     96 	.globl _TF0
                                     97 	.globl _TR0
                                     98 	.globl _IE1
                                     99 	.globl _IT1
                                    100 	.globl _IE0
                                    101 	.globl _IT0
                                    102 	.globl _P0_7
                                    103 	.globl _P0_6
                                    104 	.globl _P0_5
                                    105 	.globl _P0_4
                                    106 	.globl _P0_3
                                    107 	.globl _P0_2
                                    108 	.globl _P0_1
                                    109 	.globl _P0_0
                                    110 	.globl _nothing
                                    111 	.globl _B
                                    112 	.globl _A
                                    113 	.globl _ACC
                                    114 	.globl _PSW
                                    115 	.globl _TH2
                                    116 	.globl _TL2
                                    117 	.globl _RCAP2H
                                    118 	.globl _RCAP2L
                                    119 	.globl _T2MOD
                                    120 	.globl _T2CON
                                    121 	.globl _IP
                                    122 	.globl _P3
                                    123 	.globl _IE
                                    124 	.globl _P2
                                    125 	.globl _SBUF
                                    126 	.globl _SCON
                                    127 	.globl _P1
                                    128 	.globl _TH1
                                    129 	.globl _TH0
                                    130 	.globl _TL1
                                    131 	.globl _TL0
                                    132 	.globl _TMOD
                                    133 	.globl _TCON
                                    134 	.globl _PCON
                                    135 	.globl _DPH
                                    136 	.globl _DPL
                                    137 	.globl _SP
                                    138 	.globl _P0
                                    139 	.globl _mDelay10uS
                                    140 	.globl _mDelaymS
                                    141 ;--------------------------------------------------------
                                    142 ; special function registers
                                    143 ;--------------------------------------------------------
                                    144 	.area RSEG    (ABS,DATA)
      000000                        145 	.org 0x0000
                           000080   146 _P0	=	0x0080
                           000081   147 _SP	=	0x0081
                           000082   148 _DPL	=	0x0082
                           000083   149 _DPH	=	0x0083
                           000087   150 _PCON	=	0x0087
                           000088   151 _TCON	=	0x0088
                           000089   152 _TMOD	=	0x0089
                           00008A   153 _TL0	=	0x008a
                           00008B   154 _TL1	=	0x008b
                           00008C   155 _TH0	=	0x008c
                           00008D   156 _TH1	=	0x008d
                           000090   157 _P1	=	0x0090
                           000098   158 _SCON	=	0x0098
                           000099   159 _SBUF	=	0x0099
                           0000A0   160 _P2	=	0x00a0
                           0000A8   161 _IE	=	0x00a8
                           0000B0   162 _P3	=	0x00b0
                           0000B8   163 _IP	=	0x00b8
                           0000C8   164 _T2CON	=	0x00c8
                           0000C9   165 _T2MOD	=	0x00c9
                           0000CA   166 _RCAP2L	=	0x00ca
                           0000CB   167 _RCAP2H	=	0x00cb
                           0000CC   168 _TL2	=	0x00cc
                           0000CD   169 _TH2	=	0x00cd
                           0000D0   170 _PSW	=	0x00d0
                           0000E0   171 _ACC	=	0x00e0
                           0000E0   172 _A	=	0x00e0
                           0000F0   173 _B	=	0x00f0
                           0000FF   174 _nothing	=	0x00ff
                                    175 ;--------------------------------------------------------
                                    176 ; special function bits
                                    177 ;--------------------------------------------------------
                                    178 	.area RSEG    (ABS,DATA)
      000000                        179 	.org 0x0000
                           000080   180 _P0_0	=	0x0080
                           000081   181 _P0_1	=	0x0081
                           000082   182 _P0_2	=	0x0082
                           000083   183 _P0_3	=	0x0083
                           000084   184 _P0_4	=	0x0084
                           000085   185 _P0_5	=	0x0085
                           000086   186 _P0_6	=	0x0086
                           000087   187 _P0_7	=	0x0087
                           000088   188 _IT0	=	0x0088
                           000089   189 _IE0	=	0x0089
                           00008A   190 _IT1	=	0x008a
                           00008B   191 _IE1	=	0x008b
                           00008C   192 _TR0	=	0x008c
                           00008D   193 _TF0	=	0x008d
                           00008E   194 _TR1	=	0x008e
                           00008F   195 _TF1	=	0x008f
                           000090   196 _P1_0	=	0x0090
                           000091   197 _P1_1	=	0x0091
                           000092   198 _P1_2	=	0x0092
                           000093   199 _P1_3	=	0x0093
                           000094   200 _P1_4	=	0x0094
                           000095   201 _P1_5	=	0x0095
                           000096   202 _P1_6	=	0x0096
                           000097   203 _P1_7	=	0x0097
                           000090   204 _T2	=	0x0090
                           000091   205 _T2EX	=	0x0091
                           000098   206 _RI	=	0x0098
                           000099   207 _TI	=	0x0099
                           00009A   208 _RB8	=	0x009a
                           00009B   209 _TB8	=	0x009b
                           00009C   210 _REN	=	0x009c
                           00009D   211 _SM2	=	0x009d
                           00009E   212 _SM1	=	0x009e
                           00009F   213 _SM0	=	0x009f
                           0000A0   214 _P2_0	=	0x00a0
                           0000A1   215 _P2_1	=	0x00a1
                           0000A2   216 _P2_2	=	0x00a2
                           0000A3   217 _P2_3	=	0x00a3
                           0000A4   218 _P2_4	=	0x00a4
                           0000A5   219 _P2_5	=	0x00a5
                           0000A6   220 _P2_6	=	0x00a6
                           0000A7   221 _P2_7	=	0x00a7
                           0000A8   222 _EX0	=	0x00a8
                           0000A9   223 _ET0	=	0x00a9
                           0000AA   224 _EX1	=	0x00aa
                           0000AB   225 _ET1	=	0x00ab
                           0000AC   226 _ES	=	0x00ac
                           0000AD   227 _ET2	=	0x00ad
                           0000AF   228 _EA	=	0x00af
                           0000B0   229 _P3_0	=	0x00b0
                           0000B1   230 _P3_1	=	0x00b1
                           0000B2   231 _P3_2	=	0x00b2
                           0000B3   232 _P3_3	=	0x00b3
                           0000B4   233 _P3_4	=	0x00b4
                           0000B5   234 _P3_5	=	0x00b5
                           0000B6   235 _P3_6	=	0x00b6
                           0000B7   236 _P3_7	=	0x00b7
                           0000B0   237 _RXD	=	0x00b0
                           0000B1   238 _TXD	=	0x00b1
                           0000B2   239 _INT0	=	0x00b2
                           0000B3   240 _INT1	=	0x00b3
                           0000B4   241 _T0	=	0x00b4
                           0000B5   242 _T1	=	0x00b5
                           0000B6   243 _WR	=	0x00b6
                           0000B7   244 _RD	=	0x00b7
                           0000B8   245 _PX0	=	0x00b8
                           0000B9   246 _PT0	=	0x00b9
                           0000BA   247 _PX1	=	0x00ba
                           0000BB   248 _PT1	=	0x00bb
                           0000BC   249 _PS	=	0x00bc
                           0000BD   250 _PT2	=	0x00bd
                           0000C8   251 _T2CON_0	=	0x00c8
                           0000C9   252 _T2CON_1	=	0x00c9
                           0000CA   253 _T2CON_2	=	0x00ca
                           0000CB   254 _T2CON_3	=	0x00cb
                           0000CC   255 _T2CON_4	=	0x00cc
                           0000CD   256 _T2CON_5	=	0x00cd
                           0000CE   257 _T2CON_6	=	0x00ce
                           0000CF   258 _T2CON_7	=	0x00cf
                           0000C8   259 _CP_RL2	=	0x00c8
                           0000C9   260 _C_T2	=	0x00c9
                           0000CA   261 _TR2	=	0x00ca
                           0000CB   262 _EXEN2	=	0x00cb
                           0000CC   263 _TCLK	=	0x00cc
                           0000CD   264 _RCLK	=	0x00cd
                           0000CE   265 _EXF2	=	0x00ce
                           0000CF   266 _TF2	=	0x00cf
                           0000D0   267 _P	=	0x00d0
                           0000D1   268 _FL	=	0x00d1
                           0000D2   269 _OV	=	0x00d2
                           0000D3   270 _RS0	=	0x00d3
                           0000D4   271 _RS1	=	0x00d4
                           0000D5   272 _F0	=	0x00d5
                           0000D6   273 _AC	=	0x00d6
                           0000D7   274 _CY	=	0x00d7
                                    275 ;--------------------------------------------------------
                                    276 ; overlayable register banks
                                    277 ;--------------------------------------------------------
                                    278 	.area REG_BANK_0	(REL,OVR,DATA)
      000000                        279 	.ds 8
                                    280 ;--------------------------------------------------------
                                    281 ; internal ram data
                                    282 ;--------------------------------------------------------
                                    283 	.area DSEG    (DATA)
                                    284 ;--------------------------------------------------------
                                    285 ; overlayable items in internal ram
                                    286 ;--------------------------------------------------------
                                    287 	.area	OSEG    (OVR,DATA)
                                    288 	.area	OSEG    (OVR,DATA)
                                    289 ;--------------------------------------------------------
                                    290 ; Stack segment in internal ram
                                    291 ;--------------------------------------------------------
                                    292 	.area	SSEG
      000008                        293 __start__stack:
      000008                        294 	.ds	1
                                    295 
                                    296 ;--------------------------------------------------------
                                    297 ; indirectly addressable internal ram data
                                    298 ;--------------------------------------------------------
                                    299 	.area ISEG    (DATA)
                                    300 ;--------------------------------------------------------
                                    301 ; absolute internal ram data
                                    302 ;--------------------------------------------------------
                                    303 	.area IABS    (ABS,DATA)
                                    304 	.area IABS    (ABS,DATA)
                                    305 ;--------------------------------------------------------
                                    306 ; bit data
                                    307 ;--------------------------------------------------------
                                    308 	.area BSEG    (BIT)
                                    309 ;--------------------------------------------------------
                                    310 ; paged external ram data
                                    311 ;--------------------------------------------------------
                                    312 	.area PSEG    (PAG,XDATA)
                                    313 ;--------------------------------------------------------
                                    314 ; external ram data
                                    315 ;--------------------------------------------------------
                                    316 	.area XSEG    (XDATA)
                                    317 ;--------------------------------------------------------
                                    318 ; absolute external ram data
                                    319 ;--------------------------------------------------------
                                    320 	.area XABS    (ABS,XDATA)
                                    321 ;--------------------------------------------------------
                                    322 ; external initialized ram data
                                    323 ;--------------------------------------------------------
                                    324 	.area XISEG   (XDATA)
                                    325 	.area HOME    (CODE)
                                    326 	.area GSINIT0 (CODE)
                                    327 	.area GSINIT1 (CODE)
                                    328 	.area GSINIT2 (CODE)
                                    329 	.area GSINIT3 (CODE)
                                    330 	.area GSINIT4 (CODE)
                                    331 	.area GSINIT5 (CODE)
                                    332 	.area GSINIT  (CODE)
                                    333 	.area GSFINAL (CODE)
                                    334 	.area CSEG    (CODE)
                                    335 ;--------------------------------------------------------
                                    336 ; interrupt vector
                                    337 ;--------------------------------------------------------
                                    338 	.area HOME    (CODE)
      000000                        339 __interrupt_vect:
      000000 02 00 06         [24]  340 	ljmp	__sdcc_gsinit_startup
                                    341 ;--------------------------------------------------------
                                    342 ; global & static initialisations
                                    343 ;--------------------------------------------------------
                                    344 	.area HOME    (CODE)
                                    345 	.area GSINIT  (CODE)
                                    346 	.area GSFINAL (CODE)
                                    347 	.area GSINIT  (CODE)
                                    348 	.globl __sdcc_gsinit_startup
                                    349 	.globl __sdcc_program_startup
                                    350 	.globl __start__stack
                                    351 	.globl __mcs51_genXINIT
                                    352 	.globl __mcs51_genXRAMCLEAR
                                    353 	.globl __mcs51_genRAMCLEAR
                                    354 	.area GSFINAL (CODE)
      00005F 02 00 03         [24]  355 	ljmp	__sdcc_program_startup
                                    356 ;--------------------------------------------------------
                                    357 ; Home
                                    358 ;--------------------------------------------------------
                                    359 	.area HOME    (CODE)
                                    360 	.area HOME    (CODE)
      000003                        361 __sdcc_program_startup:
      000003 02 00 B3         [24]  362 	ljmp	_main
                                    363 ;	return from main will return to caller
                                    364 ;--------------------------------------------------------
                                    365 ; code
                                    366 ;--------------------------------------------------------
                                    367 	.area CSEG    (CODE)
                                    368 ;------------------------------------------------------------
                                    369 ;Allocation info for local variables in function 'mDelay10uS'
                                    370 ;------------------------------------------------------------
                                    371 ;n                         Allocated to registers 
                                    372 ;------------------------------------------------------------
                                    373 ;	main.c:46: void mDelay10uS(unsigned int n)  // Delay in units of 10 uS
                                    374 ;	-----------------------------------------
                                    375 ;	 function mDelay10uS
                                    376 ;	-----------------------------------------
      000062                        377 _mDelay10uS:
                           000007   378 	ar7 = 0x07
                           000006   379 	ar6 = 0x06
                           000005   380 	ar5 = 0x05
                           000004   381 	ar4 = 0x04
                           000003   382 	ar3 = 0x03
                           000002   383 	ar2 = 0x02
                           000001   384 	ar1 = 0x01
                           000000   385 	ar0 = 0x00
      000062 AE 82            [24]  386 	mov	r6,dpl
      000064 AF 83            [24]  387 	mov	r7,dph
                                    388 ;	main.c:48: while ( n )
      000066                        389 00101$:
      000066 EE               [12]  390 	mov	a,r6
      000067 4F               [12]  391 	orl	a,r7
      000068 60 09            [24]  392 	jz	00104$
                                    393 ;	main.c:50: ++ nothing;
      00006A 05 FF            [12]  394 	inc	_nothing
                                    395 ;	main.c:51: -- n;
      00006C 1E               [12]  396 	dec	r6
      00006D BE FF 01         [24]  397 	cjne	r6,#0xff,00116$
      000070 1F               [12]  398 	dec	r7
      000071                        399 00116$:
      000071 80 F3            [24]  400 	sjmp	00101$
      000073                        401 00104$:
                                    402 ;	main.c:53: }
      000073 22               [24]  403 	ret
                                    404 ;------------------------------------------------------------
                                    405 ;Allocation info for local variables in function 'mDelaymS'
                                    406 ;------------------------------------------------------------
                                    407 ;n                         Allocated to registers 
                                    408 ;------------------------------------------------------------
                                    409 ;	main.c:55: void mDelaymS(unsigned int n)   // Delay in mS
                                    410 ;	-----------------------------------------
                                    411 ;	 function mDelaymS
                                    412 ;	-----------------------------------------
      000074                        413 _mDelaymS:
      000074 AE 82            [24]  414 	mov	r6,dpl
      000076 AF 83            [24]  415 	mov	r7,dph
                                    416 ;	main.c:57: while ( n )
      000078                        417 00101$:
      000078 EE               [12]  418 	mov	a,r6
      000079 4F               [12]  419 	orl	a,r7
      00007A 60 15            [24]  420 	jz	00104$
                                    421 ;	main.c:59: mDelay10uS(100);
      00007C 90 00 64         [24]  422 	mov	dptr,#0x0064
      00007F C0 07            [24]  423 	push	ar7
      000081 C0 06            [24]  424 	push	ar6
      000083 12 00 62         [24]  425 	lcall	_mDelay10uS
      000086 D0 06            [24]  426 	pop	ar6
      000088 D0 07            [24]  427 	pop	ar7
                                    428 ;	main.c:60: -- n;
      00008A 1E               [12]  429 	dec	r6
      00008B BE FF 01         [24]  430 	cjne	r6,#0xff,00116$
      00008E 1F               [12]  431 	dec	r7
      00008F                        432 00116$:
      00008F 80 E7            [24]  433 	sjmp	00101$
      000091                        434 00104$:
                                    435 ;	main.c:62: }
      000091 22               [24]  436 	ret
                                    437 ;------------------------------------------------------------
                                    438 ;Allocation info for local variables in function 'uart_init'
                                    439 ;------------------------------------------------------------
                                    440 ;	main.c:64: void uart_init(void) //UART Initialization
                                    441 ;	-----------------------------------------
                                    442 ;	 function uart_init
                                    443 ;	-----------------------------------------
      000092                        444 _uart_init:
                                    445 ;	main.c:66: PCON = 0x80;    //double baud rate
      000092 75 87 80         [24]  446 	mov	_PCON,#0x80
                                    447 ;	main.c:67: SCON = 0x50;    // Asynchronous mode, 8-bit data and 1-stop bit
      000095 75 98 50         [24]  448 	mov	_SCON,#0x50
                                    449 ;	main.c:68: TMOD = 0x20;    //Timer1 in Mode2.
      000098 75 89 20         [24]  450 	mov	_TMOD,#0x20
                                    451 ;	main.c:69: TH1 = 256UL - XTALFREQ / (6UL * 32UL * BAUDRATE); // Load timer value for baudrate generation
      00009B 75 8D F3         [24]  452 	mov	_TH1,#0xf3
                                    453 ;	main.c:70: TR1 = 1;        //Turn ON the timer for Baud rate generation
                                    454 ;	assignBit
      00009E D2 8E            [12]  455 	setb	_TR1
                                    456 ;	main.c:71: }
      0000A0 22               [24]  457 	ret
                                    458 ;------------------------------------------------------------
                                    459 ;Allocation info for local variables in function 'uart_send'
                                    460 ;------------------------------------------------------------
                                    461 ;ch                        Allocated to registers 
                                    462 ;------------------------------------------------------------
                                    463 ;	main.c:73: void uart_send(unsigned char ch) // Function to Transmit the Data
                                    464 ;	-----------------------------------------
                                    465 ;	 function uart_send
                                    466 ;	-----------------------------------------
      0000A1                        467 _uart_send:
      0000A1 85 82 99         [24]  468 	mov	_SBUF,dpl
                                    469 ;	main.c:76: while(TI == 0); // Wait till the data is trasmitted
      0000A4                        470 00101$:
                                    471 ;	main.c:77: TI = 0;         //Clear the Tx flag for next cycle.
                                    472 ;	assignBit
      0000A4 10 99 02         [24]  473 	jbc	_TI,00114$
      0000A7 80 FB            [24]  474 	sjmp	00101$
      0000A9                        475 00114$:
                                    476 ;	main.c:78: }
      0000A9 22               [24]  477 	ret
                                    478 ;------------------------------------------------------------
                                    479 ;Allocation info for local variables in function 'uart_get'
                                    480 ;------------------------------------------------------------
                                    481 ;	main.c:81: unsigned char uart_get(void) // Fuction to Recieve Data
                                    482 ;	-----------------------------------------
                                    483 ;	 function uart_get
                                    484 ;	-----------------------------------------
      0000AA                        485 _uart_get:
                                    486 ;	main.c:83: while(RI == 0); // Wait till the data is received
      0000AA                        487 00101$:
                                    488 ;	main.c:84: RI = 0;         // Clear Receive Interrupt Flag for next cycle
                                    489 ;	assignBit
      0000AA 10 98 02         [24]  490 	jbc	_RI,00114$
      0000AD 80 FB            [24]  491 	sjmp	00101$
      0000AF                        492 00114$:
                                    493 ;	main.c:85: return SBUF;    // return the received char
      0000AF 85 99 82         [24]  494 	mov	dpl,_SBUF
                                    495 ;	main.c:86: }
      0000B2 22               [24]  496 	ret
                                    497 ;------------------------------------------------------------
                                    498 ;Allocation info for local variables in function 'main'
                                    499 ;------------------------------------------------------------
                                    500 ;msg                       Allocated to registers 
                                    501 ;data                      Allocated to registers 
                                    502 ;d                         Allocated to registers r7 
                                    503 ;------------------------------------------------------------
                                    504 ;	main.c:88: void main(void)
                                    505 ;	-----------------------------------------
                                    506 ;	 function main
                                    507 ;	-----------------------------------------
      0000B3                        508 _main:
                                    509 ;	main.c:90: const char* msg = "Hello World!";
                                    510 ;	main.c:93: uart_init();
      0000B3 12 00 92         [24]  511 	lcall	_uart_init
                                    512 ;	main.c:95: for(unsigned char d = 0; msg[d]; ++d)
      0000B6 7F 00            [12]  513 	mov	r7,#0x00
      0000B8                        514 00106$:
      0000B8 EF               [12]  515 	mov	a,r7
      0000B9 24 02            [12]  516 	add	a,#___str_0
      0000BB FC               [12]  517 	mov	r4,a
      0000BC E4               [12]  518 	clr	a
      0000BD 34 01            [12]  519 	addc	a,#(___str_0 >> 8)
      0000BF FD               [12]  520 	mov	r5,a
      0000C0 7E 80            [12]  521 	mov	r6,#0x80
      0000C2 8C 82            [24]  522 	mov	dpl,r4
      0000C4 8D 83            [24]  523 	mov	dph,r5
      0000C6 8E F0            [24]  524 	mov	b,r6
      0000C8 12 00 E2         [24]  525 	lcall	__gptrget
      0000CB FE               [12]  526 	mov	r6,a
      0000CC 60 0C            [24]  527 	jz	00103$
                                    528 ;	main.c:97: uart_send(msg[d]); // Transmit predefined string
      0000CE 8E 82            [24]  529 	mov	dpl,r6
      0000D0 C0 07            [24]  530 	push	ar7
      0000D2 12 00 A1         [24]  531 	lcall	_uart_send
      0000D5 D0 07            [24]  532 	pop	ar7
                                    533 ;	main.c:95: for(unsigned char d = 0; msg[d]; ++d)
      0000D7 0F               [12]  534 	inc	r7
                                    535 ;	main.c:100: while(1)
      0000D8 80 DE            [24]  536 	sjmp	00106$
      0000DA                        537 00103$:
                                    538 ;	main.c:102: data = uart_get(); // Receive a char from serial port
      0000DA 12 00 AA         [24]  539 	lcall	_uart_get
                                    540 ;	main.c:103: uart_send(data);    // Transmit the received char
      0000DD 12 00 A1         [24]  541 	lcall	_uart_send
                                    542 ;	main.c:105: }
      0000E0 80 F8            [24]  543 	sjmp	00103$
                                    544 	.area CSEG    (CODE)
                                    545 	.area CONST   (CODE)
                                    546 	.area CONST   (CODE)
      000102                        547 ___str_0:
      000102 48 65 6C 6C 6F 20 57   548 	.ascii "Hello World!"
             6F 72 6C 64 21
      00010E 00                     549 	.db 0x00
                                    550 	.area CSEG    (CODE)
                                    551 	.area XINIT   (CODE)
                                    552 	.area CABS    (ABS,CODE)
