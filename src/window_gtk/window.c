#include "window.h"

#include <assert.h>
#include <gtk/gtk.h>
#include <stdlib.h>

struct window
{
    const char *title;
    const char *error;
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *close_button;
    GtkWidget *error_label;
};

static void
close_button_callback([[maybe_unused]] GtkButton *button, gpointer usr)
{
    const struct window *w = usr;
    gtk_window_destroy(GTK_WINDOW(w->window));
}

static void activate(GtkApplication *app, gpointer usr)
{
    struct window *w = usr;

    // assert w is correctly constructed
    assert(w);
    assert(w->title);
    assert(w->error);
    assert(w->app);
    assert(w->window == NULL);

    w->window = gtk_application_window_new(app);
    // gtk_window_set_decorated(GTK_WINDOW(w->window), false);

    const int window_size[2] = {400, 300};

    gtk_window_set_title(GTK_WINDOW(w->window), w->title);
    gtk_window_set_default_size(
        GTK_WINDOW(w->window), window_size[0], window_size[1]);
    gtk_window_set_icon_name(GTK_WINDOW(w->window), "dialog-error");

    // create exit button
    w->close_button = gtk_button_new_with_label("Okay");
    gtk_widget_set_size_request(w->close_button, window_size[0] / 2, 0);

    g_signal_connect(
        w->close_button, "clicked", G_CALLBACK(close_button_callback), w);

    // create error label
    w->error_label = gtk_label_new(w->error);
    gtk_widget_set_vexpand(w->error_label, true);

    // create title bar
    GtkWidget *titlebar = gtk_header_bar_new();
    gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(titlebar), true);
    gtk_header_bar_set_title_widget(
        GTK_HEADER_BAR(titlebar), gtk_label_new(w->title));

    // create box layout
    GtkWidget *layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_set_spacing(GTK_BOX(layout), 10);
    g_object_set(G_OBJECT(layout), "padding", 20, NULL);
    gtk_box_append(GTK_BOX(layout), titlebar);
    gtk_box_append(GTK_BOX(layout), w->error_label);
    gtk_box_append(GTK_BOX(layout), w->close_button);

    gtk_window_set_child(GTK_WINDOW(w->window), layout);

    gtk_window_present(GTK_WINDOW(w->window));
}

struct window *create_window(const char *t, const char *e)
{
    GtkApplication *app =
        gtk_application_new("org.crashgui.window", G_APPLICATION_DEFAULT_FLAGS);
    struct window *w = malloc(sizeof(struct window));

    *w = (struct window){
        .app   = app,
        .error = e,
        .title = t,
    };
    g_signal_connect(w->app, "activate", G_CALLBACK(activate), w);

    return w;
}

int run_window(struct window *w, int argc, char **argv)
{
    assert(w->app);
    return g_application_run(G_APPLICATION(w->app), argc, argv);
}

void destroy_window(struct window *w)
{
    g_object_unref(w->app);
    free(w);
}