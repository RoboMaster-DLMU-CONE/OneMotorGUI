#ifndef CAN_CONNECTION_PANEL_HPP
#define CAN_CONNECTION_PANEL_HPP

#include <gtkmm.h>
#include <vector>
#include <string>

namespace OneMotorGUI
{
    class CanConnectionPanel final : public Gtk::Box
    {
    public:
        CanConnectionPanel();

        // 信号
        sigc::signal<void(const std::string &)> signal_connection_changed;

        // 公共方法
        bool is_connected() const { return m_connected; }
        std::string get_interface() const;

    private:
        void setupUI();
        void onConnectButtonClicked();
        void updateConnectionStatus(bool connected, const std::string &message = "");
        void populateCanInterfaces();
        static std::vector<std::string> findCanInterfaces();

        // UI组件
        Gtk::Frame m_frame;
        Gtk::Box m_content_box;
        Gtk::Box m_interface_box;
        Gtk::Box m_button_box;
        Gtk::Box m_status_box;

        Gtk::Label m_interface_label;
        Gtk::ComboBoxText m_interface_combo;

        Gtk::Button m_connect_button;

        Gtk::Image m_status_icon;
        Gtk::Label m_status_label;

        // 状态
        bool m_connected;
        std::string m_current_interface;
    };
}

#endif // CAN_CONNECTION_PANEL_HPP
