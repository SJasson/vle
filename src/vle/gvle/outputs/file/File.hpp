/**
 * @file vle/gvle/outputs/file/File.hpp
 * @author The VLE Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * VLE Environment - the multimodeling and simulation environment
 * This file is a part of the VLE environment
 * http://www.vle-project.org
 *
 * Copyright (C) 2007-2010 INRA http://www.inra.fr
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


#ifndef VLE_GVLE_OUTPUTS_FILE_HPP
#define VLE_GVLE_OUTPUTS_FILE_HPP

#include <vle/gvle/OutputPlugin.hpp>
#include <gtkmm/dialog.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/comboboxentrytext.h>

namespace vle { namespace gvle { namespace outputs {

class File : public OutputPlugin
{
public:
    File(const std::string& name);

    virtual ~File();

    virtual bool start(vpz::Output& output, vpz::View& view);

private:
    Gtk::Dialog* mDialog;
    Gtk::RadioButton* mRadioFile;
    Gtk::RadioButton* mRadioStdOut;
    Gtk::RadioButton* mRadioErrOut;
    Gtk::ComboBoxText* mComboType;
    Gtk::ComboBoxEntryText* mComboFile;
    Gtk::HBox* mHBoxFile;
    Gtk::HBox* mHBoxLocale;

    void init(vpz::Output& output);
    void assign(vpz::Output& output);
};

}}} // namespace vle gvle outputs

#endif
