/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2003-2012 Gauthier Quesnel <quesnel@users.sourceforge.net>
 * Copyright (c) 2003-2012 ULCO http://www.univ-littoral.fr
 * Copyright (c) 2007-2012 INRA http://www.inra.fr
 *
 * See the AUTHORS or Authors.txt file for copyright owners and
 * contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef VLE_OOV_PLUGIN_HPP
#define VLE_OOV_PLUGIN_HPP

#include <vle/DllDefines.hpp>
#include <vle/value/Set.hpp>
#include <vle/value/Matrix.hpp>
#include <vle/version.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

#define DECLARE_OOV_PLUGIN(x)                           \
    extern "C" {                                        \
        VLE_API vle::oov::Plugin*                       \
        vle_make_new_oov(const std::string& location)   \
        {                                               \
            return new x(location);                     \
        }                                               \
                                                        \
        VLE_API void                                    \
        vle_api_level(vle::uint32_t* major,             \
                      vle::uint32_t* minor,             \
                      vle::uint32_t* patch)             \
        {                                               \
            *major = VLE_MAJOR_VERSION;                 \
            *minor = VLE_MINOR_VERSION;                 \
            *patch = VLE_PATCH_VERSION;                 \
        }                                               \
    }

namespace vle { namespace oov {

/**
 * \c vle::oov::Plugin permit to build output plug-ins.
 *
 * The vle::oov::Plugin is a class to develop output steam plugins.
 * For instance, an output stream text (for gnuplot or R) and cairo (for
 * building vectorial image or bitmap). This class define a plugin.
 * @code
 * class Csv : public Plugin
 * {
 *   //
 *   //
 *   // fill the virtual functions.
 *   //
 *   //
 * };
 *
 * DECLARE_OOV_PLUGIN(Csv);
 * @endcode
 */
class VLE_API Plugin
{
public:
    /**
     * Default constructor of the Plugin.
     *
     * @param location this string represents the name of the default
     * directory for a \c devs::LocalStreamWriter or a
     * host:port:directory for a \c devs::DistantStreamWriter.
     */
    Plugin(const std::string& location)
        : m_location(location)
    {
    }

    /**
     * Nothing to delete.
     */
    virtual ~Plugin()
    {
    }

    /**
     * Return a pointer to the \c value::Matrix.
     *
     * If the plug-in does not manage \c value::Matrix, this function
     * returns NULL otherwise, this function return the \c
     * value::Matrix manager by the plug-in.
     *
     * @attention You are in charge of freeing the value::Matrix after
     * the end of the simulation.
     */
    virtual value::Matrix * matrix() const
    {
        return 0;
    }

    /**
     * Get the name of the Plugin class.
     *
     * @return The name of the plugin class.
     */
    virtual std::string name() const
    {
        return std::string();
    }

    ///
    ////
    ///

    /**
     * By default, a plugin is not a cairo plugin.
     *
     * @return false.
     */
    virtual bool isCairo() const
    {
        return false;
    }

    /**
     * Call to initialise plugin.
     *
     * Just before the Plugin constructor. This function is used to
     * initialise the Plugin with parameter provided by the
     * devs::StreamWriter.
     */
    virtual void onParameter(const std::string& plugin,
                             const std::string& location,
                             const std::string& file,
                             value::Value* parameters,
                             const double& time) = 0;

    /**
     * Call when a new observable (the devs::Simulator and port name)
     * is attached to a view.
     */
    virtual void onNewObservable(const std::string& simulator,
                                 const std::string& parent,
                                 const std::string& port,
                                 const std::string& view,
                                 const double& time) = 0;

    /**
     * Call whe a observable (the devs::Simulator and port name) is
     * deleted from a view.
     */
    virtual void onDelObservable(const std::string& simulator,
                                 const std::string& parent,
                                 const std::string& port,
                                 const std::string& view,
                                 const double& time) = 0;

    /**
     * Call when an external event is send to the view.
     */
    virtual void onValue(const std::string& simulator,
                         const std::string& parent,
                         const std::string& port,
                         const std::string& view,
                         const double& time,
                         value::Value* value) = 0;

    /**
     * Call when the simulation is finished.
     */
    virtual void close(const double& time) = 0;

    ///
    ////
    ///

    /**
     * Get the location provide at the constructor of this Plugin.
     * @return A constant string to the location provided a the Plugin
     * constructor. This string represent a directory for a
     * devs::LocalStreamWriter or a host:port:directory for a
     * devs::DistantStreamWriter.
     */
    inline const std::string& location() const
    { return m_location; }

private:
    std::string         m_location;
};

/**
 * This typedef is used by the StreamReader build factory to
 * automatically destroy plugin at the end of the simulation.
 */
typedef boost::shared_ptr < Plugin > PluginPtr;

/**
 * Define the pointer to function of the oov::Plugin plug-in.
 * @param The output location of the oov::Plugin.
 * @return A pointer to the oov::Plugin newly build.
 */
typedef Plugin* (*OovPluginSlot)(const std::string&);

/**
 * This typedef is used to defined the dictionnary of key view name
 * and oov::PluginPtr.
 */
typedef std::map < std::string, PluginPtr > PluginViewList;

}} // namespace vle oov

#endif
