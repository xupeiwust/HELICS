/*
Copyright (c) 2017-2025,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable
Energy, LLC.  See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

#include "JsonBuilder.hpp"

#include "JsonProcessingFunctions.hpp"
#include "gmlc/utilities/stringOps.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace helics::fileops {
using stringVector = gmlc::utilities::stringVector;

JsonMapBuilder::JsonMapBuilder() noexcept {}

JsonMapBuilder::~JsonMapBuilder() = default;

nlohmann::json& JsonMapBuilder::getJValue()
{
    if (!jMap) {
        jMap = std::make_unique<nlohmann::json>();
    }
    return *jMap;
}

bool JsonMapBuilder::isCompleted() const
{
    return (jMap) && (missing_components.empty());
}

int JsonMapBuilder::generatePlaceHolder(const std::string& location, int32_t code)
{
    int index = static_cast<int>(missing_components.size()) + 2;
    missing_components.emplace(index, std::make_pair(location, code));
    return index;
}

bool JsonMapBuilder::addComponent(const std::string& info, int index) noexcept
{
    auto loc = missing_components.find(index);
    if (loc != missing_components.end()) {
        if (info == "#invalid") {
            (*jMap)[loc->second.first].push_back(nlohmann::json::object());
        } else {
            try {
                auto element = loadJsonStr(info);
                (*jMap)[loc->second.first].push_back(element);
            }
            catch (const std::invalid_argument&) {
                (*jMap)[loc->second.first].push_back(nlohmann::json::object());
            }
        }

        missing_components.erase(loc);

        return missing_components.empty();
    }
    return false;
}

bool JsonMapBuilder::clearComponents(int32_t code)
{
    for (auto b = missing_components.begin(); b != missing_components.end(); ++b) {
        if (b->second.second == code) {
            missing_components.erase(b);
            return missing_components.empty();
        }
    }
    return false;
}

bool JsonMapBuilder::clearComponents()
{
    missing_components.clear();
    return static_cast<bool>(jMap);
}

std::string JsonMapBuilder::generate()
{
    if (jMap) {
        return generateJsonString(*jMap);
    }
    return "{}";
}

void JsonMapBuilder::reset()
{
    jMap = nullptr;
    missing_components.clear();
}

JsonBuilder::JsonBuilder() noexcept {}

JsonBuilder::~JsonBuilder() = default;
/** add an element on a specific path*/
void JsonBuilder::addElement(const std::string& path, const std::string& value)
{
    stringVector res = gmlc::utilities::stringOps::splitline(
        path, "\\/:.", gmlc::utilities::stringOps::delimiter_compression::on);
    auto* jv = &getJValue();
    size_t ii = 0;
    for (ii = 0; ii < res.size() - 1; ++ii) {
        auto& sub = (*jv)[res[ii]];
        if (sub.is_null()) {
            (*jv)[res[ii]] = nlohmann::json::object();
        }
        jv = &(*jv)[res[ii]];
    }
    (*jv)[res.back()] = value;
}

void JsonBuilder::addElement(const std::string& path, double value)
{
    stringVector res = gmlc::utilities::stringOps::splitline(
        path, "\\/:.", gmlc::utilities::stringOps::delimiter_compression::on);
    auto* jv = &getJValue();
    size_t ii = 0;
    for (ii = 0; ii < res.size() - 1; ++ii) {
        auto& sub = (*jv)[res[ii]];
        if (sub.is_null()) {
            (*jv)[res[ii]] = nlohmann::json::object();
        }
        jv = &(*jv)[res[ii]];
    }
    (*jv)[res.back()] = value;
}

void JsonBuilder::addElement(const std::string& path, const std::vector<double>& value)
{
    stringVector res = gmlc::utilities::stringOps::splitline(
        path, "\\/:.", gmlc::utilities::stringOps::delimiter_compression::on);
    auto* jv = &getJValue();
    size_t ii = 0;
    for (ii = 0; ii < res.size() - 1; ++ii) {
        auto& sub = (*jv)[res[ii]];
        if (sub.is_null()) {
            (*jv)[res[ii]] = nlohmann::json();
        }
        jv = &(*jv)[res[ii]];
    }
    (*jv)[res.back()] = nlohmann::json::array();
    for (const auto& v : value) {
        (*jv)[res.back()].push_back(v);
    }
}

nlohmann::json& JsonBuilder::getJValue()
{
    if (!jMap) {
        jMap = std::make_unique<nlohmann::json>();
    }
    return *jMap;
}

std::string JsonBuilder::generate()
{
    if (jMap) {
        return generateJsonString(*jMap);
    }
    return "{}";
}

void JsonBuilder::reset()
{
    jMap = nullptr;
}
}  // namespace helics::fileops
