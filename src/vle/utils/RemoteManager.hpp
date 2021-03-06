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


#ifndef VLE_UTILS_REMOTEMANAGER_HPP
#define VLE_UTILS_REMOTEMANAGER_HPP

#include <vle/DllDefines.hpp>
#include <vle/utils/Types.hpp>
#include <iosfwd>
#include <string>
#include <vector>


namespace vle { namespace utils {

/**
 * @e PackageOperatorType defines the relation of an identified with
 * package operator identified. Eg. the @c glue-1.0 package depends of
 * vle.extension.difference-equation >= 1.0.x and
 * vle.extension.difference-equation < 1.1.0
 */
enum PackageOperatorType
{
    PACKAGE_OPERATOR_EQUAL,     /**< = */
    PACKAGE_OPERATOR_LESS,      /**< < */
    PACKAGE_OPERATOR_LESS_OR_EQUAL, /**< <= */
    PACKAGE_OPERATOR_GREATER,   /**< > */
    PACKAGE_OPERATOR_GREATER_OR_EQUAL /**< >= */
};

/**
 * @e PackageOperatorId is defines to package in the @c depends, @c
 * buildDepends and @c conflicts lists. A @e PackageOperatorId have a
 * name, an operator (equal, less, less or equal, greater or greater
 * or equal) and a version numbers.
 */
struct PackageLinkId
{
    std::string name;           /**< Name of the package. */
    int32_t major;              /**< -1 defines undefined major
                                 * version. */
    int32_t minor;              /**< -1 defines undefined minor
                                 * version. */
    int32_t patch;              /**< -1 defines undefined patch
                                 * version. */
    PackageOperatorType op;     /**< The relation between the
                                   * identified package and this
                                   * identified operator package. */
};

typedef std::vector < PackageLinkId > PackagesLinkId;

typedef std::string Tag;

typedef std::vector < Tag > Tags;

struct PackageId
{
    uint64_t size;
    std::string name;
    std::string distribution;
    std::string maintainer;
    std::string description;
    std::string url;
    std::string md5sum;
    Tags tags;
    PackagesLinkId depends;
    PackagesLinkId builddepends;
    PackagesLinkId conflicts;
    int32_t major;
    int32_t minor;
    int32_t patch;
};

/**
 * \c RemoteManagerActions is available action for the \c RemoteManager class.
 *
 * Use the \c RemoteManagerActions in the \c start function of the \c
 * RemoteManager class to update database, install, search or show packages.
 */
enum RemoteManagerActions
{
    REMOTE_MANAGER_UPDATE,      /**< vle --remote update. */
    REMOTE_MANAGER_SOURCE,      /**< vle --remote source glue. */
    REMOTE_MANAGER_INSTALL,     /**< vle --remove install glue. */
    REMOTE_MANAGER_LOCAL_SEARCH, /**< vle --remote localsearch '.*' */
    REMOTE_MANAGER_SEARCH,      /**< vle --remote search '*lu*'. */
    REMOTE_MANAGER_SHOW         /**< vle --remote show glue. */
};

VLE_API std::ostream& operator<<(std::ostream& os, const PackageId& b);

typedef std::vector < PackageId > Packages;

/**
 * \c RemoteManager allow to manager ftp/http repositories.
 *
 * A class to access to the  \c RemoteManager repositories. By default, a file
 * in \c VLE_HOME/packages stores the packages availables. Each update will
 * rewrite this file with newly available packages.
 *
 * This class uses the \c libxml2 \c nanohttp function to download resources
 * from the http protocol. \c RemoteManager is thread-safe and multi-threaded
 * class. \c RemoteManager can not be copied or assigned to another \c
 * RemoteManager.
 *
 * @code
 * utils::RemoteManager mgr;
 *
 * mgr.start(REMOTE_MANAGER_UPDATE, std::string(), &std::cout);
 * mgr.join();
 *
 * mgr.start(REMOTE_MANAGER_INSTALL, "glue-1.0", &std::cout);
 * @endcode
 */
class VLE_API RemoteManager
{
public:
    /**
     * Build a new RemoteManager object by reading packages list from
     * the standard location:
     * - Read the \c VLE_HOME/vle.conf file to get the list of remote host.
     * - Read the \c VLE_HOME/package file to get the list of available package.
     */
    RemoteManager();

    /**
     * If an action is running, \c join it otherwise do nothing.
     */
    ~RemoteManager();

    /**
     * Start a process.
     *
     * If no thread are alive, a thread is started to perform the \c action.
     *
     * @param[in] action The type of the action to start.
     * @param[in] arg The argument of the action.
     * @param[out] os The output stream of the action.
     */
    void start(RemoteManagerActions action,
               const std::string&   arg,
               std::ostream*        os);

    /**
     * Join the current thread: wait until the thread stop.
     */
    void join();

    /**
     * Try to stop the current thread.
     *
     * This function does not try to \c join the thread. You need to use \c
     * join() to ensure the end of the thread.
     *
     * @code
     * utils::RemoteManager mgr;
     *
     * mgr.start(REMOTE_MANAGER_INSTALL, "glue-1.0", &std::cout);
     *
     * // some source code
     * // [...]
     *
     * mgr.stop(); // stop the action if it is possible.
     * mgr.join(); // join the current action.
     *
     * @endcode
     */
    void stop();

    void getResult(Packages *out);

    /**
     * Return the path @e "$VLE_HOME/local.pkg".
     *
     *
     * @return The path @e "$VLE_HOME/local.pkg."
     */
    static std::string getLocalPackageFilename();

    /**
     * Return the path @e "$VLE_HOME/remote.pkg".
     *
     *
     * @return The path @e "$VLE_HOME/remote.pkg."
     */
    static std::string getRemotePackageFilename();

private:
    /**
     * Uncopyable class.
     */
    RemoteManager(const RemoteManager& RemoteManager);

    /**
     * Uncopyable class.
     */
    RemoteManager& operator=(const RemoteManager& RemoteManager);

    class Pimpl;
    Pimpl* mPimpl;
};

}} // namespace vle utils

#endif
