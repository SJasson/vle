/**
 * @file vle/gvle/DynamicBox.hpp
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


#ifndef VLE_GVLE_DYNAMICBOX_HPP
#define VLE_GVLE_DYNAMICBOX_HPP

#include <libglademm.h>
#include <gtkmm/dialog.h>
#include <gtkmm/box.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/entry.h>
#include <vle/vpz/Dynamic.hpp>

namespace vle { namespace gvle {

    class DynamicBox : public sigc::trackable
    {
    public:
        DynamicBox(Glib::RefPtr < Gnome::Glade::Xml > xml);

        ~DynamicBox();

        void show(vpz::Dynamic* dyn);
    private:
        Glib::RefPtr < Gnome::Glade::Xml >  mXml;
        Gtk::Dialog*                        mDialog;
        Gtk::ComboBoxText*                  mCombo;
        Gtk::Entry*                         mLocationHost;
        Gtk::SpinButton*                    mLocationPort;
        Gtk::Entry*                         mModel;
        Gtk::ComboBoxText*                  mLanguage;
        Gtk::Button*                        mButtonApply;
        Gtk::Button*                        mButtonCancel;
        vpz::Dynamic*                       mDyn;

        void makeCombo();
        void on_apply();
        void on_cancel();
    };

}} // namespace vle gvle

#endif