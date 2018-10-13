#pragma once

#include <memory>
#include <string>

#include "gtkmm-3.0/gtkmm.h"

#include "gui/Builder.hpp"

namespace depot
{
class Container;
class AddContainerProcedure;
class HierarchicalItemsContainer;
}

namespace gui
{
namespace widget
{
class NewContainerDialog : public Gtk::Dialog
{
public:
    NewContainerDialog(BaseObjectType*, Glib::RefPtr<Gtk::Builder>&);

    void setParentContainer(const std::string& = "");
    void setProcedure(std::unique_ptr<depot::AddContainerProcedure> );
    void run(const Container*);
    std::weak_ptr<depot::HierarchicalItemsContainer> run();

private:
    void cleanAndHide();

    Builder builder;
    std::unique_ptr<Gtk::Button> okButton;
    std::unique_ptr<Gtk::Button> cancelButton;
    std::unique_ptr<Gtk::Entry> nameEntry;
    std::unique_ptr<Gtk::Label> parentLabel;
    std::unique_ptr<depot::AddContainerProcedure> procedure;
    std::weak_ptr<depot::HierarchicalItemsContainer> newContainer;
};
}
}
