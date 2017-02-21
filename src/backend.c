#include "backend.h"
#include <stdio.h>
#include <stdlib.h>
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

#include <unistd.h>

#define NCALLBACKS 8
#define BUFFERLEN 8192

static callback callbacks[NCALLBACKS] = { NULL, };

static int
add_callback(callback cb)
{
  int i;

  for (i = 0; i < NCALLBACKS; ++i) {
    if (callbacks[i])
      continue;
    callbacks[i] = cb;
    return i;
  }

  return -1;
}

callback
pop_callback(int n)
{
  callback ret = callbacks[n];
  callbacks[n] = NULL;
  return ret;
}

void
new_tab(int fd, callback cb)
{
  int id, length;
  char buf[BUFFERLEN];

  if ((id = add_callback(cb)) < 0) {
    fprintf(stderr, "callback slots exhausted\n");
    return;
  }

  length = snprintf(buf, BUFFERLEN, "{\"id\":%d,\"method\":\"new_tab\",\"params\":[]}\n", id);
  write(fd, buf, length);
}
