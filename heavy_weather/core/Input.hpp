#pragma once
#include <GLFW/glfw3.h>

// The only goal of this is to not include glfw everywhere i need input.

namespace weather {

#define HW_KEY_UNKNOWN -1
/* Printable keys */
#define HW_KEY_SPACE              GLFW_KEY_SPACE
#define HW_KEY_APOSTROPHE         GLFW_KEY_APOSTROPHE  /* ' */
#define HW_KEY_COMMA              GLFW_KEY_COMMA  /* , */
#define HW_KEY_MINUS              GLFW_KEY_MINUS  /* - */
#define HW_KEY_PERIOD             GLFW_KEY_PERIOD  /* . */
#define HW_KEY_SLASH              GLFW_KEY_SLASH  /* / */
#define HW_KEY_0                  GLFW_KEY_0
#define HW_KEY_1                  GLFW_KEY_1
#define HW_KEY_2                  GLFW_KEY_2
#define HW_KEY_3                  GLFW_KEY_3
#define HW_KEY_4                  GLFW_KEY_4
#define HW_KEY_5                  GLFW_KEY_5
#define HW_KEY_6                  GLFW_KEY_6
#define HW_KEY_7                  GLFW_KEY_7
#define HW_KEY_8                  GLFW_KEY_8
#define HW_KEY_9                  GLFW_KEY_9
#define HW_KEY_SEMICOLON          GLFW_KEY_SEMICOLON  /* ; */
#define HW_KEY_EQUAL              GLFW_KEY_EQUAL  /* = */
#define HW_KEY_A                  GLFW_KEY_A
#define HW_KEY_B                  GLFW_KEY_B
#define HW_KEY_C                  GLFW_KEY_C
#define HW_KEY_D                  GLFW_KEY_D
#define HW_KEY_E                  GLFW_KEY_E
#define HW_KEY_F                  GLFW_KEY_F
#define HW_KEY_G                  GLFW_KEY_G
#define HW_KEY_H                  GLFW_KEY_H
#define HW_KEY_I                  GLFW_KEY_I
#define HW_KEY_J                  GLFW_KEY_J
#define HW_KEY_K                  GLFW_KEY_K
#define HW_KEY_L                  GLFW_KEY_L
#define HW_KEY_M                  GLFW_KEY_M
#define HW_KEY_N                  GLFW_KEY_N
#define HW_KEY_O                  GLFW_KEY_O
#define HW_KEY_P                  GLFW_KEY_P
#define HW_KEY_Q                  GLFW_KEY_Q
#define HW_KEY_R                  GLFW_KEY_R
#define HW_KEY_S                  GLFW_KEY_S
#define HW_KEY_T                  GLFW_KEY_T
#define HW_KEY_U                  GLFW_KEY_U
#define HW_KEY_V                  GLFW_KEY_V
#define HW_KEY_W                  GLFW_KEY_W
#define HW_KEY_X                  GLFW_KEY_X
#define HW_KEY_Y                  GLFW_KEY_Y
#define HW_KEY_Z                  GLFW_KEY_Z
#define HW_KEY_LEFT_BRACKET       GLFW_KEY_LEFT_BRACKET  /* [ */
#define HW_KEY_BACKSLASH          GLFW_KEY_BACKSLASH  /* \ */
#define HW_KEY_RIGHT_BRACKET      GLFW_KEY_RIGHT_BRACKET  /* ] */
#define HW_KEY_GRAVE_ACCENT       GLFW_KEY_GRAVE_ACCENT  /* ` */
#define HW_KEY_WORLD_1            GLFW_KEY_WORLD_1 /* non-US #1 */
#define HW_KEY_WORLD_2            GLFW_KEY_WORLD_2 /* non-US #2 */

/* Function keys */
#define HW_KEY_ESCAPE             GLFW_KEY_ESCAPE
#define HW_KEY_ENTER              GLFW_KEY_ENTER
#define HW_KEY_TAB                GLFW_KEY_TAB
#define HW_KEY_BACKSPACE          GLFW_KEY_BACKSPACE
#define HW_KEY_INSERT             GLFW_KEY_INSERT
#define HW_KEY_DELETE             GLFW_KEY_DELETE
#define HW_KEY_RIGHT              GLFW_KEY_RIGHT
#define HW_KEY_LEFT               GLFW_KEY_LEFT
#define HW_KEY_DOWN               GLFW_KEY_DOWN
#define HW_KEY_UP                 GLFW_KEY_UP
#define HW_KEY_PAGE_UP            GLFW_KEY_PAGE_UP
#define HW_KEY_PAGE_DOWN          GLFW_KEY_PAGE_DOWN
#define HW_KEY_HOME               GLFW_KEY_HOME
#define HW_KEY_END                GLFW_KEY_END
#define HW_KEY_CAPS_LOCK          GLFW_KEY_CAPS_LOCK
#define HW_KEY_SCROLL_LOCK        GLFW_KEY_SCROLL_LOCK
#define HW_KEY_NUM_LOCK           GLFW_KEY_NUM_LOCK
#define HW_KEY_PRINT_SCREEN       GLFW_KEY_PRINT_SCREEN
#define HW_KEY_PAUSE              GLFW_KEY_PAUSE
#define HW_KEY_F1                 GLFW_KEY_F1
#define HW_KEY_F2                 GLFW_KEY_F2
#define HW_KEY_F3                 GLFW_KEY_F3
#define HW_KEY_F4                 GLFW_KEY_F4
#define HW_KEY_F5                 GLFW_KEY_F5
#define HW_KEY_F6                 GLFW_KEY_F6
#define HW_KEY_F7                 GLFW_KEY_F7
#define HW_KEY_F8                 GLFW_KEY_F8
#define HW_KEY_F9                 GLFW_KEY_F9
#define HW_KEY_F10                GLFW_KEY_F10
#define HW_KEY_F11                GLFW_KEY_F11
#define HW_KEY_F12                GLFW_KEY_F12
#define HW_KEY_F13                GLFW_KEY_F13
#define HW_KEY_F14                GLFW_KEY_F14
#define HW_KEY_F15                GLFW_KEY_F15
#define HW_KEY_F16                GLFW_KEY_F16
#define HW_KEY_F17                GLFW_KEY_F17
#define HW_KEY_F18                GLFW_KEY_F18
#define HW_KEY_F19                GLFW_KEY_F19
#define HW_KEY_F20                GLFW_KEY_F20
#define HW_KEY_F21                GLFW_KEY_F21
#define HW_KEY_F22                GLFW_KEY_F22
#define HW_KEY_F23                GLFW_KEY_F23
#define HW_KEY_F24                GLFW_KEY_F24
#define HW_KEY_F25                GLFW_KEY_F25
#define HW_KEY_KP_0               GLFW_KEY_KP_0
#define HW_KEY_KP_1               GLFW_KEY_KP_1
#define HW_KEY_KP_2               GLFW_KEY_KP_2
#define HW_KEY_KP_3               GLFW_KEY_KP_3
#define HW_KEY_KP_4               GLFW_KEY_KP_4
#define HW_KEY_KP_5               GLFW_KEY_KP_5
#define HW_KEY_KP_6               GLFW_KEY_KP_6
#define HW_KEY_KP_7               GLFW_KEY_KP_7
#define HW_KEY_KP_8               GLFW_KEY_KP_8
#define HW_KEY_KP_9               GLFW_KEY_KP_9
#define HW_KEY_KP_DECIMAL         GLFW_KEY_KP_DECIMAL
#define HW_KEY_KP_DIVIDE          GLFW_KEY_KP_DIVIDE
#define HW_KEY_KP_MULTIPLY        GLFW_KEY_KP_MULTIPLY
#define HW_KEY_KP_SUBTRACT        GLFW_KEY_KP_SUBTRACT
#define HW_KEY_KP_ADD             GLFW_KEY_KP_ADD
#define HW_KEY_KP_ENTER           GLFW_KEY_KP_ENTER
#define HW_KEY_KP_EQUAL           GLFW_KEY_KP_EQUAL
#define HW_KEY_LEFT_SHIFT         GLFW_KEY_LEFT_SHIFT
#define HW_KEY_LEFT_CONTROL       GLFW_KEY_LEFT_CONTROL
#define HW_KEY_LEFT_ALT           GLFW_KEY_LEFT_ALT
#define HW_KEY_LEFT_SUPER         GLFW_KEY_LEFT_SUPER
#define HW_KEY_RIGHT_SHIFT        GLFW_KEY_RIGHT_SHIFT
#define HW_KEY_RIGHT_CONTROL      GLFW_KEY_RIGHT_CONTROL
#define HW_KEY_RIGHT_ALT          GLFW_KEY_RIGHT_ALT
#define HW_KEY_RIGHT_SUPER        GLFW_KEY_RIGHT_SUPER
#define HW_KEY_MENU               GLFW_KEY_MENU

#define HW_KEY_LAST               GLFW_KEY_LAST

/* Bit flags for mod keys: */
#define HW_MOD_SHIFT           GLFW_MOD_SHIFT
#define HW_MOD_CONTROL         GLFW_MOD_CONTROL
#define HW_MOD_ALT             GLFW_MOD_ALT
#define HW_MOD_SUPER           GLFW_MOD_SUPER
#define HW_MOD_CAPS_LOCK       GLFW_MOD_CAPS_LOCK
#define HW_MOD_NUM_LOCK        GLFW_MOD_NUM_LOCK

/* Mouse: */
#define HW_MOUSE_BUTTON_1         GLFW_MOUSE_BUTTON_1
#define HW_MOUSE_BUTTON_2         GLFW_MOUSE_BUTTON_2
#define HW_MOUSE_BUTTON_3         GLFW_MOUSE_BUTTON_3
#define HW_MOUSE_BUTTON_4         GLFW_MOUSE_BUTTON_4
#define HW_MOUSE_BUTTON_5         GLFW_MOUSE_BUTTON_5
#define HW_MOUSE_BUTTON_6         GLFW_MOUSE_BUTTON_6
#define HW_MOUSE_BUTTON_7         GLFW_MOUSE_BUTTON_7
#define HW_MOUSE_BUTTON_8         GLFW_MOUSE_BUTTON_8
#define HW_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_LAST
#define HW_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_LEFT
#define HW_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_RIGHT
#define HW_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_MIDDLE


// Store input state for current frame. Can be queried by other systems
class Input {
public:
  Input();
  bool isKeyDown(int key);

private:
  // Huge array for all input types, since they all have unique GLFW defines
  int keys[HW_KEY_LAST]; 
};

}
