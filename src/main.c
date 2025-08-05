#include "state.h"

int main(void) {
  state_t state = state_initialize();

  if (state.contest.registered_participants > 0) {
    state.stage = state.contest.registered_participants > 1 ? STAGE_COMPETITION_READY : STAGE_REGISTRATION;
  }

  while (state.is_running) {
    while (state.is_changed) { state_draw(&state); }
    state_update(&state);
  }

  state_destroy(&state);
  return 0;
}
