#include "console.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define SBF_SIZE 1024

static char         screen_buffer[SBF_SIZE] = {0};
static unsigned int offset                  = 0;

typedef struct style_map_t {
  console_style_t style;
  const char*     style_string;
} style_map_t;

static const style_map_t style_map[] = {
    {       STYLE_NONE,  "0"},
    {       STYLE_BOLD,  "1"},
    {      STYLE_FAINT,  "2"},
    {     STYLE_ITALIC,  "3"},
    {  STYLE_UNDERLINE,  "4"},
    { STYLE_SLOW_BLINK,  "5"},
    {STYLE_RAPID_BLINK,  "6"},
    {    STYLE_REVERSE,  "7"},
    {    STYLE_CONCEAL,  "8"},
    {STYLE_CROSSED_OUT,  "9"},

    {     STYLE_FG_RED, "31"},
    {   STYLE_FG_GREEN, "32"},
    {  STYLE_FG_YELLOW, "33"},
    {    STYLE_FG_BLUE, "34"},
    { STYLE_FG_MAGENTA, "35"},
    {    STYLE_FG_CYAN, "36"},
    {   STYLE_FG_WHITE, "37"},
    { STYLE_FG_DEFAULT, "39"},

    {     STYLE_BG_RED, "41"},
    {   STYLE_BG_GREEN, "42"},
    {  STYLE_BG_YELLOW, "43"},
    {    STYLE_BG_BLUE, "44"},
    { STYLE_BG_MAGENTA, "45"},
    {    STYLE_BG_CYAN, "46"},
    {   STYLE_BG_WHITE, "47"},
    { STYLE_BG_DEFAULT, "49"},

    {        STYLE_MAX, NULL},
};

typedef struct termios termios_t;

static termios_t terminal_config_default;
static termios_t terminal_config_raw;

void console_shutdown(void) {
  console_raw_disable();
  console_cursor_show();
  console_reset();
}

void console_initialize(void) {
  tcgetattr(STDIN_FILENO, &terminal_config_default);
  terminal_config_raw = terminal_config_default;
  terminal_config_raw.c_lflag &= ~(ICANON | ECHO);  // disable canonical mode and echo
  console_reset();
}

void console_raw_enable(void) { tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal_config_raw); }
void console_raw_disable(void) { tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal_config_default); }

void console_write(console_style_t style, const char* fmt, ...) {
  int idx         = 0;
  int is_unstyled = (style & style_map[idx++].style) ? 1 : 0;

  if (!is_unstyled) {
    offset += snprintf(&screen_buffer[offset], SBF_SIZE - offset, "\033[");

    unsigned int base_offset = offset;

    while (style_map[idx].style != STYLE_FG_RED) {
      if (style & style_map[idx].style) {
        if (offset > base_offset) { screen_buffer[offset++] = ';'; }
        offset += snprintf(&screen_buffer[offset], SBF_SIZE - offset, "%s", style_map[idx].style_string);
      }
      idx++;
    }

    while (style_map[idx].style != STYLE_BG_RED) {
      if (style & style_map[idx].style) {
        if (offset > base_offset) { screen_buffer[offset++] = ';'; }
        offset += snprintf(&screen_buffer[offset], SBF_SIZE - offset, "%s", style_map[idx].style_string);
        idx++;
        break;
      }
      idx++;
    }

    while (style_map[idx].style != STYLE_MAX) {
      if (style & style_map[idx].style) {
        if (offset > base_offset) { screen_buffer[offset++] = ';'; }
        offset += snprintf(&screen_buffer[offset], SBF_SIZE - offset, "%s", style_map[idx].style_string);
        idx++;
        break;
      }
      idx++;
    }

    screen_buffer[offset++] = 'm';
  }

  va_list args;
  va_start(args, fmt);
  offset += vsnprintf(&screen_buffer[offset], SBF_SIZE - offset, fmt, args);
  if (!is_unstyled) { offset += snprintf(&screen_buffer[offset], SBF_SIZE - offset, "\033[0m"); }
  va_end(args);
}

void console_flush(int hide_cursor) {
  if (hide_cursor) {
    offset += snprintf(&screen_buffer[offset], SBF_SIZE - offset, "\033[?25l");
  } else {
    offset += snprintf(&screen_buffer[offset], SBF_SIZE - offset, "\033[?25h");
  }

  printf("%s", screen_buffer);
  memset(screen_buffer, 0, sizeof(screen_buffer));
  fflush(stdout);
  offset = snprintf(screen_buffer, SBF_SIZE, "\033[2J\033[3J\033[H");
}
