#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "console.h"
#include "contest.h"
#include "menu.h"
#include "state.h"

void menu_register_draw(state_t* state) {
  console_write(STYLE_NONE, "Enter bunny name (max. 31 character): ");
  console_flush(0);

  char name[32] = {0};
  console_raw_disable();
  fgets(name, sizeof(name), stdin);
  console_raw_enable();
  name[strcspn(name, "\n")] = '\0';

  if (strlen(name) == 0) {
    state->menus.notification = "Bunny name cannot be empty!";
    state->is_changed         = 1;
    return;
  }

  if (strchr(name, ',') != NULL) {
    state->menus.notification = "Bunny name cannot contain comma!";
    state->is_changed         = 1;
    return;
  }

  int poem_idx = rand() % HAIKU_NUM;

  state->is_changed = 1;
  if (contest_add_participant(state, name, poem_idx)) {
    state->menus.notification = "Bunny registered successfully!";
    state->menus.current_menu = MENU_ROOT;

    if (state->stage < STAGE_COMPETITION_READY) {
      state->stage = state->contest.registered_participants > 1 ? STAGE_COMPETITION_READY : STAGE_REGISTRATION;
    }
  } else {
    state->menus.notification = "Bunny with this name is already registered!";
  }
}

menu_item_t menu_register_create(void) {
  return (menu_item_t){
      .title = "Register contestant",
      .draw  = menu_register_draw,
  };
}
