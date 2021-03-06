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


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utils_library_test
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/config.hpp>
#include <boost/filesystem.hpp>
#include <boost/assign.hpp>
#include <stdexcept>
#include <limits>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <iterator>
#include <iostream>
#include <sstream>
#include <numeric>
#include <vle/utils/i18n.hpp>
#include <vle/utils/Algo.hpp>
#include <vle/utils/DateTime.hpp>
#include <vle/utils/Package.hpp>
#include <vle/utils/Path.hpp>
#include <vle/utils/Rand.hpp>
#include <vle/utils/Trace.hpp>
#include <vle/utils/Tools.hpp>
#include <vle/utils/RemoteManager.hpp>
#include <vle/vle.hpp>

using namespace vle;

namespace bs = boost::system;
namespace fs = boost::filesystem;

struct F
{
    vle::Init *a;
    std::string oldname;
    std::string newname;

    F() throw() : a(0), oldname()
    {
        bs::error_code ec;

        fs::path tmp = fs::temp_directory_path(ec);
        tmp /= fs::unique_path("%%%%-%%%%-%%%%-%%%%");

        fs::create_directory(tmp, ec);

        if (fs::exists(tmp, ec) && fs::is_directory(tmp, ec)) {
            char *env = ::getenv("VLE_HOME");
            if (env) {
                oldname.assign(env);
            }
        }

        newname = tmp.string();
        ::setenv("VLE_HOME", newname.c_str(), 1);

        a = new vle::Init();
    }

    ~F() throw()
    {
        bs::error_code ec;

        delete a;

        fs::path tmp(newname);
        if (fs::exists(tmp, ec) && fs::is_directory(tmp, ec)) {
            fs::remove_all(tmp, ec); /**< comment this line if you
                                      * want to conserve the temporary
                                      * VLE_HOME directory. */
        }

        if (not oldname.empty()) {
            ::setenv("VLE_HOME", oldname.c_str(), 1);
        }
    }

private:
    F(const F&);
    F& operator=(const F&);
};

BOOST_GLOBAL_FIXTURE(F)

BOOST_AUTO_TEST_CASE(show_path)
{
    using vle::utils::Package;
    using vle::utils::Path;
    using vle::utils::PathList;

    BOOST_REQUIRE_EQUAL((PathList::size_type)0,
                        Path::path().getSimulatorDirs().size());

    Package::package().select("x");
    vle::utils::Package::package().create();
}

BOOST_AUTO_TEST_CASE(show_package)
{
    using vle::utils::Path;
    using vle::utils::PathList;
    using vle::utils::Package;

    std::ostringstream out, err;

    vle::utils::Package::package().select("tmp");
    vle::utils::Package::package().create();

    std::cout << "Packages:\n";
    PathList lst = Path::path().getInstalledPackages();
    std::copy(lst.begin(), lst.end(), std::ostream_iterator < std::string >(
                  std::cout, "\n"));

    std::cout << "Vpz:\n";
    PathList vpz = Path::path().getInstalledExperiments();
    std::copy(vpz.begin(), vpz.end(), std::ostream_iterator < std::string >(
                  std::cout, "\n"));
}

BOOST_AUTO_TEST_CASE(remote_package_check_2_package_tmp_and_x)
{
    utils::RemoteManager rmt;
    utils::Packages results;

    {
        rmt.start(utils::REMOTE_MANAGER_SEARCH, ".*", NULL);
        rmt.join();
        rmt.getResult(&results);
        BOOST_REQUIRE_EQUAL(results.empty(), true);
        BOOST_REQUIRE_EQUAL(results.size(), 0u);
    }

    {
        rmt.start(utils::REMOTE_MANAGER_LOCAL_SEARCH, ".*", NULL);
        rmt.join();
        rmt.getResult(&results);
        BOOST_REQUIRE_EQUAL(results.empty(), false);
        BOOST_REQUIRE_EQUAL(results.size(), 2u);

        BOOST_REQUIRE(results[0].name == "MyProject");
    }
}

BOOST_AUTO_TEST_CASE(remote_package_local_remote)
{
    using namespace boost::assign;

    utils::PackageId pkg;

    pkg.size = 0;
    pkg.name = "name";
    pkg.distribution = "distribution";
    pkg.maintainer = "me";
    pkg.description = "too good";
    pkg.url = "http://www.vle-project.org";
    pkg.md5sum = "1234567890987654321";
    pkg.tags += "a", "b", "c";

    {
        utils::PackageLinkId dep = { "a", 1, 1, 1,
                                     utils::PACKAGE_OPERATOR_EQUAL };
        pkg.depends = utils::PackagesLinkId(1, dep);
    }

    {
        utils::PackageLinkId dep = { "a", 1, 1, 1,
                                     utils::PACKAGE_OPERATOR_EQUAL };
        pkg.builddepends = utils::PackagesLinkId(1, dep);
    }

    {
        utils::PackageLinkId dep = { "a", 1, 1, 1,
                                     utils::PACKAGE_OPERATOR_EQUAL };
        pkg.conflicts = utils::PackagesLinkId(1, dep);
    }

    pkg.major = 1;
    pkg.minor = 2;
    pkg.patch = 3;

     fs::path path = utils::RemoteManager::getRemotePackageFilename();

    {
        std::ofstream ofs(path.string().c_str());
        ofs << pkg << "\n";
    }

    utils::RemoteManager rmt;

    {
        utils::Packages results;
        rmt.start(utils::REMOTE_MANAGER_SEARCH, ".*", NULL);
        rmt.join();
        rmt.getResult(&results);
        BOOST_REQUIRE_EQUAL(results.empty(), false);
        BOOST_REQUIRE_EQUAL(results.size(), 1u);

        BOOST_REQUIRE(results[0].name == "name");
    }

    {
        utils::Packages results;
        rmt.start(utils::REMOTE_MANAGER_LOCAL_SEARCH, ".*", NULL);
        rmt.join();
        rmt.getResult(&results);
        BOOST_REQUIRE_EQUAL(results.empty(), false);
        BOOST_REQUIRE_EQUAL(results.size(), 2u);

        BOOST_REQUIRE(results[0].name == "MyProject");
        BOOST_REQUIRE(results[1].name == "MyProject");
    }
}

BOOST_AUTO_TEST_CASE(remote_package_read_write)
{
    using namespace boost::assign;

    {
        std::ofstream OX("/tmp/X.dat");
        std::ofstream ofs(
            utils::RemoteManager::getRemotePackageFilename().c_str());

        for (int i = 0; i < 10; ++i) {
            utils::PackageId pkg;

            pkg.size = i;
            pkg.name = (fmt("name-%1%") % i).str();
            pkg.distribution = "distribution";
            pkg.maintainer = "me";
            pkg.description = "too good";
            pkg.url = "http://www.vle-project.org";
            pkg.md5sum = "1234567890987654321";
            pkg.tags += "a", "b", "c";

            {
                utils::PackageLinkId dep = { "a", 1, 1, 1,
                                             utils::PACKAGE_OPERATOR_EQUAL };
                pkg.depends = utils::PackagesLinkId(1, dep);
            }

            {
                utils::PackageLinkId dep = { "a", 1, 1, 1,
                                             utils::PACKAGE_OPERATOR_EQUAL };
                pkg.builddepends = utils::PackagesLinkId(1, dep);
            }

            {
                utils::PackageLinkId dep = { "a", 1, 1, 1,
                                             utils::PACKAGE_OPERATOR_EQUAL };
                pkg.conflicts = utils::PackagesLinkId(1, dep);
            }

            pkg.major = 1;
            pkg.minor = 2;
            pkg.patch = 3;

            ofs << pkg;
            OX << pkg;
        }
    }

    {
        utils::RemoteManager rmt;
        rmt.start(utils::REMOTE_MANAGER_SEARCH, ".*", NULL);
        rmt.join();

        utils::Packages results;
        rmt.getResult(&results);

        BOOST_REQUIRE_EQUAL(results.empty(), false);
        BOOST_REQUIRE_EQUAL(results.size(), 10u);
    }

    {
        utils::RemoteManager rmt;
        rmt.start(utils::REMOTE_MANAGER_SEARCH, ".*", NULL);
        rmt.join();

        utils::Packages results;
        rmt.getResult(&results);

        BOOST_REQUIRE_EQUAL(results.empty(), false);
        BOOST_REQUIRE_EQUAL(results.size(), 10u);
    }

    {
        utils::RemoteManager rmt;
        rmt.start(utils::REMOTE_MANAGER_SEARCH, ".*", NULL);
        rmt.join();

        utils::Packages results;
        rmt.getResult(&results);

        BOOST_REQUIRE_EQUAL(results.empty(), false);
        BOOST_REQUIRE_EQUAL(results.size(), 10u);
    }

    vle::utils::RemoteManager rmt;

    std::ostringstream output;
    rmt.start(vle::utils::REMOTE_MANAGER_SHOW, "name-8", &output);
    rmt.join();

    {
        vle::utils::Packages pkgs;
        rmt.getResult(&pkgs);

        BOOST_REQUIRE_EQUAL(pkgs.size(), 1u);

        BOOST_REQUIRE_EQUAL(pkgs[0].name, "name-8");
        BOOST_REQUIRE_EQUAL(pkgs[0].size, 8u);
        BOOST_REQUIRE_EQUAL(pkgs[0].md5sum, "1234567890987654321");
        BOOST_REQUIRE_EQUAL(pkgs[0].url, "http://www.vle-project.org");
        BOOST_REQUIRE_EQUAL(pkgs[0].maintainer, "me");
        BOOST_REQUIRE_EQUAL(pkgs[0].major, 1);
        BOOST_REQUIRE_EQUAL(pkgs[0].minor, 2);
        BOOST_REQUIRE_EQUAL(pkgs[0].patch, 3);
        BOOST_REQUIRE_EQUAL(pkgs[0].description, "too good");
    }

    {
        vle::utils::Packages pkgs;
        rmt.start(vle::utils::REMOTE_MANAGER_SEARCH, "name.*", &output);
        rmt.join();
        rmt.getResult(&pkgs);
        BOOST_REQUIRE_EQUAL(pkgs.size(), 10u);
    }

    {
        vle::utils::Packages pkgs;
        rmt.start(vle::utils::REMOTE_MANAGER_SEARCH, ".*0", &output);
        rmt.join();
        rmt.getResult(&pkgs);
        BOOST_REQUIRE_EQUAL(pkgs.size(), 1u);

    }

    {
        vle::utils::Packages pkgs;
        rmt.start(vle::utils::REMOTE_MANAGER_SEARCH, ".*", &output);
        rmt.join();
        rmt.getResult(&pkgs);
        BOOST_REQUIRE_EQUAL(pkgs.size(), 10u);
    }

    {
        vle::utils::Packages pkgs;
        rmt.start(vle::utils::REMOTE_MANAGER_SEARCH, ".*[1|2]", &output);
        rmt.join();
        rmt.getResult(&pkgs);
        BOOST_REQUIRE_EQUAL(pkgs.size(), 2u);
    }
}

BOOST_AUTO_TEST_CASE(test_compress_filepath)
{
    std::string filepath;
    std::string uniquepath;
    try {
        fs::path unique = fs::unique_path("%%%%-%%%%-%%%%-%%%%");

        vle::utils::Package::package().select(unique.string());
        vle::utils::Package::package().create();

        filepath = vle::utils::Path::path().getPackageDir();
        uniquepath = unique.string();
    } catch (...) {
        BOOST_REQUIRE(false);
    }

    BOOST_REQUIRE(not filepath.empty());

    fs::path tarfile(fs::temp_directory_path());
    tarfile /= "check.tar.bz2";


    fs::current_path(vle::utils::Path::path().getPackagesDir());

    BOOST_REQUIRE_NO_THROW(utils::Path::path().compress(uniquepath,
                                                        tarfile.string()));
    BOOST_REQUIRE(fs::exists(fs::path(tarfile)));

    fs::path tmpfile(fs::temp_directory_path());
    tmpfile /= "unique";

    fs::create_directory(tmpfile);
    fs::current_path(tmpfile);

    BOOST_REQUIRE_NO_THROW(utils::Path::path().decompress(tarfile.string(),
                                                          tmpfile.string()));
    BOOST_REQUIRE(fs::exists(fs::path(tmpfile)));

    tmpfile /= uniquepath;

    BOOST_REQUIRE(fs::exists(fs::path(tmpfile)));
}
