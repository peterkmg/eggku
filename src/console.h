#ifndef __CONSOLE_H__
#define __CONSOLE_H__

typedef enum console_style_t {
  STYLE_NONE        = 0U,
  STYLE_BOLD        = 1 << 0,
  STYLE_FAINT       = 1 << 1,
  STYLE_ITALIC      = 1 << 2,
  STYLE_UNDERLINE   = 1 << 3,
  STYLE_SLOW_BLINK  = 1 << 4,
  STYLE_RAPID_BLINK = 1 << 5,
  STYLE_REVERSE     = 1 << 6,
  STYLE_CONCEAL     = 1 << 7,
  STYLE_CROSSED_OUT = 1 << 8,

  STYLE_FG_RED     = 1 << 9,
  STYLE_FG_GREEN   = 1 << 10,
  STYLE_FG_YELLOW  = 1 << 11,
  STYLE_FG_BLUE    = 1 << 12,
  STYLE_FG_MAGENTA = 1 << 13,
  STYLE_FG_CYAN    = 1 << 14,
  STYLE_FG_WHITE   = 1 << 15,
  STYLE_FG_DEFAULT = 1 << 16,

  STYLE_BG_RED     = 1 << 17,
  STYLE_BG_GREEN   = 1 << 18,
  STYLE_BG_YELLOW  = 1 << 19,
  STYLE_BG_BLUE    = 1 << 20,
  STYLE_BG_MAGENTA = 1 << 21,
  STYLE_BG_CYAN    = 1 << 22,
  STYLE_BG_WHITE   = 1 << 23,
  STYLE_BG_DEFAULT = 1 << 24,

  STYLE_MAX = 1 << 25
} console_style_t;

void console_initialize(void);
void console_shutdown(void);
void console_raw_enable(void);
void console_raw_disable(void);

void console_write(console_style_t style, const char* fmt, ...);
void console_flush(int hide_cursor);

#define console_reset()       printf("\033c");
#define console_cursor_hide() printf("\033[?25l");
#define console_cursor_show() printf("\033[?25h");

#endif  // __CONSOLE_H__
