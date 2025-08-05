#include "menu.h"

#include <stdio.h>

#include "console.h"
#include "state.h"

void menu_header_draw(state_t* state) {
  console_write(STYLE_BOLD | STYLE_FG_MAGENTA | STYLE_SLOW_BLINK, "Welcome to Easter Bunny King election!\n");
  console_write(STYLE_FAINT | STYLE_BOLD, "Use 'WASD' or arrow keys to navigate.\n");
  console_write(STYLE_FAINT | STYLE_BOLD, "'ENTER' or 'E' to select, 'Esc' or 'Q' to go back or exit.\n\n");
  console_write(STYLE_FG_YELLOW, "%s\n\n", state->menus.notification != NULL ? state->menus.notification : "");
  console_write(STYLE_ITALIC | STYLE_UNDERLINE, "%s\n\n", state->menus.menu_list[state->menus.current_menu].title);
}

menu_t menu_initialize(void) {
  return (menu_t){
      {
       [MENU_ROOT]        = menu_root_create(),
       [MENU_APPLICANTS]  = menu_applicants_create(),
       [MENU_REGISTER]    = menu_register_create(),
       [MENU_EDIT]        = menu_edit_create(),
       [MENU_EDIT_DIALOG] = menu_edit_dialog_create(),
       [MENU_EDIT_NAME]   = menu_edit_name_create(),
       [MENU_EDIT_POEM]   = menu_edit_poem_create(),
       [MENU_UNREGISTER]  = menu_unregister_create(),
       [MENU_COMPETITION] = menu_competition_create(),
       [MENU_REPORT]      = menu_report_create(),
       },
      0,
      NULL,
  };
}
