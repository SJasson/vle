/*
 * @file vle/eov/plugins/default/GtkDefault.cpp
 *
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems
 * http://www.vle-project.org
 *
 * Copyright (c) 2003-2007 Gauthier Quesnel <quesnel@users.sourceforge.net>
 * Copyright (c) 2003-2010 ULCO http://www.univ-littoral.fr
 * Copyright (c) 2007-2010 INRA http://www.inra.fr
 *
 * See the AUTHORS or Authors.txt file for copyright owners and contributors
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


#include <vle/eov/plugins/default/GtkDefault.hpp>

namespace vle { namespace eov {

GtkDefault::GtkDefault(oov::CairoPluginPtr cairoplugin,
                       NetStreamReader* net) :
    Plugin(cairoplugin, net),
    m_vbox(false, 5),
    m_label("Default plugin")
{
    m_vbox.pack_start(m_da, true, true, 0);
    m_vbox.pack_start(m_label, false, false, 0);
}

GtkDefault::~GtkDefault()
{
}

Glib::RefPtr < Gdk::Window > GtkDefault::drawingSurface()
{
    return m_da.get_window();
}

Gtk::Widget& GtkDefault::drawingWidget()
{
    return m_da;
}

Gtk::Container& GtkDefault::widget()
{
    return m_vbox;
}

int GtkDefault::width()
{
    int width, height;
    drawingSurface()->get_size(width, height);
    return width;
}

int GtkDefault::height()
{
    int width, height;
    drawingSurface()->get_size(width, height);
    return height;
}

}} // namespace vle::eov
