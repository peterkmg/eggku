#ifndef __INPUT_H__
#define __INPUT_H__

typedef enum input_key_t {
  KEY_UP = 0,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,

  KEY_ENTER,
  KEY_ESCAPE,
  KEY_BACKSPACE,

  KEY_W,
  KEY_A,
  KEY_S,
  KEY_D,
  KEY_Q,
  KEY_E,

  KEY_OTHER
} input_key_t;

input_key_t input_get(void);

#endif  // __INPUT_H__
