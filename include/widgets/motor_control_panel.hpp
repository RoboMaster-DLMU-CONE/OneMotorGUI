#ifndef MOTOR_CONTROL_PANEL_HPP
#define MOTOR_CONTROL_PANEL_HPP

#include <gtkmm.h>
#include "models/motor_info.hpp"

namespace OneMotorGUI
{
    class MotorControlPanel final : public Gtk::Box
    {
    public:
        MotorControlPanel();

        void setMotor(std::shared_ptr<MotorInfo> motor_info);

    private:
        void setupUI();
        void onPIDSubmit();
        void onCalibrationSubmit();
        void updateMotorInfo();

        // UI组件
        Gtk::Label m_motor_title;

        // PID参数组
        Gtk::Frame m_pid_frame;
        Gtk::Box m_pid_box;
        Gtk::Grid m_pid_grid;
        Gtk::Label m_p_label, m_i_label, m_d_label;
        Gtk::Scale m_p_scale, m_i_scale, m_d_scale;
        Gtk::SpinButton m_p_spin, m_i_spin, m_d_spin;
        Gtk::Button m_pid_submit_btn;

        // 标定值组
        Gtk::Frame m_calibration_frame;
        Gtk::Box m_calibration_box;
        Gtk::Grid m_calibration_grid;
        Gtk::Label m_calibration_label;
        Gtk::SpinButton m_calibration_spin;
        Gtk::Button m_calibration_submit_btn;

        // 当前电机信息
        std::shared_ptr<MotorInfo> m_current_motor;

        // 调整对象
        Glib::RefPtr<Gtk::Adjustment> m_p_adjustment, m_i_adjustment, m_d_adjustment;
        Glib::RefPtr<Gtk::Adjustment> m_calibration_adjustment;
    };
}

#endif // MOTOR_CONTROL_PANEL_HPP
