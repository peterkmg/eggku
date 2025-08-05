#include "console.h"
#include "menu.h"
#include "state.h"

void menu_applicants_draw(state_t* state) {
  menu_item_t* menu = &state->menus.menu_list[state->menus.current_menu];

  int start = menu->param;
  int end   = MIN(start + PAGE_SIZE, state->contest.registered_participants);
  for (int i = start; i < end; ++i) {
    console_write(STYLE_NONE, "%d. ", i + 1);
    console_write(STYLE_FG_CYAN, "%s", state->contest.bunnies[i].name);
    console_write(STYLE_FG_MAGENTA, " (Poem %d)\n", state->contest.bunnies[i].poem_idx);
  }

  console_write(STYLE_FAINT | STYLE_FG_CYAN, "\nPage %d of %d\n", (menu->param / PAGE_SIZE) + 1,
                (state->contest.registered_participants + PAGE_SIZE - 1) / PAGE_SIZE);
}

void menu_applicants_action(state_t* state, action_t action) {
  menu_item_t* menu = &state->menus.menu_list[state->menus.current_menu];

  switch (action) {
    case ACTION_SELECT:
    case ACTION_BACK:
      menu->param               = 0;
      state->menus.current_menu = MENU_ROOT;
      state->is_changed         = 1;
      return;
    case ACTION_PAGE_PREV:
      if (menu->param == 0) { return; }
      menu->param -= PAGE_SIZE;
      state->is_changed = 1;
      return;
    case ACTION_PAGE_NEXT:
      if (menu->param + PAGE_SIZE >= state->contest.registered_participants) { return; }
      menu->param += PAGE_SIZE;
      state->is_changed = 1;
      return;
    case ACTION_PREV:
    case ACTION_NEXT: return;
  }
}

menu_item_t menu_applicants_create(void) {
  return (menu_item_t){
      .title  = "List of contestants",
      .draw   = menu_applicants_draw,
      .action = menu_applicants_action,
  };
}
