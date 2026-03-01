#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TB_IMPL
#include "termbox2.h"

void clear() {
  tb_clear();
  tb_present();
}

void print(int init_x, int init_y, const char *print_text, uintattr_t fg, uintattr_t bg) {
  int x = init_x;
  int y = init_y;
  for (int data = 0; print_text[data] != '\0'; data++) {
    if (print_text[data] == '\n' ) {
      x = init_x;
      y++;
      continue;
    }
    tb_set_cell(x, y, print_text[data], fg, bg);
    x++;
  }
}

char* getfile(char filename[256]) {
  FILE *file;
  file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "File failed to open!\n");
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);
  char *data = malloc(size + 1);
  if (data) {
    fread(data, 1, size, file);
    data[size] = '\0';
  }
  fclose(file);
  return data;
}

char getinput(struct tb_event *ev) {
  if (tb_poll_event(ev) == TB_OK) {
    if (ev->type == TB_EVENT_KEY && ev->ch != 0) {
      return (char)ev->ch;
    }
  }
}

void changeline(char input) {
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "You must specify a file!\n");
    exit(1);
  }

  if (strlen(argv[1]) > 255) {
    fprintf(stderr, "File name too long!\n");
    exit(1);
  }

  char *text = getfile(argv[1]);

  if (tb_init() != TB_OK) {
    return 1;
  }
  tb_set_clear_attrs(TB_WHITE, TB_DEFAULT);
  clear();

  print(0,0, text, TB_WHITE, TB_DEFAULT);
  tb_present();

  struct tb_event ev;
  while(1) {
    char input = getinput(&ev);
    if (ev.key == TB_KEY_ESC) {
      free(text);
      tb_shutdown();
      exit(0);
    }
  }
}
