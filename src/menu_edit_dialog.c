#include "console.h"
#include "contest.h"
#include "menu.h"
#include "state.h"

#define MENU_EDIT_DIALOG_ITEM_NUM 3

static const char* menu_edit_dialog_items[MENU_EDIT_DIALOG_ITEM_NUM] = {"1. Edit name", "2. Edit poem",
                                                                        "3. Back to main menu"};

void menu_edit_dialog_draw(state_t* state) {
  int bunny_idx = state->menus.menu_list[MENU_EDIT].selected_item;

  menu_item_t* menu = &state->menus.menu_list[state->menus.current_menu];

  console_write(STYLE_NONE, "Selected bunny: ");
  console_write(STYLE_FG_CYAN, "%s\n\n", state->contest.bunnies[bunny_idx].name);

  for (int i = 0; i < MENU_EDIT_DIALOG_ITEM_NUM; ++i) {
    unsigned int style = menu->selected_item == i ? STYLE_FG_GREEN | STYLE_BOLD : STYLE_NONE;
    console_write(style, "%s\n", menu_edit_dialog_items[i]);
  }
}

void menu_edit_dialog_action(state_t* state, action_t action) {
  menu_item_t* menu = &state->menus.menu_list[state->menus.current_menu];
  switch (action) {
    case ACTION_SELECT:
      switch (menu->selected_item) {
        case 0:
          state->menus.current_menu = MENU_EDIT_NAME;
          menu->selected_item       = 0;
          state->is_changed         = 1;
          return;
        case 1:
          state->menus.current_menu = MENU_EDIT_POEM;
          menu->selected_item       = 0;
          state->is_changed         = 1;
          return;
        case 2:
          state->menus.menu_list[MENU_EDIT].selected_item = 0;
          menu->selected_item                             = 0;
          state->menus.current_menu                       = MENU_ROOT;
          state->is_changed                               = 1;
          return;
        default: return;
      }
      return;
    case ACTION_BACK:
      state->menus.menu_list[MENU_EDIT].selected_item = 0;
      menu->selected_item                             = 0;
      state->menus.current_menu                       = MENU_ROOT;
      return;
    case ACTION_PREV:
      if (menu->selected_item == 0) { return; }
      menu->selected_item--;
      state->is_changed = 1;
      return;
    case ACTION_NEXT:
      if (!(menu->selected_item < MENU_EDIT_DIALOG_ITEM_NUM - 1)) { return; }
      menu->selected_item++;
      state->is_changed = 1;
      return;
    case ACTION_PAGE_PREV:
    case ACTION_PAGE_NEXT: return;
  }
}

menu_item_t menu_edit_dialog_create(void) {
  return (menu_item_t){
      .title  = "Edit bunny info",
      .draw   = menu_edit_dialog_draw,
      .action = menu_edit_dialog_action,
  };
}
