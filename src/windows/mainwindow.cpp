#include "windows/mainwindow.hpp"

#include <utility>

namespace OneMotorGUI
{
    MainWindow::MainWindow()
        : m_main_box(Gtk::Orientation::HORIZONTAL),
          m_paned(Gtk::Orientation::HORIZONTAL),
          m_left_box(Gtk::Orientation::VERTICAL) // Changed from m_left_frame
    {
        set_title("OneMotorGUI - 电机调试工具");
        set_default_size(1200, 800);

        setupUI();

        // 连接信号
        m_motor_tree.signal_motor_selected.connect(
            sigc::mem_fun(*this, &MainWindow::onMotorSelected));
    }

    void MainWindow::setupUI()
    {
        // 设置左侧TreeView
        m_scrolled_window.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
        m_scrolled_window.set_vexpand(true);
        m_scrolled_window.set_hexpand(true);
        m_scrolled_window.set_child(m_motor_tree);

        // 设置左侧Box
        m_left_box.append(m_scrolled_window);
        m_left_box.set_vexpand(true);
        m_left_box.set_hexpand(true);

        // 设置Paned
        m_paned.set_start_child(m_left_box); // Changed from m_left_frame
        m_paned.set_end_child(m_control_panel);
        m_paned.set_position(300); // 左侧宽度300像素
        m_paned.set_resize_start_child(false);
        m_paned.set_resize_end_child(true);
        m_paned.set_shrink_start_child(false);
        m_paned.set_shrink_end_child(false);

        // 设置主容器
        set_child(m_paned);
    }

    void MainWindow::onMotorSelected(std::shared_ptr<MotorInfo> motor_info)
    {
        m_control_panel.setMotor(std::move(motor_info));
    }
}