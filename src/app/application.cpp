#include "application.h"
#include "nvcore/version.h"
#include <iostream>
namespace nv::app {
Application::Application() {
    gtkApp_ = gtk_application_new("edit.nirvana.app", G_APPLICATION_FLAGS_NONE);
    if (!gtkApp_) { std::cerr << "[FATAL] Failed to create GtkApplication\n"; return; }
    g_signal_connect(gtkApp_, "activate", G_CALLBACK(onActivate), this);
    g_signal_connect(gtkApp_, "shutdown", G_CALLBACK(onShutdown), this);
}
Application::~Application() { if (gtkApp_) { g_object_unref(gtkApp_); gtkApp_ = nullptr; } }
int Application::run(int argc, char** argv) { return gtkApp_ ? g_application_run(G_APPLICATION(gtkApp_), argc, argv) : 1; }
void Application::onActivate(GtkApplication*, gpointer userData) { static_cast<Application*>(userData)->activate(); }
void Application::onShutdown(GtkApplication*, gpointer userData) { static_cast<Application*>(userData)->shutdown(); }
void Application::activate() {
    const auto& ver = nv::currentVersion();
    std::string title = "NirvanaEdit " + ver.toString();
    mainWindow_ = gtk_application_window_new(gtkApp_);
    gtk_window_set_title(GTK_WINDOW(mainWindow_), title.c_str());
    gtk_window_set_default_size(GTK_WINDOW(mainWindow_), 1280, 720);
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(mainWindow_), box);
    GtkWidget* label = gtk_label_new("NirvanaEdit — Foundation Ready");
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);
    gtk_widget_show_all(mainWindow_);
    std::cout << "[INFO] NirvanaEdit " << ver.toString() << " started\n";
}
void Application::shutdown() { std::cout << "[INFO] NirvanaEdit shutting down\n"; mainWindow_ = nullptr; }
}
