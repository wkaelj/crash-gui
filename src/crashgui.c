#include "crashgui.h"

#include <stddef.h>

#ifdef __APPLE__
#include "window_osx/window.h"
#include <unistd.h>
#elifdef __unix
#include "window_gtk/window.h"
#include <unistd.h>
#elifdef _WIN_32_
#warning crash gui does not support windows
#include "window_win/window.h"
#endif

static void fork_main(const char *error)
{
#ifndef __unix
    return;
#else

    struct window *w     = create_window("A Fatal Error Occured:", error);
    char *fake_exec_name = "libcrashgui";
    run_window(w, 1, &fake_exec_name);
    destroy_window(w);
#endif
}

void create_crashgui(const char *error)
{
    // create fork
    if (fork() == 0)
        fork_main(error);
    else
        return;
}