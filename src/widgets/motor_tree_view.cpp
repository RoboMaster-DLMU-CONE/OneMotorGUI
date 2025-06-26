#include "widgets/motor_tree_view.hpp"

namespace OneMotorGUI
{
    MotorTreeView::MotorTreeView()
    {
        set_vexpand(true);
        set_hexpand(true);

        setupTreeModel();

        // 设置选择模式
        m_selection = get_selection();
        m_selection->set_mode(Gtk::SelectionMode::SINGLE);
        m_selection->signal_changed().connect(
            sigc::mem_fun(*this, &MotorTreeView::onSelectionChanged));
        append_column("电机", m_columns.col_name);
        expand_all();
    }

    void MotorTreeView::setupTreeModel()
    {
        m_tree_store = Gtk::TreeStore::create(m_columns);
        set_model(m_tree_store);

        // 获取所有品牌
        auto brands = MotorDatabase::getBrands();

        for (const auto &brand : brands)
        {
            // 创建品牌节点
            auto brand_iter = m_tree_store->append();
            auto brand_row = *brand_iter;
            brand_row[m_columns.col_name] = brand;
            brand_row[m_columns.col_motor_info] = nullptr; // 品牌节点没有电机信息

            // 添加该品牌下的电机
            auto motors = MotorDatabase::getMotorsByBrand(brand);
            for (const auto &motor : motors)
            {
                auto motor_iter = m_tree_store->append(brand_row.children());
                auto motor_row = *motor_iter;
                motor_row[m_columns.col_name] = motor->name;
                motor_row[m_columns.col_motor_info] = motor;
            }
        }
    }

    void MotorTreeView::onSelectionChanged()
    {
        if (const auto selection = get_selection())
        {
            if (const auto iter = selection->get_selected())
            {
                auto row = *iter;
                if (const std::shared_ptr<MotorInfo> motor_info = row[m_columns.col_motor_info])
                {
                    // 这是一个电机节点
                    signal_motor_selected.emit(motor_info);
                }
            }
        }
    }
}
