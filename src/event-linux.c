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
#include <sys/epoll.h>
#include <unistd.h>

#define BUFFERLEN 8192
#define EVENTMAX 10
#define TOKENMAX 128

void
process_events(int outfd)
{
  int epollfd, nfds, n, ntoks, i;
  struct epoll_event ev, evs[EVENTMAX];
  ssize_t length;
  char buf[BUFFERLEN];
  jsmn_parser parser;
  jsmntok_t toks[TOKENMAX];

  if ((epollfd = epoll_create1(0)) == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = outfd;

  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, outfd, &ev) == -1) {
    perror("epoll_ctl");
    exit(EXIT_FAILURE);
  }

  for (;;) {
    if ((nfds = epoll_wait(epollfd, evs, EVENTMAX, -1)) == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }

    for (n = 0; n < nfds; ++n) {
      if (evs[n].data.fd == outfd) {
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

          for (i = 1; i < ntoks; i += 2) {
            if (buf[toks[i].start] == 'i' && buf[toks[i].start+1] == 'd') {
              long id = strtoul(&buf[toks[i+1].start], NULL, 10);
              callback cb = pop_callback(id);
              cb();
            }
          }
        }
      }
    }
  }
}
