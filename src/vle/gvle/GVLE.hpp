/**
 * @file vle/gvle/GVLE.hpp
 * @author The VLE Development Team
 */

/*
 * VLE Environment - the multimodeling and simulation environment
 * This file is a part of the VLE environment (http://vle.univ-littoral.fr)
 * Copyright (C) 2003 - 2008 The VLE Development Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef GUI_GVLE_HH
#define GUI_GVLE_HH

#include <vle/gvle/ExperimentBox.hpp>
#include <vle/gvle/HelpBox.hpp>
#include <vle/gvle/LaunchSimulationBox.hpp>
#include <vle/gvle/ObserverPlugin.hpp>
#include <vle/gvle/ParameterExecutionBox.hpp>
#include <vle/gvle/Plugin.hpp>
#include <vle/gvle/ProjectBox.hpp>
#include <vle/value/Value.hpp>
#include <vle/gvle/ComboboxString.hpp>
#include <vle/utils/Path.hpp>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/separator.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/table.h>
#include <gtkmm/tooltips.h>
#include <libglademm/xml.h>
#include <string>
#include <list>
#include <map>


namespace vle
{
namespace gvle {

class GVLEMenu;
class Modeling;
class PluginTable;

/**
 * @brief GVLE is a Gtk::Window use to build the main window with all button
 * to control project.
 */
class GVLE : public Gtk::Window
{
public:
    typedef std::multimap < std::string, std::string > MapCategory;
    typedef std::multimap < std::string, std::string > MapObserverCategory;
    typedef std::map < std::string, Plugin * > MapPlugin;
    typedef std::map < std::string, ObserverPlugin * > MapObserverPlugin;

    /** list off all available buttons. */
    enum ButtonType { POINTER, ADDMODEL, ADDLINK, ADDCOUPLED, DELETE, ZOOM,
                      PLUGINMODEL, GRID, QUESTION
                    };

    static const int overview_max = 60;

    /**
     * Create a new window GVLE, if parameter filename is not empty, try to
     * load the vpz file else, new document is load.
     *
     * @param filename vpz filename to load.
     */
    GVLE(const std::string& filename = std::string());

    /**
     * Delete all plugin and Modeling document class.
     *
     */
    virtual ~GVLE();

    /**
     * return the current selectionned button.
     *
     * @return ButtonType of current selected button.
     */
    inline ButtonType getCurrentButton() const {
        return m_currentButton;
    }

    /*********************************************************************
     *
     * PLUGIN
     *
     *********************************************************************/

    /**
     * recursive show plugin path and all file.
     *
     * @param path load plugin file from this directory
     */
    //void loadPlugins(const std::string& path);

    /**
     * recursive show observer plugin path and all file.
     *
     * @param path load observer plugin file from this directory
     */
    //void loadObserverPlugins(const std::string& path);

    /**
     * Open plugin, add icons and button into button group.
     *
     * @param name filename of plugin file to load
     */
    //Plugin* loadPlugin(const std::string& name);

    /**
     * Open observer plugin.
     *
     * @param name filename of observer plugin file to load
     */
    //ObserverPlugin* loadObserverPlugin(const std::string& name);

    /**
     * return a plugin from plugin list.
     *
     * @param name formalism name of plugin to find
     * @return a ptr to plugin, otherwise 0
     */
    //Plugin* getPlugin(const std::string& name);

    /**
     * return a observer plugin from observer plugin list.
     *
     * @param name observer name of plugin to find
     * @return a ptr to plugin, otherwise 0
     */
    //ObserverPlugin* getObserverPlugin(const std::string& name);

    //const MapObserverPlugin & getObserverPluginList() const
    //  { return m_observerPlugins; }

    /**
     * Delete all plugins and togglebutton loaded.
     *
     */
    //void delPlugins();

    /**
     * When select a plugin button, we change current selection.
     *
     * @param name formalism name (or plugin name) of selected plugin.
     */
    //void onPluginButton(const std::string& name);

    /**
     * Get current plugin name selected in GVLE plugin table.
     *
     * @return current plugin name.
     */
    //inline const std::string& getCurrentPluginName() const
    //{ return m_pluginSelected; }


private:
    /*********************************************************************
     *
     * MAKING GRAPHICS INTERFACE
     *
     *********************************************************************/

    /**
     * Buttons creating and connect events.
     *
     */
    void makeButtons();

    /**
     * Assign to this comboboxstring, all category read from plugins ;
     * Before call this function, comboboxstring default choice is 'All' ;
     * This function connect comboCategory to his function changed.
     */
    void makeComboCategory();

    /**
     * When user select a new category from ComboBoxString comboCategory.
     *
     */
    void on_combocategory_changed();


    /*********************************************************************
     *
     * WHEN SELECT A MENU FROM GVLEMenu
     *
     *********************************************************************/


    /**
     * When select arrow button.
     *
     */
    void onArrow();

    /**
     * When select atomic models button.
     *
     */
    void onAddModels();

    /**
     * When select connection button.
     *
     */
    void onAddLinks();

    /**
     * When select coupled button.
     *
     */
    void onAddCoupled();

    /**
     * When select delete button.
     *
     */
    void onDelete();

    /**
     * When select zoom button.
     *
     */
    void onZoom();

    /**
     * When select question button.
     *
     */
    void onQuestion();


    /*********************************************************************
     *
     * MENU FILE
     *
     *********************************************************************/


public:
    /**
     * When click on new menu.
     *
     */
    void onMenuNew();

    /**
     * When click on load menu.
     *
     */
    void onMenuLoad();

    /**
     * When click on save menu.
     *
     */
    void onMenuSave();

    /**
     * When click on save as menu.
     *
     */
    void onMenuSaveAs();


    /********************************************************************
     *
     * MENU SHOW
     *
     ********************************************************************/


    /**
     * When click on Show Model TreeView menu.
     *
     */
    void onShowModelTreeView();


    /**
     * When click on Show deconnected models Treeview menu.
     *
     */
    void onShowClassModelTreeView();

    /**
     * @brief Delete all gvle::View from the gvle::Modeling object.
     */
    void onCloseAllViews();

    /**
     * @brief Iconify all gvle::View from the gvle::Modeling object.
     */
    void onIconifyAllViews();

    /**
     * @brief Deiconify all gvle::View from the gvle::Modeling object.
     */
    void onDeiconifyAllViews();

    /*********************************************************************
     *
     * MENU EXECUTION
     *
     *********************************************************************/


    /**
     * When click on Execution menu.
     *
     */
    void onSimulationBox();

    /**
     * When click on parameter execution menu box.
     *
     */
    void onParameterExecutionBox();

    /**
     * When click on experiment box to define init, state etc.
     *
     */
    void onExperimentsBox();

    /**
     * When click on hosts box to define simulation hosts.
     *
     */
    void onHostsBox();

    /**
       * When click on Project box to define the VLE project data.
       *
       */
    void onProjectBox();

    /*********************************************************************
       *
       * MENU HELP
       *
       *********************************************************************/


    /**
     * on menu HelpBox click.
     *
     */
    void onHelpBox();

    /**
     * on menu About click.
     *
     */
    void onShowAbout();

private:
    Gtk::VBox                       m_vbox;
    GVLEMenu*                       m_menu;
    Gtk::HSeparator                 m_separator;
    Gtk::Table                      m_buttons;
    //widgets::ComboBoxString         m_comboCategory;
    //Gtk::ScrolledWindow             m_scrolledPlugins;
    //PluginTable*                    m_pluginTable;
    //Gtk::Label                      m_labelName;
    Gtk::RadioButton::Group         m_buttonGroup;
    Gtk::RadioButton                m_arrow;
    Gtk::RadioButton                m_addModels;
    Gtk::RadioButton                m_addLinks;
    Gtk::RadioButton                m_addCoupled;
    Gtk::RadioButton                m_delete;
    Gtk::RadioButton                m_zoom;
    Gtk::RadioButton                m_question;
    Gtk::Tooltips                   m_tooltips;
    Gtk::Statusbar                  m_status;
    Modeling*                       m_modeling;

    MapCategory                     m_category;
    MapObserverCategory             m_observerCategory;
    MapPlugin                       m_plugins;
    MapObserverPlugin               m_observerPlugins;

    ButtonType                      m_currentButton;
    HelpBox*                        m_helpbox;

    //std::string                     m_pluginSelected;

    Glib::RefPtr < Gnome::Glade::Xml >  mRefXML;

    //Menu
    ExperimentBox*        mExpBox;
    ProjectBox*           mProjectBox;
    LaunchSimulationBox*  mSimulationBox;
};

std::string valuetype_to_string(value::ValueBase::type type);

}
} // namespace vle gvle

#endif