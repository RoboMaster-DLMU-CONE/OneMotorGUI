#include "widgets/motor_control_panel.hpp"
#include <iostream>

namespace OneMotorGUI
{
    MotorControlPanel::MotorControlPanel()
        : Gtk::Box(Gtk::Orientation::VERTICAL, 10),
          m_motor_title("请选择一个电机"),
          m_pid_frame("PID参数"),
          m_pid_box(Gtk::Orientation::VERTICAL, 5),
          m_calibration_frame("标定值"),
          m_calibration_box(Gtk::Orientation::VERTICAL, 5),
          m_p_label("P:"), m_i_label("I:"), m_d_label("D:"),
          m_calibration_label("标定值:"),
          m_pid_submit_btn("提交PID参数"),
          m_calibration_submit_btn("提交标定值")
    {
        setupUI();
    }

    void MotorControlPanel::setupUI()
    {
        set_margin(10);

        // 设置标题
        m_motor_title.set_markup("<big><b>请选择一个电机</b></big>");
        m_motor_title.set_halign(Gtk::Align::START);
        append(m_motor_title);

        // 创建调整对象
        m_p_adjustment = Gtk::Adjustment::create(0.0, 0.0, 100.0, 0.1, 1.0, 0.0);
        m_i_adjustment = Gtk::Adjustment::create(0.0, 0.0, 100.0, 0.1, 1.0, 0.0);
        m_d_adjustment = Gtk::Adjustment::create(0.0, 0.0, 100.0, 0.1, 1.0, 0.0);
        m_calibration_adjustment = Gtk::Adjustment::create(0.0, -10000.0, 10000.0, 1.0, 10.0, 0.0);

        // 设置Scale和SpinButton
        m_p_scale.set_adjustment(m_p_adjustment);
        m_i_scale.set_adjustment(m_i_adjustment);
        m_d_scale.set_adjustment(m_d_adjustment);

        m_p_spin.set_adjustment(m_p_adjustment);
        m_i_spin.set_adjustment(m_i_adjustment);
        m_d_spin.set_adjustment(m_d_adjustment);
        m_calibration_spin.set_adjustment(m_calibration_adjustment);

        // 设置精度
        m_p_scale.set_digits(2);
        m_i_scale.set_digits(2);
        m_d_scale.set_digits(2);
        m_p_spin.set_digits(2);
        m_i_spin.set_digits(2);
        m_d_spin.set_digits(2);
        m_calibration_spin.set_digits(0);

        // 设置Scale宽度
        m_p_scale.set_hexpand(true);
        m_i_scale.set_hexpand(true);
        m_d_scale.set_hexpand(true);

        // 布局PID参数
        m_pid_grid.set_row_spacing(5);
        m_pid_grid.set_column_spacing(10);

        m_pid_grid.attach(m_p_label, 0, 0, 1, 1);
        m_pid_grid.attach(m_p_scale, 1, 0, 1, 1);
        m_pid_grid.attach(m_p_spin, 2, 0, 1, 1);

        m_pid_grid.attach(m_i_label, 0, 1, 1, 1);
        m_pid_grid.attach(m_i_scale, 1, 1, 1, 1);
        m_pid_grid.attach(m_i_spin, 2, 1, 1, 1);

        m_pid_grid.attach(m_d_label, 0, 2, 1, 1);
        m_pid_grid.attach(m_d_scale, 1, 2, 1, 1);
        m_pid_grid.attach(m_d_spin, 2, 2, 1, 1);

        m_pid_box.append(m_pid_grid);
        m_pid_box.append(m_pid_submit_btn);
        m_pid_frame.set_child(m_pid_box);

        // 布局标定值
        m_calibration_grid.set_row_spacing(5);
        m_calibration_grid.set_column_spacing(10);
        m_calibration_grid.attach(m_calibration_label, 0, 0, 1, 1);
        m_calibration_grid.attach(m_calibration_spin, 1, 0, 1, 1);

        m_calibration_box.append(m_calibration_grid);
        m_calibration_box.append(m_calibration_submit_btn);
        m_calibration_frame.set_child(m_calibration_box);

        // 添加到主容器
        append(m_pid_frame);
        append(m_calibration_frame);

        // 连接信号
        m_pid_submit_btn.signal_clicked().connect(
            sigc::mem_fun(*this, &MotorControlPanel::onPIDSubmit));
        m_calibration_submit_btn.signal_clicked().connect(
            sigc::mem_fun(*this, &MotorControlPanel::onCalibrationSubmit));

        // 初始状态下禁用控件
        set_sensitive(false);
    }

    void MotorControlPanel::setMotor(std::shared_ptr<MotorInfo> motor_info)
    {
        m_current_motor = motor_info;
        updateMotorInfo();
        set_sensitive(true);
    }

    void MotorControlPanel::updateMotorInfo()
    {
        if (!m_current_motor)
        {
            m_motor_title.set_markup("<big><b>请选择一个电机</b></big>");
            set_sensitive(false);
            return;
        }

        std::string title = "<big><b>" + m_current_motor->brand + " - " + m_current_motor->name + "</b></big>";
        m_motor_title.set_markup(title);

        // 根据不同电机类型设置不同的默认值
        switch (m_current_motor->type)
        {
        case MotorType::DJI_M3508:
            m_p_adjustment->set_value(8.0);
            m_i_adjustment->set_value(0.1);
            m_d_adjustment->set_value(0.0);
            break;
        case MotorType::DJI_G6020:
            m_p_adjustment->set_value(15.0);
            m_i_adjustment->set_value(0.0);
            m_d_adjustment->set_value(50.0);
            break;
        case MotorType::DAMIAO_J4310:
            m_p_adjustment->set_value(10.0);
            m_i_adjustment->set_value(0.5);
            m_d_adjustment->set_value(0.1);
            break;
        case MotorType::DAMIAO_J10010:
            m_p_adjustment->set_value(12.0);
            m_i_adjustment->set_value(0.3);
            m_d_adjustment->set_value(0.2);
            break;
        }

        m_calibration_adjustment->set_value(0.0);
    }

    void MotorControlPanel::onPIDSubmit()
    {
        if (!m_current_motor)
            return;

        double p = m_p_adjustment->get_value();
        double i = m_i_adjustment->get_value();
        double d = m_d_adjustment->get_value();

        std::cout << "提交PID参数 - 电机: " << m_current_motor->name
                  << ", P: " << p << ", I: " << i << ", D: " << d << std::endl;

        // TODO: 在这里添加实际的PID参数设置逻辑
    }

    void MotorControlPanel::onCalibrationSubmit()
    {
        if (!m_current_motor)
            return;

        int calibration = static_cast<int>(m_calibration_adjustment->get_value());

        std::cout << "提交标定值 - 电机: " << m_current_motor->name
                  << ", 标定值: " << calibration << std::endl;

        // TODO: 在这里添加实际的标定值设置逻辑
    }
}
