#include "contest.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"

contest_t contest_initialize(void) { return contest_file_exists() ? contest_file_load() : (contest_t){0}; }

contest_t contest_file_load(void) {
  FILE* file = fopen(CONTEST_FILE, "r");

  if (file == NULL) {
    perror("Failed to open contest file");
    exit(EXIT_FAILURE);
  }

  int       cnt     = 0;
  contest_t contest = {0};

  while (fscanf(file, "%31[^,],%d\n", contest.bunnies[cnt].name, &contest.bunnies[cnt].poem_idx) == 2) { cnt++; }

  if (cnt > CONTEST_MAX_BUNNIES) {
    perror("Too many participants in contest file");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  contest.registered_participants = cnt;

  fclose(file);

  return contest;
}

void contest_file_save(state_t* state) {
  FILE* file = fopen(CONTEST_FILE, "w");

  if (file == NULL) {
    perror("Failed to open contest file");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < state->contest.registered_participants; ++i) {
    fprintf(file, "%s,%d\n", state->contest.bunnies[i].name, state->contest.bunnies[i].poem_idx);
  }

  fclose(file);
}

int contest_file_exists(void) {
  FILE* file   = fopen(CONTEST_FILE, "r");
  int   exists = (file != NULL);
  if (exists) { fclose(file); }
  return exists;
}

int contest_exists_participant(state_t* state, char* name) {
  for (int i = 0; i < state->contest.registered_participants; ++i) {
    if (strcmp(state->contest.bunnies[i].name, name) == 0) { return 1; }
  }
  return 0;
}

int contest_add_participant(state_t* state, char* name, int poem_idx) {
  if (contest_exists_participant(state, name)) { return 0; }

  strcpy(state->contest.bunnies[state->contest.registered_participants].name, name);
  state->contest.bunnies[state->contest.registered_participants].poem_idx  = poem_idx;
  state->contest.bunnies[state->contest.registered_participants].egg_count = 0;
  state->contest.registered_participants++;
  contest_file_save(state);
  return 1;
}

int contest_update_participant_name(state_t* state, int idx, char* name) {
  if (contest_exists_participant(state, name)) { return 0; }

  strcpy(state->contest.bunnies[idx].name, name);
  contest_file_save(state);
  return 1;
}

int contest_update_participant_poem(state_t* state, int idx, int poem_idx) {
  if (poem_idx < 0 || poem_idx >= HAIKU_NUM) { return 0; }
  state->contest.bunnies[idx].poem_idx = poem_idx;
  contest_file_save(state);
  return 1;
}

int contest_remove_participant_by_id(state_t* state, int idx) {
  if (idx != state->contest.registered_participants - 1) {
    memcpy(&state->contest.bunnies[idx], &state->contest.bunnies[idx + 1],
           (state->contest.registered_participants - idx - 1) * sizeof(bunny_t));
  }

  state->contest.bunnies[state->contest.registered_participants-- - 1] = (bunny_t){0};
  contest_file_save(state);
  return 1;
}

int contest_remove_participant_by_name(state_t* state, char* name) {
  for (int i = 0; i < state->contest.registered_participants; ++i) {
    if (strcmp(state->contest.bunnies[i].name, name) == 0) { contest_remove_participant_by_id(state, i); }
  }

  return 0;
}

int contest_compare_bunnies(const void* left, const void* right) {
  return ((bunny_t*)right)->egg_count - ((bunny_t*)left)->egg_count;
}

bunny_t contest_file_save_results(state_t* state) {
  bunny_t bunnies[state->contest.registered_participants];
  memcpy(bunnies, state->contest.bunnies, sizeof(bunny_t) * state->contest.registered_participants);
  qsort(bunnies, state->contest.registered_participants, sizeof(bunny_t), contest_compare_bunnies);

  FILE* file = fopen("results.txt", "w");

  for (int i = 0; i < state->contest.registered_participants; i++) {
    fprintf(file, "%s,%d\n", bunnies[i].name, bunnies[i].egg_count);
  }

  fclose(file);
  return bunnies[0];
}
