#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include <gtkmm.h>
#include "widgets/motor_tree_view.hpp"
#include "widgets/motor_control_panel.hpp"
#include "widgets/can_connection_panel.hpp"
#include "models/motor_info.hpp"

namespace OneMotorGUI
{
    class MainWindow final : public Gtk::Window
    {
    public:
        MainWindow();

    private:
        void setupUI();
        void onMotorSelected(std::shared_ptr<MotorInfo> motor_info);
        void onCanConnectionChanged(const std::string &interface);

        // UI组件
        Gtk::Box m_main_box;
        Gtk::Paned m_paned;
        Gtk::Box m_left_box;

        // 左侧面板
        CanConnectionPanel m_can_panel;
        Gtk::ScrolledWindow m_scrolled_window;
        MotorTreeView m_motor_tree;

        // 右侧面板
        MotorControlPanel m_control_panel;
    };
}

#endif // MAINWINDOW_HPP
