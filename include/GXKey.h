//
//  GXKey.h
//  GX
//
//  Created by Manuel Deneu on 02/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXKey_h
#define GXKey_h


typedef enum
{
    GXKey_Unknown = -1,
    
    /* Printable keys */
    GXKey_SPACE          =    32,
    GXKey_APOSTROPHE     =    39,  /* ' */
    GXKey_COMMA          =    44,  /* , */
    GXKey_MINUS          =    45,  /* - */
    GXKey_PERIOD         =    46,  /* . */
    GXKey_SLASH          =    47,  /* / */
    GXKey_0              =    48,
    GXKey_1              =    49,
    GXKey_2              =    50,
    GXKey_3              =    51,
    GXKey_4              =    52,
    GXKey_5              =    53,
    GXKey_6              =    54,
    GXKey_7              =    55,
    GXKey_8              =    56,
    GXKey_9              =    57,
    GXKey_SEMICOLON      =    59,  /* ; */
    GXKey_EQUAL          =    61,  /* = */
    GXKey_A              =    65,
    GXKey_B              =    66,
    GXKey_C              =    67,
    GXKey_D              =    68,
    GXKey_E              =    69,
    GXKey_F              =    70,
    GXKey_G              =    71,
    GXKey_H              =    72,
    GXKey_I              =    73,
    GXKey_J              =    74,
    GXKey_K              =    75,
    GXKey_L              =    76,
    GXKey_M              =    77,
    GXKey_N              =    78,
    GXKey_O              =    79,
    GXKey_P              =    80,
    GXKey_Q              =    81,
    GXKey_R              =    82,
    GXKey_S              =    83,
    GXKey_T              =    84,
    GXKey_U              =    85,
    GXKey_V              =    86,
    GXKey_W              =    87,
    GXKey_X              =    88,
    GXKey_Y              =    89,
    GXKey_Z              =    90,
    GXKey_LEFT_BRACKET   =    91, /* [ */
    GXKey_BACKSLASH      =    92,  /* \ */
    GXKey_RIGHT_BRACKET  =    93, /* ] */
    GXKey_GRAVE_ACCENT   =    96,  /* ` */
    GXKey_WORLD_1        =    161, /* non-US #1 */
    GXKey_WORLD_2        =    162, /* non-US #2 */
    
    /* Function keys */
    GXKey_ESCAPE         =    256,
    GXKey_ENTER          =    257,
    GXKey_TAB            =    258,
    GXKey_BACKSPACE      =    259,
    GXKey_INSERT         =    260,
    GXKey_DELETE         =    261,
    GXKey_RIGHT          =    262,
    GXKey_LEFT           =    263,
    GXKey_DOWN           =    264,
    GXKey_UP             =    265,
    GXKey_PAGE_UP        =    266,
    GXKey_PAGE_DOWN      =    267,
    GXKey_HOME           =    268,
    GXKey_END            =    269,
    GXKey_CAPS_LOCK      =    280,
    GXKey_SCROLL_LOCK    =    281,
    GXKey_NUM_LOCK       =    282,
    GXKey_PRINT_SCREEN   =    283,
    GXKey_PAUSE          =    284,
    GXKey_F1             =    290,
    GXKey_F2             =    291,
    GXKey_F3             =    292,
    GXKey_F4             =    293,
    GXKey_F5             =    294,
    GXKey_F6             =    295,
    GXKey_F7             =    296,
    GXKey_F8             =    297,
    GXKey_F9             =    298,
    GXKey_F10            =    299,
    GXKey_F11            =    300,
    GXKey_F12            =    301,
    GXKey_F13            =    302,
    GXKey_F14            =    303,
    GXKey_F15            =    304,
    GXKey_F16            =    305,
    GXKey_F17            =    306,
    GXKey_F18            =    307,
    GXKey_F19            =    308,
    GXKey_F20            =    309,
    GXKey_F21            =    310,
    GXKey_F22            =    311,
    GXKey_F23            =    312,
    GXKey_F24            =    313,
    GXKey_F25            =    314,
    GXKey_KP_0           =    320,
    GXKey_KP_1           =    321,
    GXKey_KP_2           =    322,
    GXKey_KP_3           =    323,
    GXKey_KP_4           =    324,
    GXKey_KP_5           =    325,
    GXKey_KP_6           =    326,
    GXKey_KP_7           =    327,
    GXKey_KP_8           =    328,
    GXKey_KP_9           =    329,
    GXKey_KP_DECIMAL     =    330,
    GXKey_KP_DIVIDE      =    331,
    GXKey_KP_MULTIPLY    =    332,
    GXKey_KP_SUBTRACT    =    333,
    GXKey_KP_ADD         =    334,
    GXKey_KP_ENTER       =    335,
    GXKey_KP_EQUAL       =    336,
    GXKey_LEFT_SHIFT     =    340,
    GXKey_LEFT_CONTROL   =    341,
    GXKey_LEFT_ALT       =    342,
    GXKey_LEFT_SUPER     =    343,
    GXKey_RIGHT_SHIFT    =    344,
    GXKey_RIGHT_CONTROL  =    345,
    GXKey_RIGHT_ALT      =    346,
    GXKey_RIGHT_SUPER    =    347,
    GXKey_MENU           =    348,
    
    //GXKey_LAST               GLFW_KEY_MENU
    
    
} GXKeyCode;


typedef enum
{
    /*! @brief If this bit is set one or more Shift keys were held down.
     */
    GXKeyMod_SHIFT         =  0x0001,
    /*! @brief If this bit is set one or more Control keys were held down.
     */
    GXKeyMod_CONTROL       =  0x0002,
    /*! @brief If this bit is set one or more Alt keys were held down.
     */
    GXKeyMod_ALT               = 0x0004,
    /*! @brief If this bit is set one or more Super keys were held down.
     */
    GXKeyMod_SUPER             = 0x0008
    
} GXKeyMod;

typedef enum
{
    GXKeyAction_Release  = 0,
    GXKeyAction_Press    = 1,
    GXKeyAction_Repeat   = 2
} GXKeyAction;



#endif /* GXKey_h */
