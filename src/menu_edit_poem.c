#include "console.h"
#include "contest.h"
#include "haiku.h"
#include "menu.h"
#include "state.h"

#define POEM_PAGE_SIZE 3

void menu_edit_poem_draw(state_t* state) {
  menu_item_t* menu = &state->menus.menu_list[state->menus.current_menu];

  int start = menu->param;
  int end   = MIN(start + POEM_PAGE_SIZE, HAIKU_NUM);
  for (int i = start; i < end; ++i) {
    unsigned int style = menu->selected_item == i ? STYLE_FG_GREEN | STYLE_BOLD : STYLE_NONE;
    for (int j = 0; j < HAIKU_LINE_COUNT; ++j) { console_write(style, "\t%s\n", state->haikus[i].lines[j]); }
    console_write(STYLE_NONE, "\n");
  }

  console_write(STYLE_FAINT | STYLE_FG_CYAN, "\nPage %d of %d\n", (menu->param / POEM_PAGE_SIZE) + 1,
                (HAIKU_NUM + POEM_PAGE_SIZE - 1) / POEM_PAGE_SIZE);
}

void menu_edit_poem_action(state_t* state, action_t action) {
  int          bunny_idx = state->menus.menu_list[MENU_EDIT].selected_item;
  menu_item_t* menu      = &state->menus.menu_list[state->menus.current_menu];
  switch (action) {
    case ACTION_SELECT:
      if (contest_update_participant_poem(state, bunny_idx, menu->selected_item)) {
        state->menus.notification = "Poem changed successfully!";
      } else {
        state->menus.notification = "Failed to change poem!";
      }

      menu->selected_item                             = 0;
      menu->param                                     = 0;
      state->menus.menu_list[MENU_EDIT].selected_item = 0;
      state->menus.current_menu                       = MENU_ROOT;
      state->is_changed                               = 1;
      return;
    case ACTION_BACK:
      state->menus.menu_list[MENU_EDIT].selected_item = 0;
      menu->param                                     = 0;
      state->menus.current_menu                       = MENU_ROOT;
      state->is_changed                               = 1;
      return;
    case ACTION_PAGE_PREV:
      if (menu->param == 0) { return; }
      menu->param -= POEM_PAGE_SIZE;
      menu->selected_item = menu->param;
      state->is_changed   = 1;
      return;
    case ACTION_PAGE_NEXT:
      if (menu->param + POEM_PAGE_SIZE >= HAIKU_NUM) { return; }
      menu->param += POEM_PAGE_SIZE;
      menu->selected_item = menu->param;
      state->is_changed   = 1;
      return;
    case ACTION_PREV:
      if (menu->selected_item == menu->param) { return; }
      menu->selected_item--;
      state->is_changed = 1;
      return;
    case ACTION_NEXT:
      if (menu->selected_item == MIN(menu->param + POEM_PAGE_SIZE, HAIKU_NUM) - 1) { return; }
      menu->selected_item++;
      state->is_changed = 1;
      return;
  }
}

menu_item_t menu_edit_poem_create(void) {
  return (menu_item_t){
      .title  = "Select new poem",
      .draw   = menu_edit_poem_draw,
      .action = menu_edit_poem_action,
  };
}
