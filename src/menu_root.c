#include "console.h"
#include "menu.h"
#include "state.h"

#define MENU_ROOT_ITEM_NUM 7

static const char* menu_root_items[MENU_ROOT_ITEM_NUM] = {
    "1. Show registered bunnies", "2. Register bunny", "3. Edit bunny info", "4. Unregister bunny", "5. To competition",
    "6. Show standings",          "7. Exit",
};

void menu_root_draw(state_t* state) {
  menu_item_t* menu = &state->menus.menu_list[state->menus.current_menu];

  for (int i = 0; i < MENU_ROOT_ITEM_NUM; ++i) {
    unsigned int style = menu->selected_item == i ? STYLE_FG_GREEN | STYLE_BOLD : STYLE_NONE;
    console_write(style, "%s\n", menu_root_items[i]);
  }
}

void menu_root_action(state_t* state, action_t action) {
  menu_item_t* menu = &state->menus.menu_list[state->menus.current_menu];
  switch (action) {
    case ACTION_SELECT:
      switch (menu->selected_item) {
        case 0:
          if (state->stage == STAGE_INITIAL) {
            state->menus.notification = "No bunnies registered yet!";
            state->is_changed         = 1;
            return;
          }

          menu->selected_item       = 0;
          state->menus.current_menu = MENU_APPLICANTS;
          state->is_changed         = 1;
          return;
        case 1:
          if (state->stage > STAGE_COMPETITION_READY) {
            state->menus.notification = "You cannot register more bunnies after competition started!";
            state->is_changed         = 1;
            return;
          }

          menu->selected_item       = 0;
          state->menus.current_menu = MENU_REGISTER;
          state->is_changed         = 1;
          return;
        case 2:
          if (state->stage == STAGE_INITIAL) {
            state->menus.notification = "No bunnies registered yet!";
            state->is_changed         = 1;
            return;
          } else if (state->stage > STAGE_COMPETITION_READY) {
            state->menus.notification = "You cannot add more bunnies after competition started!";
            state->is_changed         = 1;
            return;
          }

          menu->selected_item       = 0;
          state->menus.current_menu = MENU_EDIT;
          state->is_changed         = 1;
          return;
        case 3:
          if (state->stage > STAGE_COMPETITION_READY) {
            state->menus.notification = "You cannot unregister bunnies after competition started!";
            state->is_changed         = 1;
            return;
          } else if (state->stage == STAGE_INITIAL) {
            state->menus.notification = "No bunnies registered yet!";
            state->is_changed         = 1;
            return;
          }

          menu->selected_item       = 0;
          state->menus.current_menu = MENU_UNREGISTER;
          state->is_changed         = 1;
          return;
        case 4:
          if (state->stage == STAGE_INITIAL) {
            state->menus.notification = "No bunnies registered yet!";
            state->is_changed         = 1;
            return;
          } else if (state->stage == STAGE_COMPETITION_FINISHED) {
            state->menus.notification = "Competition already finished!";
            state->is_changed         = 1;
            return;
          } else if (state->stage == STAGE_REGISTRATION) {
            state->menus.notification = "At least 2 bunnies need to be registered to start competition!";
            state->is_changed         = 1;
            return;
          }

          menu->selected_item       = 0;
          state->menus.current_menu = MENU_COMPETITION;
          state->stage              = STAGE_COMPETITION_IN_PROGRESS;
          state->is_changed         = 1;
          return;
        case 5:
          if (state->stage < STAGE_COMPETITION_IN_PROGRESS) {
            state->menus.notification = "You need to start competition first!";
            state->is_changed         = 1;
            return;
          }

          menu->selected_item       = 0;
          state->menus.current_menu = MENU_REPORT;
          state->is_changed         = 1;
          return;
        case 6 : state->is_running = 0; return;
        default: return;
      }
    case ACTION_BACK: state->is_running = 0; return;
    case ACTION_PREV:
      if (menu->selected_item == 0) { return; }
      menu->selected_item--;
      state->is_changed = 1;
      return;
    case ACTION_NEXT:
      if (!(menu->selected_item < MENU_ROOT_ITEM_NUM - 1)) { return; }
      menu->selected_item++;
      state->is_changed = 1;
      return;
    case ACTION_PAGE_PREV:
    case ACTION_PAGE_NEXT: return;
  }
}

menu_item_t menu_root_create(void) {
  return (menu_item_t){
      .title  = "Choose an option:",
      .draw   = menu_root_draw,
      .action = menu_root_action,
  };
}
