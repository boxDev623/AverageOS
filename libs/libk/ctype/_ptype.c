#include "_ctype.h"

const unsigned short int _ctype[384] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /*  -1 EOF   */ 0,
        /*   0 (NUL) */ _CNTRL,
        /*   1 (SOH) */ _CNTRL,
        /*   2 (STX) */ _CNTRL,
        /*   3 (ETX) */ _CNTRL,
        /*   4 (EOT) */ _CNTRL,
        /*   5 (ENQ) */ _CNTRL,
        /*   6 (ACK) */ _CNTRL,
        /*   7 (BEL) */ _CNTRL,
        /*   8 (BS)  */ _CNTRL,
        /*   9 (HT)  */ _SPACE | _CNTRL,
        /*  10 (LF)  */ _SPACE | _CNTRL,
        /*  11 (VT)  */ _SPACE | _CNTRL,
        /*  12 (FF)  */ _SPACE | _CNTRL,
        /*  13 (CR)  */ _SPACE | _CNTRL,
        /*  14 (SI)  */ _CNTRL,
        /*  15 (SO)  */ _CNTRL,
        /*  16 (DLE) */ _CNTRL,
        /*  17 (DC1) */ _CNTRL,
        /*  18 (DC2) */ _CNTRL,
        /*  19 (DC3) */ _CNTRL,
        /*  20 (DC4) */ _CNTRL,
        /*  21 (NAK) */ _CNTRL,
        /*  22 (SYN) */ _CNTRL,
        /*  23 (ETB) */ _CNTRL,
        /*  24 (CAN) */ _CNTRL,
        /*  25 (EM)  */ _CNTRL,
        /*  26 (SUB) */ _CNTRL,
        /*  27 (ESC) */ _CNTRL,
        /*  28 (FS)  */ _CNTRL,
        /*  29 (GS)  */ _CNTRL,
        /*  30 (RS)  */ _CNTRL,
        /*  31 (US)  */ _CNTRL,
        /*  32 SPACE */ _SPACE | _BLANK,
        /*  33 !     */ _PUNCT,
        /*  34 "     */ _PUNCT,
        /*  35 #     */ _PUNCT,
        /*  36 $     */ _PUNCT,
        /*  37 %     */ _PUNCT,
        /*  38 &     */ _PUNCT,
        /*  39 '     */ _PUNCT,
        /*  40 (     */ _PUNCT,
        /*  41 )     */ _PUNCT,
        /*  42 *     */ _PUNCT,
        /*  43 +     */ _PUNCT,
        /*  44 ,     */ _PUNCT,
        /*  45 -     */ _PUNCT,
        /*  46 .     */ _PUNCT,
        /*  47 /     */ _PUNCT,
        /*  48 0     */ _DIGIT | _XDIGIT,
        /*  49 1     */ _DIGIT | _XDIGIT,
        /*  50 2     */ _DIGIT | _XDIGIT,
        /*  51 3     */ _DIGIT | _XDIGIT,
        /*  52 4     */ _DIGIT | _XDIGIT,
        /*  53 5     */ _DIGIT | _XDIGIT,
        /*  54 6     */ _DIGIT | _XDIGIT,
        /*  55 7     */ _DIGIT | _XDIGIT,
        /*  56 8     */ _DIGIT | _XDIGIT,
        /*  57 9     */ _DIGIT | _XDIGIT,
        /*  58 :     */ _PUNCT,
        /*  59 ;     */ _PUNCT,
        /*  60 <     */ _PUNCT,
        /*  61 =     */ _PUNCT,
        /*  62 >     */ _PUNCT,
        /*  63 ?     */ _PUNCT,
        /*  64 @     */ _PUNCT,
        /*  65 A     */ _UPPER | _XDIGIT,
        /*  66 B     */ _UPPER | _XDIGIT,
        /*  67 C     */ _UPPER | _XDIGIT,
        /*  68 D     */ _UPPER | _XDIGIT,
        /*  69 E     */ _UPPER | _XDIGIT,
        /*  70 F     */ _UPPER | _XDIGIT,
        /*  71 G     */ _UPPER,
        /*  72 H     */ _UPPER,
        /*  73 I     */ _UPPER,
        /*  74 J     */ _UPPER,
        /*  75 K     */ _UPPER,
        /*  76 L     */ _UPPER,
        /*  77 M     */ _UPPER,
        /*  78 N     */ _UPPER,
        /*  79 O     */ _UPPER,
        /*  80 P     */ _UPPER,
        /*  81 Q     */ _UPPER,
        /*  82 R     */ _UPPER,
        /*  83 S     */ _UPPER,
        /*  84 T     */ _UPPER,
        /*  85 U     */ _UPPER,
        /*  86 V     */ _UPPER,
        /*  87 W     */ _UPPER,
        /*  88 X     */ _UPPER,
        /*  89 Y     */ _UPPER,
        /*  90 Z     */ _UPPER,
        /*  91 [     */ _PUNCT,
        /*  92 \     */ _PUNCT,
        /*  93 ]     */ _PUNCT,
        /*  94 ^     */ _PUNCT,
        /*  95 _     */ _PUNCT,
        /*  96 `     */ _PUNCT,
        /*  97 a     */ _LOWER | _XDIGIT,
        /*  98 b     */ _LOWER | _XDIGIT,
        /*  99 c     */ _LOWER | _XDIGIT,
        /* 100 d     */ _LOWER | _XDIGIT,
        /* 101 e     */ _LOWER | _XDIGIT,
        /* 102 f     */ _LOWER | _XDIGIT,
        /* 103 g     */ _LOWER,
        /* 104 h     */ _LOWER,
        /* 105 i     */ _LOWER,
        /* 106 j     */ _LOWER,
        /* 107 k     */ _LOWER,
        /* 108 l     */ _LOWER,
        /* 109 m     */ _LOWER,
        /* 110 n     */ _LOWER,
        /* 111 o     */ _LOWER,
        /* 112 p     */ _LOWER,
        /* 113 q     */ _LOWER,
        /* 114 r     */ _LOWER,
        /* 115 s     */ _LOWER,
        /* 116 t     */ _LOWER,
        /* 117 u     */ _LOWER,
        /* 118 v     */ _LOWER,
        /* 119 w     */ _LOWER,
        /* 120 x     */ _LOWER,
        /* 121 y     */ _LOWER,
        /* 122 z     */ _LOWER,
        /* 123 {     */ _PUNCT,
        /* 124 |     */ _PUNCT,
        /* 125 }     */ _PUNCT,
        /* 126 ~     */ _PUNCT,
        /* 127 (DEL) */ _CNTRL,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const unsigned short int *_ptype = &_ctype[128];