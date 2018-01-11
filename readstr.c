#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "readstr.h"

void ReadSocket(int fd, char* str) {
  do {
      if ((read(fd, str, 1)) < 0) {
          perror("readstr: read error");
      }
  } while(*str++ != '\n');
  *str = '\0';
}

void WriteSocket(int fd, char *str) {
    if ((write(fd, str, strlen(str))) < 0) {
        perror("readstr: write error");
    }
}

int getrand() {
    srandom((long) getpid());
    return random()%3;
}

