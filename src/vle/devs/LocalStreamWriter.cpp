/*
 * @file vle/devs/LocalStreamWriter.cpp
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


#include <vle/devs/LocalStreamWriter.hpp>
#include <vle/devs/Simulator.hpp>



namespace vle { namespace devs {

void LocalStreamWriter::open(const std::string& plugin,
                             const std::string& package,
                             const std::string& location,
                             const std::string& file,
                             value::Value* parameters,
                             const devs::Time& time)
{
    m_reader.onParameter(plugin, package, location, file, parameters,
                         time.getValue());
}

void LocalStreamWriter::processNewObservable(Simulator* simulator,
                                             const std::string& portname,
                                             const devs::Time& time,
                                             const std::string& view)
{
    m_reader.onNewObservable(simulator->getName(), simulator->getParent(),
                             portname, view, time.getValue());
}

void LocalStreamWriter::processRemoveObservable(Simulator* simulator,
                                                const std::string& portname,
                                                const devs::Time& time,
                                                const std::string& view)
{
    m_reader.onDelObservable(simulator->getName(), simulator->getParent(),
                             portname, view, time.getValue());
}

void LocalStreamWriter::process(ObservationEvent& event)
{
    value::Value* val = 0;
    if (event.haveAttributes()) {
        value::Map::iterator
            it(event.getAttributes().value().find(event.getPortName()));
        if (it != event.getAttributes().value().end()) {
            val = it->second;
            it->second = 0;
            event.getAttributes().value().erase(it);
        }
    }

    m_reader.onValue(event.getModel()->getName(),
                     event.getModel()->getParent(),
                     event.getPortName(),
                     event.getViewName(),
                     event.getTime().getValue(),
                     val);
}

oov::PluginPtr LocalStreamWriter::close(const devs::Time& time)
{
    m_reader.onClose(time.getValue());
    return m_reader.plugin();
}

oov::PluginPtr LocalStreamWriter::refreshPlugin()
{
    return m_reader.plugin();
}

}} // namespace vle devs
