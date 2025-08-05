#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "console.h"
#include "haiku.h"
#include "menu.h"
#include "state.h"

#define EGG_COUNT_MIN 1
#define EGG_COUNT_MAX 20

typedef struct sigaction sigaction_t;

typedef struct shared_params {
  int      initialized;
  int      signal_received;
  int      child_pid;
  int      pipe_fd[2];
  sigset_t sigusr_blocked_mask;
  sigset_t sigusr_unblocked_mask;
  int*     eggs;
} shared_params_t;

static shared_params_t params          = {0};
static char            winner_str[128] = {0};

void sig_handle(int sig) {
  if (sig == SIGUSR1) { params.signal_received = 1; }
}

void create_fork(void) {
  sigaction_t sigact;
  sigact.sa_handler = sig_handle;
  sigact.sa_flags   = 0;
  sigemptyset(&sigact.sa_mask);
  if (sigaction(SIGUSR1, &sigact, NULL) == -1) {
    perror("sigaction error");
    exit(EXIT_FAILURE);
  }

  sigemptyset(&params.sigusr_blocked_mask);
  sigaddset(&params.sigusr_blocked_mask, SIGUSR1);
  if (sigprocmask(SIG_BLOCK, &params.sigusr_blocked_mask, &params.sigusr_unblocked_mask) == -1) {
    perror("sigprocmask error");
    exit(EXIT_FAILURE);
  }

  pipe(params.pipe_fd);

  params.child_pid = fork();
}

void menu_competition_draw(state_t* state) {
  if (!params.initialized) {
    create_fork();
    params.initialized = 1;

    if (params.child_pid != 0) {
      sigsuspend(&params.sigusr_unblocked_mask);

      close(params.pipe_fd[1]);
      params.eggs = calloc(state->contest.registered_participants, sizeof(int));

      read(params.pipe_fd[0], params.eggs, sizeof(int) * state->contest.registered_participants);
      close(params.pipe_fd[0]);

      for (int i = 0; i < state->contest.registered_participants; i++) {
        state->contest.bunnies[i].egg_count = params.eggs[i];
      }

      bunny_t bunny = contest_file_save_results(state);

      sprintf(winner_str, "Easter Bunny King is %s with %d eggs!", bunny.name, bunny.egg_count);

      free(params.eggs);
      state->menus.current_menu = MENU_ROOT;
      state->stage              = STAGE_COMPETITION_FINISHED;
      state->menus.notification = winner_str;
      state->is_changed         = 1;
      return;
    }

    params.eggs = calloc(state->contest.registered_participants, sizeof(int));
    close(params.pipe_fd[0]);
    kill(getppid(), SIGUSR1);
  }

  menu_item_t* menu = &state->menus.menu_list[state->menus.current_menu];

  console_write(STYLE_NONE, "Bunny #%d ", menu->param + 1);
  console_write(STYLE_BOLD | STYLE_FG_CYAN, "%s", state->contest.bunnies[menu->param].name);
  console_write(STYLE_NONE, " is reading his poem:\n\n");

  haiku_line_t* haiku = state->haikus[state->contest.bunnies[menu->param].poem_idx].lines;

  // line 1
  console_write(STYLE_NONE, "%-12s", "(\\(\\");
  console_write(STYLE_FG_MAGENTA, "%-40s", haiku[0]);
  console_write(STYLE_NONE, "%-5s", " /)/)\n");

  // line 2
  console_write(STYLE_NONE, "%-12s", "( -.-)");
  console_write(STYLE_FG_MAGENTA, "%-40s", haiku[1]);
  console_write(STYLE_NONE, "%-5s", "(•.• )\n");

  // line 3
  console_write(STYLE_FG_CYAN, "%s", "o_");
  console_write(STYLE_NONE, "%-10s", "(\")(\")");
  console_write(STYLE_FG_MAGENTA, "%-40s", haiku[2]);
  console_write(STYLE_NONE, "%-5s", "(\")_(\")");
  console_write(STYLE_FG_RED, "❤\n");

  // reward
  params.eggs[menu->param] = (rand() % EGG_COUNT_MAX) + EGG_COUNT_MIN;

  console_write(STYLE_NONE, "\nAnd gets rewarded with: ");
  console_write(STYLE_BOLD | STYLE_RAPID_BLINK | STYLE_FG_YELLOW, "%d eggs!\n", params.eggs[menu->param]);

  console_write(STYLE_BOLD | STYLE_FG_GREEN, "\nContinue...\n");
}

void menu_competition_action(state_t* state, action_t action) {
  menu_item_t* menu = &state->menus.menu_list[state->menus.current_menu];

  switch (action) {
    case ACTION_SELECT:
      if (menu->param == state->contest.registered_participants - 1) {
        write(params.pipe_fd[1], params.eggs, sizeof(int) * state->contest.registered_participants);
        close(params.pipe_fd[1]);
        free(params.eggs);
        exit(0);
      }

      menu->param++;
      state->is_changed = 1;
      return;
    case ACTION_BACK:
    case ACTION_PAGE_PREV:
    case ACTION_PAGE_NEXT:
    case ACTION_PREV:
    case ACTION_NEXT     : return;
  }
}

menu_item_t menu_competition_create(void) {
  return (menu_item_t){
      .title  = "Poem reading competition",
      .draw   = menu_competition_draw,
      .action = menu_competition_action,
  };
}
