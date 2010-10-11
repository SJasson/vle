/*
 * @file vle/oov/plugins/Rdata.cpp
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


#include <vle/oov/plugins/Rdata.hpp>

namespace vle { namespace oov { namespace plugin {

Rdata::Rdata(const std::string& location) :
    SimpleFile(location)
{
}

Rdata::~Rdata()
{
}

std::string Rdata::extension() const
{
    return ".rdata";
}

void Rdata::writeSeparator(std::ostream& out)
{
    out << '\t';
}

void Rdata::writeHead(std::ostream& out,
                      const std::vector < std::string >& heads)
{
    for (std::vector < std::string >::const_iterator it = heads.begin();
         it != heads.end(); ++it) {
        out << "\"" << *it << "\"\t";
    }
    out << '\n';
}

}}} // namespace vle oov plugin
