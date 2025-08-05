#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "console.h"
#include "contest.h"
#include "menu.h"
#include "state.h"

void menu_edit_name_draw(state_t* state) {
  int bunny_idx = state->menus.menu_list[MENU_EDIT].selected_item;

  console_write(STYLE_NONE, "Enter new name for");
  console_write(STYLE_FG_CYAN, " %s", state->contest.bunnies[bunny_idx].name);
  console_write(STYLE_NONE, ": ");
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

  state->is_changed = 1;
  if (contest_update_participant_name(state, bunny_idx, name)) {
    state->menus.notification = "Bunny renamed successfully!";
  } else {
    state->menus.notification = "Bunny with this name already exists!";
  }

  state->menus.current_menu                       = MENU_ROOT;
  state->menus.menu_list[MENU_EDIT].selected_item = 0;
}

menu_item_t menu_edit_name_create(void) {
  return (menu_item_t){
      .title = "Rename bunny",
      .draw  = menu_edit_name_draw,
  };
}
