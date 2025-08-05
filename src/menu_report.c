#include <stdlib.h>
#include <string.h>

#include "console.h"
#include "contest.h"
#include "menu.h"
#include "state.h"

void menu_report_draw(state_t* state) {
  menu_item_t* menu = &state->menus.menu_list[state->menus.current_menu];

  console_write(STYLE_NONE, "Status: ");
  console_write(STYLE_FG_RED, "%s\n\n",
                state->stage == STAGE_COMPETITION_FINISHED ? "Competition finished!" : "Competition in progress!");

  console_write(STYLE_NONE, "Standings:\n");

  bunny_t bunnies[state->contest.registered_participants];
  memcpy(bunnies, state->contest.bunnies, sizeof(bunny_t) * state->contest.registered_participants);
  qsort(bunnies, state->contest.registered_participants, sizeof(bunny_t), contest_compare_bunnies);

  int start = menu->param;
  int end   = MIN(start + PAGE_SIZE, state->contest.registered_participants);
  for (int i = start; i < end; ++i) {
    console_write(STYLE_NONE, "%d. ", i + 1);
    console_write(STYLE_FG_CYAN, "%s", bunnies[i].name);
    console_write(STYLE_FG_MAGENTA, " (Eggs %d)", bunnies[i].egg_count);
    if (i == 0) {
      console_write(STYLE_FG_RED, "%s\n",
                    state->stage == STAGE_COMPETITION_FINISHED ? "\t<--- WINNER" : "\t<--- WINNING");
    } else {
      console_write(STYLE_NONE, "\n");
    }
  }

  console_write(STYLE_FAINT | STYLE_FG_CYAN, "\nPage %d of %d\n", (menu->param / PAGE_SIZE) + 1,
                (state->contest.registered_participants + PAGE_SIZE - 1) / PAGE_SIZE);
}

void menu_report_action(state_t* state, action_t action) {
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

menu_item_t menu_report_create(void) {
  return (menu_item_t){
      .title  = "Competition report",
      .draw   = menu_report_draw,
      .action = menu_report_action,
  };
}
