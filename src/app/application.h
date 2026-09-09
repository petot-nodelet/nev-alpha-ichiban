#pragma once
#include <gtk/gtk.h>
namespace nv::app {
class Application {
public:
    Application();
    ~Application();
    int run(int argc, char** argv);
private:
    static void onActivate(GtkApplication* app, gpointer userData);
    static void onShutdown(GtkApplication* app, gpointer userData);
    void activate();
    void shutdown();
    GtkApplication* gtkApp_ = nullptr;
    GtkWidget* mainWindow_ = nullptr;
};
}
