#pragma once

#include <gtk/gtk.h>

namespace nv::app {

// Application — GTK application wrapper.
// Owns the main window and application lifecycle.
class Application {
public:
    Application();
    ~Application();

    // Run the GTK main loop. Returns exit code.
    int run(int argc, char** argv);

private:
    // GTK callbacks (static, with 'this' passed via user_data)
    static void onActivate(GtkApplication* app, gpointer userData);
    static void onShutdown(GtkApplication* app, gpointer userData);

    void activate();
    void shutdown();

    GtkApplication* gtkApp_ = nullptr;
    GtkWidget* mainWindow_ = nullptr;
};

} // namespace nv::app
