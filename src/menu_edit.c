#include "console.h"
#include "menu.h"
#include "state.h"

void menu_edit_draw(state_t* state) {
  menu_item_t* menu = &state->menus.menu_list[state->menus.current_menu];

  int start = menu->param;
  int end   = MIN(start + PAGE_SIZE, state->contest.registered_participants);
  for (int i = start; i < end; ++i) {
    unsigned int style = menu->selected_item == i ? STYLE_FG_GREEN | STYLE_BOLD : STYLE_NONE;
    console_write(style, "%d. %s\n", i + 1, state->contest.bunnies[i].name);
  }

  console_write(STYLE_FAINT | STYLE_FG_CYAN, "\nPage %d of %d\n", (menu->param / PAGE_SIZE) + 1,
                (state->contest.registered_participants + PAGE_SIZE - 1) / PAGE_SIZE);
}

void menu_edit_action(state_t* state, action_t action) {
  menu_item_t* menu = &state->menus.menu_list[state->menus.current_menu];

  switch (action) {
    case ACTION_SELECT:
      state->menus.current_menu = MENU_EDIT_DIALOG;
      menu->param               = 0;
      state->is_changed         = 1;
      return;
    case ACTION_BACK:
      menu->param               = 0;
      menu->selected_item       = 0;
      state->menus.current_menu = MENU_ROOT;
      state->is_changed         = 1;
      return;
    case ACTION_PAGE_PREV:
      if (menu->param == 0) { return; }
      menu->param -= PAGE_SIZE;
      menu->selected_item = menu->param;
      state->is_changed   = 1;
      return;
    case ACTION_PAGE_NEXT:
      if (menu->param + PAGE_SIZE >= state->contest.registered_participants) { return; }
      menu->param += PAGE_SIZE;
      menu->selected_item = menu->param;
      state->is_changed   = 1;
      return;
    case ACTION_PREV:
      if (menu->selected_item == menu->param) { return; }
      menu->selected_item--;
      state->is_changed = 1;
      return;
    case ACTION_NEXT:
      if (menu->selected_item == MIN(menu->param + PAGE_SIZE, state->contest.registered_participants) - 1) { return; }
      menu->selected_item++;
      state->is_changed = 1;
      return;
  }
}

menu_item_t menu_edit_create(void) {
  return (menu_item_t){
      .title  = "Chose bunny to edit:",
      .draw   = menu_edit_draw,
      .action = menu_edit_action,
  };
}
