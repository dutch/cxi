/**
 * Copyright (C) 2017 Chris Lamberson
 *
 * This file is part of cxi.
 *
 * cxi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cxi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cxi.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "event.h"
#include "jsmn.h"
#include "backend.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

#define BUFFERLEN 8192
#define TOKENMAX 128

void
process_events(int outfd)
{
  struct kevent change, ev;
  int kqfd, nevs, i, ntoks, j;
  ssize_t length;
  char buf[BUFFERLEN];
  jsmn_parser parser;
  jsmntok_t toks[TOKENMAX];

  if ((kqfd = kqueue()) == -1) {
    perror("kqueue");
    exit(EXIT_FAILURE);
  }

  EV_SET(&change, outfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);

  for (;;) {
    if ((nevs = kevent(kqfd, &change, 1, &ev, 1, NULL)) == -1) {
      perror("kevent");
      exit(EXIT_FAILURE);
    }

    for (i = 0; i < nevs; ++i) {
      if (ev.ident == outfd) {
        for (;;) {
          length = read(outfd, buf, BUFFERLEN);

          if (length == -1) {
            if (errno == EAGAIN)
              break;
            perror("read");
            exit(EXIT_FAILURE);
          }

          if (length == 0)
            break;

          jsmn_init(&parser);
          if ((ntoks = jsmn_parse(&parser, buf, length, toks, TOKENMAX)) < 0) {
            fprintf(stderr, "jsmn_parse: Parse error\n");
            exit(EXIT_FAILURE);
          }

          for (j = 1; j < ntoks; j += 2) {
            if (buf[toks[j].start] == 'i' && buf[toks[j].start+1] == 'd') {
              long id = strtoul(&buf[toks[j+1].start], NULL, 10);
              callback cb = pop_callback(id);
              cb();
            }
          }
        }
      }
    }
  }
}
