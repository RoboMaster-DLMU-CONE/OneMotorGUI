#include "widgets/can_connection_panel.hpp"
#include <iostream>

namespace OneMotorGUI
{
    CanConnectionPanel::CanConnectionPanel()
        : Gtk::Box(Gtk::Orientation::VERTICAL),
          m_frame("CAN连接"),
          m_content_box(Gtk::Orientation::VERTICAL),
          m_interface_box(Gtk::Orientation::HORIZONTAL),
          m_button_box(Gtk::Orientation::HORIZONTAL),
          m_status_box(Gtk::Orientation::HORIZONTAL),
          m_interface_label("接口:"),
          m_connect_button("连接"),
          m_status_label("未连接"),
          m_connected(false)
    {
        setupUI();
    }

    void CanConnectionPanel::setupUI()
    {
        // 设置间距和边距
        set_spacing(5);
        set_margin(10);

        m_content_box.set_spacing(10);
        m_content_box.set_margin(10);

        // 接口输入框区域
        m_interface_box.set_spacing(5);
        m_interface_box.set_homogeneous(false);

        m_interface_label.set_size_request(60, -1);
        m_interface_label.set_halign(Gtk::Align::START);

        m_interface_entry.set_text("can0");
        m_interface_entry.set_placeholder_text("例如: can0");
        m_interface_entry.set_hexpand(true);

        m_interface_box.append(m_interface_label);
        m_interface_box.append(m_interface_entry);

        // 连接按钮区域
        m_button_box.set_spacing(5);
        m_button_box.set_halign(Gtk::Align::CENTER);

        m_connect_button.set_size_request(100, 35);
        m_connect_button.signal_clicked().connect(
            sigc::mem_fun(*this, &CanConnectionPanel::onConnectButtonClicked));

        m_button_box.append(m_connect_button);

        // 状态指示区域
        m_status_box.set_spacing(8);
        m_status_box.set_halign(Gtk::Align::CENTER);

        // 设置状态图标（红色圆点表示未连接）
        updateConnectionStatus(false);

        m_status_box.append(m_status_icon);
        m_status_box.append(m_status_label);

        // 组装内容
        m_content_box.append(m_interface_box);
        m_content_box.append(m_button_box);
        m_content_box.append(m_status_box);

        m_frame.set_child(m_content_box);

        // 添加到主容器
        append(m_frame);

        set_vexpand(false);
        set_hexpand(true);
    }

    void CanConnectionPanel::onConnectButtonClicked()
    {
        if (m_connected)
        {
            // 断开连接
            updateConnectionStatus(false, "已断开连接");
            m_current_interface.clear();
            std::cout << "断开CAN连接" << std::endl;
        }
        else
        {
            // 尝试连接
            std::string interface = m_interface_entry.get_text();
            if (interface.empty())
            {
                updateConnectionStatus(false, "请输入接口名称");
                return;
            }

            // 这里应该实现实际的CAN连接逻辑
            // 目前只是模拟连接成功
            bool connection_success = true; // 实际应该根据CAN连接结果设置

            if (connection_success)
            {
                m_current_interface = interface;
                updateConnectionStatus(true, "已连接到 " + interface);
                std::cout << "连接到CAN接口: " << interface << std::endl;

                // 发射信号通知连接状态改变
                signal_connection_changed.emit(interface);
            }
            else
            {
                updateConnectionStatus(false, "连接失败");
            }
        }
    }

    void CanConnectionPanel::updateConnectionStatus(bool connected, const std::string &message)
    {
        m_connected = connected;

        if (connected)
        {
            // 绿色圆点表示已连接
            m_status_icon.set_from_icon_name("network-wired-symbolic");
            m_status_icon.set_pixel_size(16);
            m_status_icon.add_css_class("success");

            m_connect_button.set_label("断开");
            m_connect_button.remove_css_class("suggested-action");
            m_connect_button.add_css_class("destructive-action");

            if (message.empty())
            {
                m_status_label.set_text("已连接");
            }
            else
            {
                m_status_label.set_text(message);
            }
        }
        else
        {
            // 红色圆点表示未连接
            m_status_icon.set_from_icon_name("network-wired-disconnected-symbolic");
            m_status_icon.set_pixel_size(16);
            m_status_icon.remove_css_class("success");

            m_connect_button.set_label("连接");
            m_connect_button.remove_css_class("destructive-action");
            m_connect_button.add_css_class("suggested-action");

            if (message.empty())
            {
                m_status_label.set_text("未连接");
            }
            else
            {
                m_status_label.set_text(message);
            }
        }
    }
}
