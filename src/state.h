#ifndef __STATE_H__
#define __STATE_H__

#include "contest.h"
#include "haiku.h"
#include "input.h"
#include "menu.h"

typedef enum stage_t {
  STAGE_INITIAL = 0,
  STAGE_REGISTRATION,
  STAGE_COMPETITION_READY,
  STAGE_COMPETITION_IN_PROGRESS,
  STAGE_COMPETITION_FINISHED,
} stage_t;

typedef struct state_t {
  haiku_t*  haikus;
  stage_t   stage;
  menu_t    menus;
  contest_t contest;
  int       is_running;
  int       is_changed;
} state_t;

state_t state_initialize(void);
void    state_update(state_t* state);
void    state_draw(state_t* state);
void    state_destroy(state_t* state);

#endif  // __STATE_H__
