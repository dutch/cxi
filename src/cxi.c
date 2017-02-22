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
#include "backend.h"
#include <config.h>
#include <curses.h>
#include <fcntl.h>
#include <getopt.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void
new_tab_cb(void)
{
  initscr();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  timeout(0);
  getch();
  keypad(stdscr, FALSE);
  intrflush(stdscr, TRUE);
  nl();
  echo();
  nocbreak();
  endwin();
  exit(EXIT_SUCCESS);
}

void
do_help(void)
{
  printf("Usage: cxi [options]\n");
  printf("\n");
  printf("Options:\n");
  printf("  -h, --help      print this message and exit\n");
  printf("  -v, --version   print version number and exit\n");
  printf("\n");
  printf("Report bugs to <" PACKAGE_BUGREPORT ">\n");
  exit(EXIT_SUCCESS);
}

void
do_version(void)
{
  printf(PACKAGE_STRING "\n");
  exit(EXIT_SUCCESS);
}

int
main(int argc, char *argv[])
{
  pid_t res;
  int ch, in_pfd[2], out_pfd[2], optidx, flags;

  setlocale(LC_ALL, "");

  for (;;) {
    optidx = 0;
    static struct option options[] = {
      { "help",    no_argument, NULL, 'h' },
      { "version", no_argument, NULL, 'v' },
      { NULL,      0,           NULL, 0   }
    };

    if ((ch = getopt_long(argc, argv, "hv", options, &optidx)) == -1)
      break;

    switch (ch) {
    case 'h':
      do_help();
      break;

    case 'v':
      do_version();
      break;

    default:
      do_help();
    }
  }

  pipe(in_pfd);
  pipe(out_pfd);

  flags = fcntl(in_pfd[0], F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl(in_pfd[0], F_SETFL, flags);

  flags = fcntl(in_pfd[1], F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl(in_pfd[1], F_SETFL, flags);

  flags = fcntl(out_pfd[0], F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl(out_pfd[0], F_SETFL, flags);

  flags = fcntl(out_pfd[1], F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl(out_pfd[1], F_SETFL, flags);

  if ((res = fork()) == -1) {
    perror("fork");
    return EXIT_FAILURE;
  }

  if (res == 0) {
    close(in_pfd[1]);
    close(out_pfd[0]);
    dup2(in_pfd[0], STDIN_FILENO);
    dup2(out_pfd[1], STDOUT_FILENO);
    dup2(out_pfd[1], STDERR_FILENO);
    close(out_pfd[1]);
    execlp("xi-core", "xi-core", (char *)NULL);
    _exit(EXIT_SUCCESS);
  } else {
    close(in_pfd[0]);
    close(out_pfd[1]);
    new_tab(in_pfd[1], new_tab_cb);
    process_events(out_pfd[0]);
  }

  return EXIT_SUCCESS;
}
