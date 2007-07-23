/** 
 * @file NetStreamWriter.hpp
 * @brief 
 * @author The vle Development Team
 * @date 2007-07-16
 */

/*
 * Copyright (C) 2007 - The vle Development Team
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef VLE_DEVS_NETSTREAMWRITER_HPP
#define VLE_DEVS_NETSTREAMWRITER_HPP

#include <vle/devs/StreamWriter.hpp>
#include <vle/utils/Socket.hpp>



namespace vle { namespace devs {

    class NetStreamWriter : public StreamWriter
    {
    public:
        NetStreamWriter();

        virtual ~NetStreamWriter();

        virtual void open(
                    const std::string& outputPlugin,
                    const std::string& outputType,
                    const std::string& outputLocation,
                    const std::string& parameters);

        virtual void processNewObservable(
                    const Time& time,
                    Simulator* simulator,
                    const std::string& portname,
                    const std::string& view);

        virtual void processRemoveObservable(
                    const Time& time,
                    Simulator* simulator,
                    const std::string& portname,
                    const std::string& view);

        virtual void process(
                    const StateEvent& event);

        virtual void close();


    private:
        utils::net::Client*     m_client;
    };

}} // namespace vle devs

#endif