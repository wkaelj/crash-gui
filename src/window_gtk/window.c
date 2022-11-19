#include "window.h"

#include <assert.h>
#include <gtk/gtk.h>
#include <stdlib.h>

#include <dlfcn.h>

typedef void *Application;

void gtk_widget_set_margin(GtkWidget *widget, int margin);

struct window
{
    const char *title;
    const char *error;
    Application *app;
    GtkWidget *window;
    GtkWidget *close_button;
    GtkWidget *error_label;

    void *libadwaita;
};

static void
close_button_callback([[maybe_unused]] GtkButton *button, gpointer usr)
{
    const struct window *w = usr;
    gtk_window_destroy(GTK_WINDOW(w->window));
}

static void activate([[maybe_unused]] GtkApplication *app, gpointer usr)
{
    struct window *w = usr;

    // assert w is correctly constructed
    assert(w);
    assert(w->title);
    assert(w->error);
    assert(w->app);
    assert(w->window == NULL);

    const int window_size[2] = {400, 300};

    w->window = gtk_application_window_new(app);

    gtk_window_set_title(GTK_WINDOW(w->window), w->title);

    gtk_window_set_decorated(GTK_WINDOW(w->window), true);

    gtk_window_set_default_size(
        GTK_WINDOW(w->window), window_size[0], window_size[1]);

    gtk_window_set_title(GTK_WINDOW(w->window), w->title);

    gtk_window_set_icon_name(GTK_WINDOW(w->window), "dialog-error");

    // create exit button
    w->close_button = gtk_button_new_with_label("Okay");
    gtk_widget_set_size_request(w->close_button, window_size[0] / 2, 0);

    gtk_widget_set_margin(w->close_button, 10);

    g_signal_connect(
        w->close_button, "clicked", G_CALLBACK(close_button_callback), w);

    // create error label
    w->error_label = gtk_label_new(w->error);
    gtk_widget_set_vexpand(w->error_label, true);

    GtkWidget *titlebar = gtk_header_bar_new();
    gtk_header_bar_set_title_widget(
        GTK_HEADER_BAR(titlebar), gtk_label_new(w->title));
    gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(titlebar), true);
    gtk_window_set_titlebar(GTK_WINDOW(w->window), titlebar);

    // create box layout
    GtkWidget *layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(layout, window_size[0], window_size[1]);
    gtk_box_set_spacing(GTK_BOX(layout), 10);

    // gtk_box_append(GTK_BOX(layout), titlebar);
    gtk_box_append(GTK_BOX(layout), w->error_label);
    gtk_box_append(GTK_BOX(layout), w->close_button);

    gtk_window_set_child(GTK_WINDOW(w->window), layout);
    gtk_window_present(GTK_WINDOW(w->window));
}

struct window *create_window(const char *t, const char *e)
{
    struct window *w = malloc(sizeof(struct window));

    // check if the app needs to use adwaita
    void *libadwaita = dlopen("libadwaita-1.so", RTLD_LAZY);

    Application *app   = NULL;
    const char *app_id = "crash.gui.window";

    // if libadwaita is present, use it. Otherwise use gtk by itself
    if (libadwaita)
    {
        Application (*adw_application_new)(const char *, int) =
            dlsym(libadwaita, "adw_application_new");
        app = adw_application_new(app_id, G_APPLICATION_DEFAULT_FLAGS);
    }
    else
    {
        app = (Application)gtk_application_new(
            app_id, G_APPLICATION_DEFAULT_FLAGS);
    }

    *w = (struct window){
        .app        = app,
        .error      = e,
        .title      = t,
        .libadwaita = libadwaita,
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
    dlclose(w->libadwaita);
    free(w);
}

void gtk_widget_set_margin(GtkWidget *widget, int margin)
{
    gtk_widget_set_margin_start(widget, margin);
    gtk_widget_set_margin_end(widget, margin);
    gtk_widget_set_margin_top(widget, margin);
    gtk_widget_set_margin_bottom(widget, margin);
}