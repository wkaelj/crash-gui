#pragma once

struct window;

typedef void (*window_closed_callback)(void *usr);

struct window *create_window(const char *t, const char *e);

int run_window(struct window *w, int argc, char **argv);

void destroy_window(struct window *w);
