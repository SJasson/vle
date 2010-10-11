/*
 * @file apps/eov/OptionGroup.hpp
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


#ifndef EOV_OPTIONGROUP_HPP
#define EOV_OPTIONGROUP_HPP

#include <glibmm/optioncontext.h>

namespace vle { namespace eov {

    /**
     * @brief Manage the command option, manager, simulator and justrun
     * simulator.
     */
    class CommandOptionGroup : public Glib::OptionGroup
    {
    public:
        CommandOptionGroup();

        virtual ~CommandOptionGroup() { }

        inline int port() const { return mPort; }

        inline bool infos() const { return mInfos; }

        inline bool version() const { return mVersion; }

        inline int verbose() const { return mVerbose; }

        /**
         * @brief Check if only on mode is active.
         *
         * @throw Exception::Internal two or three modes are actives.
         */
        virtual void check();

    private:
        int     mPort;
        bool    mInfos;
        bool    mVersion;
        int     mVerbose;
    };

}} // namespace vle eov

#endif
