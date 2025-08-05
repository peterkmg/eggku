#ifndef __MENU_H__
#define __MENU_H__

#define MENU_NUM 10

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define PAGE_SIZE 9

typedef struct state_t state_t;

typedef enum action_t {
  ACTION_SELECT = 0,
  ACTION_BACK,
  ACTION_PREV,
  ACTION_NEXT,
  ACTION_PAGE_PREV,
  ACTION_PAGE_NEXT,
} action_t;

typedef enum menu_state_t {
  MENU_ROOT = 0,
  MENU_APPLICANTS,
  MENU_REGISTER,
  MENU_EDIT,
  MENU_EDIT_DIALOG,
  MENU_EDIT_NAME,
  MENU_EDIT_POEM,
  MENU_UNREGISTER,
  MENU_COMPETITION,
  MENU_REPORT,
} menu_state_t;

typedef struct menu_item_t {
  void (*draw)(state_t* state);
  void (*action)(state_t* state, action_t action);
  int         selected_item;
  int         param;
  const char* title;
} menu_item_t;

typedef struct menu_t {
  menu_item_t menu_list[MENU_NUM];
  int         current_menu;
  char*       notification;
} menu_t;

menu_t menu_initialize(void);
void   menu_header_draw(state_t* state);

menu_item_t menu_root_create(void);
menu_item_t menu_applicants_create(void);
menu_item_t menu_register_create(void);
menu_item_t menu_unregister_create(void);
menu_item_t menu_competition_create(void);
menu_item_t menu_report_create(void);
menu_item_t menu_edit_create(void);
menu_item_t menu_edit_dialog_create(void);
menu_item_t menu_edit_name_create(void);
menu_item_t menu_edit_poem_create(void);

#endif  // __MENU_H__
