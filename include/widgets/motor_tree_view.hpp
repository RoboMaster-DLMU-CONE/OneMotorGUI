#ifndef MOTOR_TREE_VIEW_HPP
#define MOTOR_TREE_VIEW_HPP

#include <gtkmm.h>
#include <sigc++/sigc++.h>
#include "models/motor_info.hpp"

namespace OneMotorGUI
{
    class MotorTreeView : public Gtk::TreeView
    {
    public:
        MotorTreeView();

        // 当选择电机时发出的信号
        sigc::signal<void(std::shared_ptr<MotorInfo>)> signal_motor_selected;

    private:
        void setupTreeModel();
        void onSelectionChanged();

        // TreeModel相关
        class ModelColumns : public Gtk::TreeModel::ColumnRecord
        {
        public:
            ModelColumns()
            {
                add(col_name);
                add(col_motor_info);
            }

            Gtk::TreeModelColumn<Glib::ustring> col_name;
            Gtk::TreeModelColumn<std::shared_ptr<MotorInfo>> col_motor_info;
        };

        ModelColumns m_columns;
        Glib::RefPtr<Gtk::TreeStore> m_tree_store;
        Glib::RefPtr<Gtk::TreeSelection> m_selection;
    };
}

#endif // MOTOR_TREE_VIEW_HPP
