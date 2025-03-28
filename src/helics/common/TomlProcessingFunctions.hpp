/*
Copyright (c) 2017-2025,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable
Energy, LLC.  See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

/** @file
@details functions related to loading and evaluating TOML files and helper functions for reading
them using the toml library
*/

#include "../core/helicsTime.hpp"
#ifdef __GNUC__
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wshadow"
#endif
#include <string_view>
#define TOML11_USING_STRING_VIEW 1
#include "toml.hpp"
#ifdef __GNUC__
#    pragma GCC diagnostic pop
#endif

#include <functional>
#include <string>

namespace helics::fileops {
/** load a TOML string or filename that points to a TOML file and return a
toml::Value to the root object
*/
toml::value loadToml(const std::string& tomlString);
/** return true if the string has a toml extension*/
bool hasTomlExtension(std::string_view tomlString);
/** check if the string looks like a possible config object in toml format*/
bool looksLikeConfigToml(std::string_view tomlString);
/** load a TOML object in a string
 * @throws std::invalid_argument if the string parsing failed through toml
 */
toml::value loadTomlStr(const std::string& tomlString);

/** get a toml value as a string*/
std::string tomlAsString(const toml::value& element);

/** read a time from a TOML value element*/
helics::Time loadTomlTime(const toml::value& timeElement,
                          time_units defaultUnits = time_units::sec);

/** get a name or key from the element*/
std::string getName(const toml::value& element);

/** get a string value or use the default if it is not a member*/
inline std::string
    getOrDefault(const toml::value& element, const std::string& key, std::string_view defVal)
{
    if (element.contains(key)) {
        return tomlAsString(element.at(key));
    }
    return std::string(defVal);
}
/** get a double value or use the default if it is not a member*/
inline double getOrDefault(const toml::value& element, const std::string& key, double defVal)
{
    return toml::find_or<double>(element, key, defVal);
}

/** get a boolean value or use the default if it is not a member*/
inline bool getOrDefault(const toml::value& element, const std::string& key, bool defVal)
{
    return toml::find_or<bool>(element, key, defVal);
}

/** get an integer value or use the default if it is not a member*/
inline int64_t getOrDefault(const toml::value& element, const std::string& key, int64_t defVal)
{
    return toml::find_or<int64_t>(element, key, defVal);
}

/** call a function if a member element exists and pass the string to the invoked object*/
inline bool callIfMember(const toml::value& element,
                         const std::string& key,
                         const std::function<void(const std::string&)>& call)
{
    const std::string empty;
    auto& val = toml::find_or<std::string>(element, key, empty);
    if (!val.empty()) {
        call(val);
        return true;
    }
    return false;
}

/** call a function if a member element exists and pass a time to the invoked object*/
inline bool callIfMember(const toml::value& element,
                         const std::string& key,
                         const std::function<void(const std::string&, helics::Time)>& call)
{
    toml::value uval;
    auto val = toml::find_or(element, key, uval);

    if (!val.is_empty()) {
        call(key, loadTomlTime(val));
        return true;
    }
    return false;
}

/** call a function if a member element exists and pass a specific type to the invoked object*/
template<class X>
inline bool callIfMember(const toml::value& element,
                         const std::string& key,
                         const std::function<void(const std::string&, X)>& call)
{
    toml::value uval;
    auto val = toml::find_or(element, key, uval);
    if (!val.is_empty()) {
        call(key, toml::get<X>(val));
        return true;
    }
    return false;
}

inline void
    replaceIfMember(const toml::value& element, const std::string& key, helics::Time& timeVal)
{
    toml::value uval;
    auto val = toml::find_or(element, key, uval);

    if (!val.is_empty()) {
        timeVal = loadTomlTime(val);
    }
}

inline void replaceIfMember(const toml::value& element, const std::string& key, std::string& loc)
{
    toml::value uval;
    auto val = toml::find_or(element, key, uval);

    if (!val.is_empty()) {
        loc = tomlAsString(val);
    }
}

template<class X>
inline void replaceIfMember(const toml::value& element, const std::string& key, X& loc)
{
    toml::value uval;
    auto val = toml::find_or(element, key, uval);

    if (!val.is_empty()) {
        loc = toml::get<X>(val);
    }
}

/** check if a key is a member of a table object*/
inline bool isMember(const toml::value& element, const std::string& key)
{
    toml::value uval;
    auto val = toml::find_or(element, key, uval);

    return (!val.is_empty());
}

}  // namespace helics::fileops
