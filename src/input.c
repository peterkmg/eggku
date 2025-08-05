#include "input.h"

#include <unistd.h>

input_key_t input_get(void) {
  char seq[3];
  read(STDIN_FILENO, seq, 3);

  switch (seq[0]) {
    case 87:
    case 119: return KEY_W;
    case 65 :
    case 97 : return KEY_A;
    case 83 :
    case 115: return KEY_S;
    case 68 :
    case 100: return KEY_D;
    case 81 :
    case 113: return KEY_Q;
    case 69 :
    case 101: return KEY_E;

    case 127: return KEY_BACKSPACE;
    case 10 : return KEY_ENTER;

    case 27:  // escape sequence
      switch (seq[2]) {
        case 0: return KEY_ESCAPE;

        case 65: return KEY_UP;
        case 66: return KEY_DOWN;
        case 67: return KEY_RIGHT;
        case 68: return KEY_LEFT;

        default: return KEY_OTHER;
      }

    default: return KEY_OTHER;
  }
}
