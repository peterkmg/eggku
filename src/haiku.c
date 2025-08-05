#include "haiku.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void skip_line(FILE* file) {
  fscanf(file, "%*[^\n]");  // skip non-newline characters
  fscanf(file, "%*c");      // skip the newline
}

haiku_t* haiku_load_from_file(void) {
  haiku_t* haikus = (haiku_t*)calloc(HAIKU_NUM, sizeof(haiku_t));

  FILE* file = fopen(HAIKU_FILE, "r");
  if (file == NULL) {
    perror("Failed to open haiku file");
    free(haikus);
    exit(EXIT_FAILURE);
  }

  for (int k = 0; k < HAIKU_NUM; ++k) {
    for (int i = 0; i < HAIKU_LINE_COUNT; ++i) {
      if (fgets(haikus[k].lines[i], HAIKU_LINE_LENGTH, file) == NULL) {
        perror("Failed to read haiku line");
        fclose(file);
        free(haikus);
        exit(EXIT_FAILURE);
      }
      haikus[k].lines[i][strcspn(haikus[k].lines[i], "\n")] = '\0';
    }
    skip_line(file);
  }

  fclose(file);

  return haikus;
}
