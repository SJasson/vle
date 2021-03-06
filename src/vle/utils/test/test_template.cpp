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
#define BOOST_TEST_MODULE utils_library_test_template
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include <limits>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <numeric>
#include <vle/utils/Template.hpp>
#include <vle/utils/Tools.hpp>
#include <vle/utils/Exception.hpp>
#include <vle/vle.hpp>

using namespace vle;

struct F
{
    vle::Init a;

    F() : a() { }
    ~F() { }
};

BOOST_GLOBAL_FIXTURE(F)

const char* tpl1 = "@@tag test1@package.name-ext1_ext2 @@" \
                    "azertyuiop qsdfghjklm wxcvbn,;:!" \
                    "@@end tag@@" \
                    "a) My name is {{name}} and I am {{year}} old\n" \
                    "b) My name is {{name}} and I am {{year}} old\n" \
                    "c) My name is {{name}} and I am {{year}} olds\n";

const char* tpl2 = "class {{name}} : public DifferenceEquation::Multiple\n" \
                    "{\n" \
                    "public:\n" \
                    "    {{name}}(\n" \
                    "       const graph::AtomicModel& atom,\n" \
                    "       const devs::InitEventList& evts)\n" \
                    "        : DifferenceEquation::Multiple(atom, evts)\n" \
                    "    {\n" \
                    "{{for i in list}}" \
                    "        {{list^i}} = createVar(\"{{list2^i}}\");\n" \
                    "        {{list2^i}} = createVar(\"{{list^i}}\");\n" \
                    "{{end for}}" \
                    "    }\n" \
                    "\n" \
                    "    virtual ~{{name}}()\n" \
                    "    {}\n" \
                    "\n" \
                    "private:\n" \
                    "{{for i in list}}" \
                    "    /* avant {{name}}:{{list^i}} */\n" \
                    "    Var {{list^i}};\n" \
                    "    /* milieu c'est merveilleux */\n" \
                    "    Var {{list2^i}};\n" \
                    "    /* après*/\n" \
                    "{{end for}}" \
                    "\n" \
                    "};\n";

const char* tpl3 = "class {{name}} : public DifferenceEquation::Multiple\n" \
                    "{\n" \
                    "public:\n" \
                    "    {{name}}(\n" \
                    "       const graph::AtomicModel& atom,\n" \
                    "       const devs::InitEventList& evts)\n" \
                    "        : DifferenceEquation::Multiple(atom, evts)\n" \
                    "    {\n" \
                    "{{for i in list}}" \
                    "{{if isabc}}" \
                    "        {{list^i}} = createVar(\"{{list2^i}}\");\n" \
                    "{{end if}}" \
                    "{{ifnot isabc}}" \
                    "        {{list2^i}} = createVar(\"{{list^i}}\");\n" \
                    "{{end ifnot}}" \
                    "{{end for}}" \
                    "    }\n" \
                    "\n" \
                    "    virtual ~{{name}}()\n" \
                    "    {}\n" \
                    "\n" \
                    "private:\n" \
                    "{{for i in list}}" \
                    "    /* avant {{name}}:{{list^i}} */\n" \
                    "{{if isabc}}" \
                    "    Var {{list^i}};\n" \
                    "{{end if}}" \
                    "{{ifnot isabc}}" \
                    "    Var {{list2^i}};\n" \
                    "    /* après*/\n" \
                    "{{end ifnot}}" \
                    "{{end for}}" \
                    "\n" \
                    "};\n";

const char* tpl4 = "@tag test1@package.name-ext1_ext2 @@" \
                    "azertyuiop qsdfghjklm wxcvbn,;:!" \
                    "@@end tag@@" \
                    "a) My name is {{name}} and I am {{year}} old\n" \
                    "b) My name is {{name}} and I am {{year}} old\n" \
                    "c) My name is {{name}} and I am {{year}} olds\n";

const char* tpl5 = "@@end tag@@" \
                    "azertyuiop qsdfghjklm wxcvbn,;:!" \
                    "@@tag tag@@" \
                    "a) My name is {{name}} and I am {{year}} old\n" \
                    "b) My name is {{name}} and I am {{year}} old\n" \
                    "c) My name is {{name}} and I am {{year}} olds\n";


BOOST_AUTO_TEST_CASE(test_template_simple)
{
    vle::utils::Template tpl(tpl1);
    tpl.stringSymbol().append("name", "toto");
    tpl.stringSymbol().append("year", "74");

    std::ostringstream out;
    tpl.process(out);

    std::string str = out.str();

    BOOST_REQUIRE_EQUAL(str,
                        "@@tag test1@package.name-ext1_ext2 @@" \
                        "azertyuiop qsdfghjklm wxcvbn,;:!" \
                        "@@end tag@@" \
                        "a) My name is toto and I am 74 old\n"
                        "b) My name is toto and I am 74 old\n"
                        "c) My name is toto and I am 74 olds\n");
}

BOOST_AUTO_TEST_CASE(test_template_for)
{
    vle::utils::Template tpl(tpl2);

    tpl.stringSymbol().append("name", "meteo");

    tpl.listSymbol().append("list", "X");
    tpl.listSymbol().append("list", "Y");
    tpl.listSymbol().append("list", "Z");

    tpl.listSymbol().append("list2", "A");
    tpl.listSymbol().append("list2", "B");
    tpl.listSymbol().append("list2", "C");

    std::stringstream out;
    tpl.process(out);

    BOOST_REQUIRE_EQUAL(out.str(),
                        "class meteo : public DifferenceEquation::Multiple\n"
                        "{\n"
                        "public:\n"
                        "    meteo(\n"
                        "       const graph::AtomicModel& atom,\n"
                        "       const devs::InitEventList& evts)\n"
                        "        : DifferenceEquation::Multiple(atom, evts)\n"
                        "    {\n"
                        "        X = createVar(\"A\");\n"
                        "        A = createVar(\"X\");\n"
                        "        Y = createVar(\"B\");\n"
                        "        B = createVar(\"Y\");\n"
                        "        Z = createVar(\"C\");\n"
                        "        C = createVar(\"Z\");\n"
                        "    }\n"
                        "\n"
                        "    virtual ~meteo()\n"
                        "    {}\n"
                        "\n"
                        "private:\n"
                        "    /* avant meteo:X */\n"
                        "    Var X;\n"
                        "    /* milieu c'est merveilleux */\n"
                        "    Var A;\n"
                        "    /* après*/\n"
                        "    /* avant meteo:Y */\n"
                        "    Var Y;\n"
                        "    /* milieu c'est merveilleux */\n"
                        "    Var B;\n"
                        "    /* après*/\n"
                        "    /* avant meteo:Z */\n"
                        "    Var Z;\n"
                        "    /* milieu c'est merveilleux */\n"
                        "    Var C;\n"
                        "    /* après*/\n"
                        "\n"
                        "};\n");
}

BOOST_AUTO_TEST_CASE(test_template_for_if)
{
    vle::utils::Template tpl(tpl3);

    tpl.stringSymbol().append("name", "meteo");

    tpl.listSymbol().append("list", "X");
    tpl.listSymbol().append("list", "Y");
    tpl.listSymbol().append("list", "Z");

    tpl.listSymbol().append("list2", "A");
    tpl.listSymbol().append("list2", "B");
    tpl.listSymbol().append("list2", "C");

    tpl.boolSymbol().append("isabc", true);

    std::stringstream out;
    tpl.process(out);

    BOOST_REQUIRE_EQUAL(out.str(),
                        "class meteo : public DifferenceEquation::Multiple\n"
                        "{\n"
                        "public:\n"
                        "    meteo(\n"
                        "       const graph::AtomicModel& atom,\n"
                        "       const devs::InitEventList& evts)\n"
                        "        : DifferenceEquation::Multiple(atom, evts)\n"
                        "    {\n"
                        "        X = createVar(\"A\");\n"
                        "        Y = createVar(\"B\");\n"
                        "        Z = createVar(\"C\");\n"
                        "    }\n"
                        "\n"
                        "    virtual ~meteo()\n"
                        "    {}\n"
                        "\n"
                        "private:\n"
                        "    /* avant meteo:X */\n"
                        "    Var X;\n"
                        "    /* avant meteo:Y */\n"
                        "    Var Y;\n"
                        "    /* avant meteo:Z */\n"
                        "    Var Z;\n"
                        "\n"
                        "};\n");
}

BOOST_AUTO_TEST_CASE(test_template_for_ifnot)
{
    vle::utils::Template tpl(tpl3);

    tpl.stringSymbol().append("name", "meteo");

    tpl.listSymbol().append("list", "X");
    tpl.listSymbol().append("list", "Y");
    tpl.listSymbol().append("list", "Z");

    tpl.listSymbol().append("list2", "A");
    tpl.listSymbol().append("list2", "B");
    tpl.listSymbol().append("list2", "C");

    tpl.boolSymbol().append("isabc", false);

    std::stringstream out;
    tpl.process(out);

    BOOST_REQUIRE_EQUAL(out.str(),
                        "class meteo : public DifferenceEquation::Multiple\n"
                        "{\n"
                        "public:\n"
                        "    meteo(\n"
                        "       const graph::AtomicModel& atom,\n"
                        "       const devs::InitEventList& evts)\n"
                        "        : DifferenceEquation::Multiple(atom, evts)\n"
                        "    {\n"
                        "        A = createVar(\"X\");\n"
                        "        B = createVar(\"Y\");\n"
                        "        C = createVar(\"Z\");\n"
                        "    }\n"
                        "\n"
                        "    virtual ~meteo()\n"
                        "    {}\n"
                        "\n"
                        "private:\n"
                        "    /* avant meteo:X */\n"
                        "    Var A;\n"
                        "    /* après*/\n"
                        "    /* avant meteo:Y */\n"
                        "    Var B;\n"
                        "    /* après*/\n"
                        "    /* avant meteo:Z */\n"
                        "    Var C;\n"
                        "    /* après*/\n"
                        "\n"
                        "};\n");
}

BOOST_AUTO_TEST_CASE(test_template_tag)
{
    std::string plugin, package, conf;

    {
        vle::utils::Template tpl(tpl1);
        tpl.tag(plugin, package, conf);
        BOOST_REQUIRE_EQUAL(plugin, "test1");
        BOOST_REQUIRE_EQUAL(package, "package.name-ext1_ext2");
        BOOST_REQUIRE_EQUAL(conf, "azertyuiop qsdfghjklm wxcvbn,;:!");
    }

    {
        vle::utils::Template tpl(tpl5);
        BOOST_REQUIRE_THROW(tpl.tag(plugin, package, conf), utils::ArgError);
    }

    {
        vle::utils::Template tpl(tpl4);
        BOOST_REQUIRE_THROW(tpl.tag(plugin, package, conf), utils::ArgError);
    }
}

