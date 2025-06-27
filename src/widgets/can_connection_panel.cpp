#include "widgets/can_connection_panel.hpp"
#include <iostream>
#include <netlink/socket.h>
#include <netlink/cache.h>
#include <netlink/route/link.h>

namespace OneMotorGUI
{
    namespace
    {
        // 回调函数，用于处理每个网络链接
        void link_filter_cb(nl_object *obj, void *data)
        {
            auto *links = static_cast<std::vector<std::string> *>(data);
            auto *link = reinterpret_cast<struct rtnl_link *>(obj);

            // 检查链接类型
            if (const char *type = rtnl_link_get_type(link); type && strcmp(type, "can") == 0)
            {
                links->emplace_back(rtnl_link_get_name(link));
            }
        }
    }

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
        populateCanInterfaces();
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

        m_interface_combo.set_hexpand(true);

        m_interface_box.append(m_interface_label);
        m_interface_box.append(m_interface_combo);

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

    std::string CanConnectionPanel::get_interface() const
    {
        return m_interface_combo.get_active_text();
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
            const std::string interface = m_interface_combo.get_active_text();
            if (interface.empty())
            {
                updateConnectionStatus(false, "请选择一个接口");
                return;
            }

            bool connection_success = true; // 实际应该根据CAN连接结果设置

            if (connection_success)
            {
                m_current_interface = interface;
                updateConnectionStatus(true, "已连接到 " + interface);
                std::cout << "连接到CAN接口: " << interface << std::endl;

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
        m_interface_combo.set_sensitive(!connected);

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

    void CanConnectionPanel::populateCanInterfaces()
    {
        m_interface_combo.remove_all();
        if (const auto interfaces = findCanInterfaces(); interfaces.empty())
        {
            m_interface_combo.append("无可用CAN接口");
            m_interface_combo.set_active(0);
            m_interface_combo.set_sensitive(false);
            m_connect_button.set_sensitive(false);
        }
        else
        {
            for (const auto &iface : interfaces)
            {
                m_interface_combo.append(iface);
            }
            m_interface_combo.set_active(0);
        }
    }

    std::vector<std::string> CanConnectionPanel::findCanInterfaces()
    {
        std::vector<std::string> interfaces;
        nl_sock *sk = nl_socket_alloc();
        if (!sk)
        {
            std::cerr << "Failed to allocate netlink socket." << std::endl;
            return interfaces;
        }

        if (nl_connect(sk, NETLINK_ROUTE) < 0)
        {
            std::cerr << "Failed to connect to netlink socket." << std::endl;
            nl_socket_free(sk);
            return interfaces;
        }

        nl_cache *cache = nullptr;
        if (rtnl_link_alloc_cache(sk, AF_UNSPEC, &cache) < 0)
        {
            std::cerr << "Failed to allocate link cache." << std::endl;
            nl_socket_free(sk);
            return interfaces;
        }

        nl_cache_foreach(cache, link_filter_cb, &interfaces);

        nl_cache_free(cache);
        nl_socket_free(sk);

        return interfaces;
    }
}
