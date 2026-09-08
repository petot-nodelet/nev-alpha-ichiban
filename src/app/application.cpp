#include "application.h"
#include "nvcore/version.h"

#include <iostream>
#include <string>

namespace nv::app {

Application::Application() {
    // GTK application with unique ID (for single-instance behavior)
    gtkApp_ = gtk_application_new(
        "edit.nirvana.app",
        G_APPLICATION_FLAGS_NONE
    );

    if (!gtkApp_) {
        std::cerr << "[FATAL] Failed to create GtkApplication\n";
        return;
    }

    g_signal_connect(gtkApp_, "activate", G_CALLBACK(onActivate), this);
    g_signal_connect(gtkApp_, "shutdown", G_CALLBACK(onShutdown), this);
}

Application::~Application() {
    if (gtkApp_) {
        g_object_unref(gtkApp_);
        gtkApp_ = nullptr;
    }
}

int Application::run(int argc, char** argv) {
    if (!gtkApp_) {
        return 1;
    }
    return g_application_run(G_APPLICATION(gtkApp_), argc, argv);
}

void Application::onActivate(GtkApplication* /*app*/, gpointer userData) {
    auto* self = static_cast<Application*>(userData);
    self->activate();
}

void Application::onShutdown(GtkApplication* /*app*/, gpointer userData) {
    auto* self = static_cast<Application*>(userData);
    self->shutdown();
}

void Application::activate() {
    const auto& ver = nv::currentVersion();
    std::string title = "NirvanaEdit " + ver.toString();

    mainWindow_ = gtk_application_window_new(gtkApp_);
    gtk_window_set_title(GTK_WINDOW(mainWindow_), title.c_str());
    gtk_window_set_default_size(GTK_WINDOW(mainWindow_), 1280, 720);

    // Placeholder: empty box for future UI
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(mainWindow_), box);

    // Temporary label to confirm UI is alive
    GtkWidget* label = gtk_label_new("NirvanaEdit — Foundation Ready");
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

    gtk_widget_show_all(mainWindow_);

    std::cout << "[INFO] NirvanaEdit " << ver.toString()
              << " started (project format v"
              << nv::projectFormatVersion().toString() << ")\n";
}

void Application::shutdown() {
    std::cout << "[INFO] NirvanaEdit shutting down\n";
    // mainWindow_ is owned by GTK, no need to unref manually
    mainWindow_ = nullptr;
}

} // namespace nv::app
