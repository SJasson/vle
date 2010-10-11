/*
 * @file vle/oov/SimpleFile.hpp
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


#ifndef VLE_OOV_SIMPLEFILE_HPP
#define VLE_OOV_SIMPLEFILE_HPP

#include <vle/oov/DllDefines.hpp>
#include <vle/oov/Plugin.hpp>
#include <fstream>
#include <map>
#include <vector>

namespace vle { namespace oov { namespace plugin {

    /**
     * @brief SimpleFile is a virtual class for the csv, text, and rdata
     * plug-in. When simulation is running, SimpleFile writes information into a
     * tempory file localized into the local directory or in the directory
     * specified in the parameter trame.
     * The SimpleFile accepts a value::Map in parameter with two keys:
     * - out: define the type of output. By default, it uses and file. But if
     *   the value equal 'out', it copy result into the standard output and if
     *   the value equal 'error', it copy result into the stardard error.
     * - locale: define the locale of the stream. By default, the locale 'C' is
     *   used, ie. the C Ansi locale. If the value equal 'user' then the locale
     *   is attached to the locale of the user (run locale). Otherwise, the user
     *   can use all locale defines in the environment. Use the 'locale -a'
     *   command to show all locale of your system.
     * <map>
     *  <key name="output">
     *   <string>out</string> <!-- or 'error' -->
     *  </key>
     *  <key name="locale">
     *   <string>fr_FR.UTF-8</string> <!-- 'user', 'C', '' etc.
     *  </key>
     * </map>
     */
    class VLE_OOV_EXPORT SimpleFile : public Plugin
    {
    public:
        SimpleFile(const std::string& location);

        virtual ~SimpleFile();

        ///
        //// the interface provided by oov::Plugin.
        ///

        virtual void onParameter(const std::string& plugin,
                                 const std::string& location,
                                 const std::string& file,
                                 value::Value* parameters,
                                 const double& time);

        virtual void onNewObservable(const std::string& simulator,
                                     const std::string& parent,
                                     const std::string& portname,
                                     const std::string& view,
                                     const double& time);

        virtual void onDelObservable(const std::string& simulator,
                                     const std::string& parent,
                                     const std::string& portname,
                                     const std::string& view,
                                     const double& time);

        virtual void onValue(const std::string& simulator,
                             const std::string& parent,
                             const std::string& port,
                             const std::string& view,
                             const double& time,
                             value::Value* value);

        virtual void close(const double& time);

        ///
        //// the new interface for derived oov::SimpleFile plug-ins.
        ///

        virtual std::string extension() const = 0;

        virtual void writeSeparator(std::ostream& out) = 0;

        virtual void writeHead(std::ostream& out,
                               const std::vector < std::string >& heads) = 0;

    private:
        /** Define a dictionary (model's name, index) */
        typedef std::map < std::string, int > Columns;

        /** Define the buffer of values. */
        typedef std::vector < value::Value* > Line;

        /** Define the buffer for valid values (model observed). */
        typedef std::vector < bool > ValidElement;

        enum OutputType {
            FILE, /*!< classical file stream (std::ofstream). */
            STANDARD_OUT, /*!< use the standard output (std::cout). */
            STANDARD_ERROR /*!< use the error output (std::cerr). */
        };

        Columns         m_columns;
        Line            m_buffer;
        ValidElement    m_valid;
        double          m_time;
        std::ofstream   m_file;
        std::string     m_filename;
        std::string     m_filenametmp;
        bool            m_isstart;
        bool            m_havefirstevent;
        bool            m_julian;
        OutputType      m_type;

        void flush(double trame_time);

        void finalFlush(double trame_time);

        void copyToFile(const std::string& filename,
                        const std::vector < std::string >& array);

        void copyToStream(std::ostream& out,
                          const std::vector < std::string >& array);

        /**
         * @brief This function is use to build uniq name to each row of the
         * text output.
         * @param parent the hierarchy of coupled model.
         * @param simulator the name of the devs::Model.
         * @param port the name of the state port of the devs::Model.
         * @return a representation of the uniq name.
         */
        std::string buildname(const std::string& parent,
                              const std::string& simulator,
                              const std::string& port);
    };

}}} // namespace vle oov plugin

#endif
