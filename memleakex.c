#include <assert.h>
#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer usr)
{
    GtkWidget *w = gtk_application_window_new(app);
    assert(w);

    gtk_window_set_title(GTK_WINDOW(w), "Test window");
    gtk_window_set_default_size(GTK_WINDOW(w), 800, 600);

    gtk_window_present(GTK_WINDOW(w));
}

int main(int argc, char **argv)
{
    GtkApplication *app =
        gtk_application_new("testapp.work", G_APPLICATION_FLAGS_NONE);
    assert(app);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(G_APPLICATION(app));
}