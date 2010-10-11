/*
 * @file examples/gens/Beep.cpp
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


#include <Beep.hpp>

namespace vle { namespace examples { namespace gens {

devs::Time Beep::init(const devs::Time& /* time */)
{
    return 0.0;
}

devs::Time Beep::timeAdvance() const
{
    return 1.0;
}

void Beep::output(const devs::Time& /* time */,
                  devs::ExternalEventList& output) const
{
    output.addEvent(buildEvent("out"));
}

}}} // namespace vle examples gens
