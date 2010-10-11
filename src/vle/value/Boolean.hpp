/*
 * @file vle/value/Boolean.hpp
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


#ifndef VLE_VALUE_BOOLEAN_HPP
#define VLE_VALUE_BOOLEAN_HPP 1

#include <vle/value/Value.hpp>
#include <vle/value/DllDefines.hpp>

namespace vle { namespace value {

/**
 * @brief Boolean Value encapsulate a C++ 'bool' type into a class.
 */
class VLE_VALUE_EXPORT Boolean : public Value
{
public:
    /**
     * @brief Build a Boolean object with a false as value.
     */
    Boolean()
        : m_value(false)
    {}

    /**
     * @brief Build a Boolean object with a specified value.
     * @param value The value to copy.
     */
    Boolean(bool value)
        : m_value(value)
    {}

    /**
     * @brief Copy constructor.
     * @param value The value to copy.
     */
    Boolean(const Boolean& value)
        : Value(value), m_value(value.m_value)
    {}

    /**
     * @brief Nothing to delete.
     */
    virtual ~Boolean()
    {}

    ///
    ////
    ///

    /**
     * @brief Build a Boolean.
     * @param value the value of the boolean.
     * @return A new Boolean.
     */
    static Boolean* create(bool value = false)
    { return new Boolean(value); }

    ///
    ////
    ///

    /**
     * @brief Clone the current Boolean with the same value.
     * @return A new Boolean.
     */
    virtual Value* clone() const
    { return new Boolean(m_value); }

    /**
     * @brief Get the type of this class.
     * @return Return Value::BOOLEAN.
     */
    inline virtual Value::type getType() const
    { return Value::BOOLEAN; }

    /**
     * @brief Push the bool in the stream. Use the ostream operator
     * alphabool() to get true or false string instead of 0 an 1.
     * @param out The output stream.
     */
    virtual void writeFile(std::ostream& out) const;

    /**
     * @brief Push the bool in the stream. Use the ostream operator
     * alphabool() to get true or false string instead of 0 and 1.
     * @param out The output stream.
     */
    virtual void writeString(std::ostream& out) const;

    /**
     * @brief Push the bool in the stream. The string pushed in the ostream:
     * @code
     * <boolean>true</boolean>
     * <boolean>false</boolean>
     * @endcode
     * @param out The output stream.
     */
    virtual void writeXml(std::ostream& out) const;

    ///
    ////
    ///

    /**
     * @brief Get the value of the bool.
     * @return true or false.
     */
    inline bool value() const
    { return m_value; }

    /**
     * @brief Get a reference to the bool.
     * @return A reference true or false.
     */
    inline bool& value()
    { return m_value; }

    /**
     * @brief Assign a value to the bool.
     * @param value The value to set.
     */
    inline void set(bool value)
    { m_value = value; }

private:
    bool m_value;

    friend class boost::serialization::access;
    template < class Archive >
        void serialize(Archive& ar, const unsigned int /* version */)
        {
            ar & boost::serialization::base_object < Value >(*this);
            ar & m_value;
        }
};

inline const Boolean& toBooleanValue(const Value& value)
{ return value.toBoolean(); }

inline const Boolean* toBooleanValue(const Value* value)
{ return value ? &value->toBoolean() : 0; }

inline Boolean& toBooleanValue(Value& value)
{ return value.toBoolean(); }

inline Boolean* toBooleanValue(Value* value)
{ return value ? &value->toBoolean() : 0; }

inline bool toBoolean(const Value& value)
{ return value.toBoolean().value(); }

inline bool& toBoolean(Value& value)
{ return value.toBoolean().value(); }

inline bool toBoolean(const Value* value)
{ return value::reference(value).toBoolean().value(); }

inline bool& toBoolean(Value* value)
{ return value::reference(value).toBoolean().value(); }

}} // namespace vle value

#endif
