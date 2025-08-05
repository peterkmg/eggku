#include "state.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "console.h"
#include "haiku.h"
#include "input.h"
#include "menu.h"

state_t state_initialize(void) {
  console_initialize();
  console_raw_enable();

  srand(time(NULL));

  haiku_t* haikus = haiku_load_from_file();

  return (state_t){
      .haikus     = haikus,
      .is_running = 1,
      .is_changed = 1,
      .stage      = STAGE_REGISTRATION,
      .menus      = menu_initialize(),
      .contest    = contest_initialize(),
  };
}

void state_update(state_t* state) {
  input_key_t key = input_get();

  if (state->menus.notification != NULL) {
    state->menus.notification = NULL;
    state->is_changed         = 1;
  }

  menu_item_t* menu = &state->menus.menu_list[state->menus.current_menu];

  switch (key) {
    case KEY_W:
    case KEY_UP: menu->action(state, ACTION_PREV); break;

    case KEY_S:
    case KEY_DOWN: menu->action(state, ACTION_NEXT); break;

    case KEY_Q:
    case KEY_ESCAPE:
    case KEY_BACKSPACE: menu->action(state, ACTION_BACK); break;

    case KEY_E:
    case KEY_ENTER: menu->action(state, ACTION_SELECT); break;

    case KEY_A:
    case KEY_LEFT: menu->action(state, ACTION_PAGE_PREV); break;

    case KEY_D:
    case KEY_RIGHT: menu->action(state, ACTION_PAGE_NEXT); break;
    default       : break;
  }
}

void state_draw(state_t* state) {
  state->is_changed = 0;
  menu_header_draw(state);
  state->menus.menu_list[state->menus.current_menu].draw(state);
  console_flush(1);
}

void state_destroy(state_t* state) {
  console_shutdown();
  free(state->haikus);
  state->haikus = NULL;
}
