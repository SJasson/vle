/**
 * @file utils/Tools.hpp
 * @author The VLE Development Team.
 * @brief Most of tools to use into VLE projects, plugins etc.
 */

/*
 * Copyright (c) 2004, 2005 The vle Development Team
 *
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef UTILS_TOOLS_HPP
#define UTILS_TOOLS_HPP

#include <glibmm/fileutils.h>
#include <glibmm/miscutils.h>
#include <boost/format.hpp>
#include <cstdarg>
#include <string>
#include <sstream>
#include <vle/utils/Exception.hpp>
#include <vle/utils/Debug.hpp>



namespace vle { namespace utils {

    /**
     * test file existence.
     *
     * @param filename file name to test.
     * @return true if exist, false otherwise.
     */
    inline bool exist_file(const std::string& filename)
    {
        return Glib::file_test(filename, Glib::FILE_TEST_IS_REGULAR |
                               Glib::FILE_TEST_EXISTS);
    }

    /**
     * test directory existence.
     *
     * @param dirname directory name to test.
     * @return true if exist, false otherwise.
     */
    inline bool exist_dir(const std::string& dirname)
    {
        return Glib::file_test(dirname, Glib::FILE_TEST_IS_DIR |
                               Glib::FILE_TEST_EXISTS);
    }

    /**
     * build a portable tempory filename like:
     *
     * @code
     * $TMP/<filename> on Unix/Linux
     * $ c:\windows\tmp\<filename> on Windows
     * @endcode
     *
     * @param filename string to add on tempory directory
     * @return a portable tempory filename
     */
    inline std::string build_temp(const std::string& filename)
    {
        return Glib::build_filename(Glib::get_tmp_dir(), filename);
    }

    /**
     * Write specified buffer to tempory file and return the filename. The file
     * is created in the tempory directory.
     *
     * @param prefix if you want to prefix the tempory file.
     * @param buffer buffer to write.
     * @return filename of the new tempory file.
     */
    std::string write_to_temp(const std::string& prefix,
                              const std::string& buffer);

    /**
     * Transform a string into an object of class C.
     *
     * example :
     * @code
     * double d = translate<double>("32.435");
     * @endcode
     *
     * @param str std::string to translate into class C type.
     * @return class C.
     */
    template<class C> inline C translate(const std::string& str)
    {
        std::stringstream i;

        C c;
        i << str;
        i >> c;

        return c;
    }

    /**
     * Assert a file existence. Idem exist_file but throw an Exception if
     * file does not exist.
     *
     * @param filename file name to test
     * @throw Exception::File
     */
    inline void assert_exist_file(const std::string& filename)
    {
        if (not exist_file(filename)) {
            throw FileError((boost::format("File %1% does not exist\n") %
                            filename).str());
        }
    }

    /**
     * Return true if str can be translate into a double.
     *
     * @param str string to test.
     * @return true if str can be translate, otherwise false.
     */
    inline bool is_double(const std::string& str)
    {
        std::istringstream in(str);
        double d;
        in >> d;
        return !in.fail();
    }

    /**
     * Return true if str can be translate into an integer.
     *
     * @param str string to test.
     * @return true if str can be translate, otherwise false.
     */
    inline bool is_int(const std::string& str)
    {
        std::istringstream in(str);
        int i;
        in >> i;
        return !in.fail();
    }

    /**
     * Transform an object of class C into a string
     *
     * example :
     * @code
     * std::string s = to_string(123.324, 5, 1);
     * @endcode
     *
     * @param c object to transform
     * @param precision
     * @param width
     * @return string representation of templace < class C >
     */
    template <class C > inline
    std::string to_string(const C& c, int precision, int width)
    {
        std::ostringstream o;

        o.precision(precision);
        o.width(width);
        o << c;

        return o.str();
    }

    /**
     * Return conversion from template into string.
     *
     * example :
     * @code
     * std::string v = to_string(23243);
     * std::string l = to_string(141.12341);
     * std::string e = to_string("hello");
     * @endcode
     *
     * @param c object to transform
     * @return string representation of templace < class C >
     */
    template < class C > inline
    std::string to_string(const C& c)
    {
        std::ostringstream o;

        o << c;

        return o.str();
    }

    /**
     * Return conversion from string into double.
     *
     * @param str string to convert.
     * @param def default value to return.
     * @return result of convertion, default if error.
     */
    inline double to_double(const std::string& str, double def = 0.0)
    {
        std::istringstream in(str);
        double d;
        in >> d;

        if (in.fail() == true) {
            d = def;
        }
        return d;
    }

    /**
     * Return conversion from string into int.
     *
     * @param str string to convert.
     * @param def default value to return.
     * @return result of convertion, default if error.
     */
    inline int to_int(const std::string& str, int def = 0)
    {
        std::istringstream in(str);
        int i;
        in >> i;

        if (in.fail() == true) {
            i = def;
        }
        return i;
    }

    /**
     * Return conversion from string into unsigned int.
     *
     * @param str string to convert.
     * @param def default value to return.
     * @return result of convertion, default if error.
     */
    inline unsigned int to_uint(const std::string& str, unsigned int def = 0)
    {
        std::istringstream in(str);
        unsigned int i;
        in >> i;

        if (in.fail() == true) {
            i = def;
        }
        return i;
    }

    /**
     * Return conversion from string into size_t.
     *
     * @param str string to convert.
     * @param def default value to return.
     * @return result of convertion, default if error.
     */
    inline size_t to_size_t(const std::string& str, size_t def = 0)
    {
        std::istringstream in(str);
        size_t i;
        in >> i;

        if (in.fail() == true) {
            i = def;
        }
        return i;
    }

    /**
     * Return conversion from string into boolean.
     *
     * @param str string to convert.
     * @return true if str == TRUE or true, or a integer !0.
     */
    inline bool is_boolean(const std::string& str)
    {
        if (is_int(str)) {
            int i = to_int(str);
            return i != 0;
        } else if (str == "true" or str == "TRUE") {
            return true;
        }
        return false;
    }

    /**
     * Return true if str can be translate into a boolean.
     *
     * @param str string to test.
     * @param def default value to return.
     * @return result of convertion, default if error.
     */
    inline bool to_boolean(const std::string& str, bool def = false)
    {
        if (is_int(str)) {
            int i = to_int(str);
            return i != 0;
        } else if (str == "true" or str == "TRUE") {
            return true;
        }
        return def;
    }

    /**
     * Clean string representation of double.
     *
     * @param str string to clean.
     * @throw Exception::BadArg.
     */
    inline void clean_double(std::string& str)
    {
        std::istringstream in(str);
        double d;
        in >> d;

        if (in.fail() == true) {
            throw ArgError((boost::format("Cannot clean '%1%' into an double\n")
                            % str).str());
        }

        std::ostringstream out;
        out << d;

        str.assign(out.str());
    }

    /**
     * Clean string representation of double.
     *
     * @param str string to clean.
     * @throw Exception::BadArg.
     */
    inline void clean_int(std::string& str)
    {
        std::istringstream in(str);
        int i;
        in >> i;

        if (in.fail() == true) {
            throw ArgError((boost::format("cannot clean '%1%' into an int\n") %
                            str).str());
        }

        std::ostringstream out;
        out << i;

        str.assign(out.str());
    }

    /**
     * Clean string representation of double.
     *
     * @param str string to clean.
     * @throw Exception::BadArg.
     */
    inline std::string clean_double(const std::string& str)
    {
        std::istringstream in(str);
        double d;
        in >> d;

        if (in.fail() == true) {
            throw ArgError((boost::format("cannot clean '%1%' into an double\n") %
                            str).str());
        }

        std::ostringstream out;
        out << d;

        return out.str();
    }

    /**
     * Clean string representation of double.
     *
     * @param str string to clean.
     * @throw Exception::BadArg.
     */
    inline std::string clean_int(const std::string& str)
    {
        std::istringstream in(str);
        int i;
        in >> i;

        if (in.fail() == true) {
            throw ArgError((boost::format("cannot clean '%1%' into an int\n") %
                            str).str());
        }

        std::ostringstream out;
        out << i;

        return out.str();
    }

    /**
     * Write the current date and time conform to RFC 822.
     *
     *
     * @return string representation of date.
     */
    std::string get_current_date();

    /**
     * Return true if unicode string str is an ascii string with only
     * character between A-Z and a-z and size not empty.
     *
     * @param str unicode string to test validity in VLE name
     * @return true if str is correct, otherwise, false.
     */
    bool is_vle_string(const Glib::ustring& str);

    /**
     * @Return the stirng representation of error. This string begin with ten
     * minus characters.
     *
     */
    Glib::ustring print_trace_report();

    /**
     * Slot of lauched signals.
     *
     * @param signal_number signal description.
     */
    void print_trace_signals(int signal_number);

    /** Connection signal to print_trace_signals. */
    void install_signal();

    /** 
     * @brief Demangle the input type info from C++ compiler.
     * http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/namespaceabi.html
     * 
     * @param in the std::type_info to demangle.
     * 
     * @return the demangled string or the same if libcwd is not linked.
     */
    std::string demangle(const std::type_info& in);

    /** 
     * @brief Demangle the input string from C++ compiler.
     * http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/namespaceabi.html
     * 
     * @param in the string to demangle.
     * 
     * @return the demangled string or the same if libcwd is not linked.
     */
    std::string demangle(const std::string& in);

    /**
     * Build the user directories .vle, .vle/plugins, .vle/models etc.  in
     * $HOME/.vle. All loading error are store into log file.
     *
     */
    void initUserDirectory();

    /** 
     * @brief Make a new directory. All error are reported to log file.
     * 
     * @param dirname The new directory.
     * 
     * @return true if success, otherwise false.
     */
    bool buildDirectory(const std::string& dirname);

    /** 
     * @brief Get the user vle directory, $HOME/.vle and build it if necessary.
     * All error are reported to log file.
     * 
     * @return The string $HOME/.vle or empty string if error.
     */
    std::string getUserDirectory();

    /** 
     * @brief Change the current process to daemon. Current path is reset to
     * '/' on Linux/Unix and 'c://' to Windows, adjust the session and
     * close all opened files.
     */
    void buildDaemon();

}} // namespace vle utils

#endif
