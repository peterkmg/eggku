#ifndef __CONTEST_H__
#define __CONTEST_H__

#include "haiku.h"

typedef struct state_t state_t;

#define CONTEST_FILE "contestants.txt"

#define CONTEST_MAX_BUNNIES 64

#define BUNNY_NAME_LENGTH 32

typedef struct bunny_t {
  char name[BUNNY_NAME_LENGTH];
  int  poem_idx;
  int  egg_count;
} bunny_t;

typedef struct contest_t {
  bunny_t bunnies[CONTEST_MAX_BUNNIES];
  int     registered_participants;
} contest_t;

contest_t contest_initialize(void);
int       contest_file_exists(void);
contest_t contest_file_load(void);
void      contest_file_save(state_t* state);
bunny_t   contest_file_save_results(state_t* state);

int contest_add_participant(state_t* state, char* name, int poem_idx);
int contest_exists_participant(state_t* state, char* name);
int contest_remove_participant_by_id(state_t* state, int idx);
int contest_remove_participant_by_name(state_t* state, char* name);
int contest_update_participant_name(state_t* state, int idx, char* name);
int contest_update_participant_poem(state_t* state, int idx, int poem_idx);
int contest_compare_bunnies(const void* left, const void* right);

#endif  // __CONTEST_H__
