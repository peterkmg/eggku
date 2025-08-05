#ifndef __HAIKU_H__
#define __HAIKU_H__

#define HAIKU_NUM         15
#define HAIKU_LINE_LENGTH 40
#define HAIKU_LINE_COUNT  3

#define HAIKU_FILE "haikus.txt"

typedef char haiku_line_t[HAIKU_LINE_LENGTH];

typedef struct haiku_t {
  haiku_line_t lines[HAIKU_LINE_COUNT];
} haiku_t;

haiku_t* haiku_load_from_file(void);

#endif  // __HAIKU_H__
