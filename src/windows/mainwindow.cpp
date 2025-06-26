#include "windows/mainwindow.hpp"
#include <iostream>
#include <utility>

namespace OneMotorGUI
{
    MainWindow::MainWindow()
        : m_main_box(Gtk::Orientation::HORIZONTAL),
          m_paned(Gtk::Orientation::HORIZONTAL),
          m_left_box(Gtk::Orientation::VERTICAL)
    {
        set_title("OneMotorGUI - 电机调试工具");
        set_default_size(1200, 800);

        setupUI();

        m_motor_tree.signal_motor_selected.connect(
            sigc::mem_fun(*this, &MainWindow::onMotorSelected));

        m_can_panel.signal_connection_changed.connect(
            sigc::mem_fun(*this, &MainWindow::onCanConnectionChanged));
    }

    void MainWindow::setupUI()
    {
        // 设置CAN连接面板
        m_can_panel.set_vexpand(false);
        m_can_panel.set_hexpand(true);

        // 设置TreeView
        m_scrolled_window.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
        m_scrolled_window.set_vexpand(true);
        m_scrolled_window.set_hexpand(true);
        m_scrolled_window.set_child(m_motor_tree);

        // 组装左侧面板
        m_left_box.append(m_can_panel);
        m_left_box.append(m_scrolled_window);
        m_left_box.set_vexpand(true);
        m_left_box.set_hexpand(true);

        // 设置Paned
        m_paned.set_start_child(m_left_box);
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

    void MainWindow::onCanConnectionChanged(const std::string &interface)
    {
        // 处理CAN连接状态变化
        if (m_can_panel.is_connected())
        {
            std::cout << "CAN已连接到接口: " << interface << std::endl;
            // 这里可以添加连接成功后的逻辑，比如启用电机扫描等
        }
        else
        {
            std::cout << "CAN连接已断开" << std::endl;
            // 这里可以添加断开连接后的逻辑，比如清空电机列表等
        }
    }
}