/*
Copyright (c) 2017-2025,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable
Energy, LLC.  See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "../coreTypeLists.hpp"
#include "helics/application_api/Federate.hpp"
#include "helics/core/BrokerFactory.hpp"
#include "helics/core/Core.hpp"
#include "helics/core/CoreFactory.hpp"
#include "helics/core/coreTypeOperations.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

/** check if a type_name has an index code*/
bool hasIndexCode(std::string_view type_name);
/** get the index code if it has one*/
int getIndexCode(std::string_view type_name);

struct FederateTestFixture {
    FederateTestFixture() = default;
    ~FederateTestFixture();

    std::shared_ptr<helics::Broker> AddBroker(std::string_view CoreType_name, int count);
    std::shared_ptr<helics::Broker> AddBroker(std::string_view CoreType_name,
                                              const std::string& initialization_string);

    template<class FedType>
    void SetupTest(std::string_view CoreType_name,
                   int count,
                   helics::Time time_delta = helics::timeZero,
                   const std::string& name_prefix = defaultNamePrefix)
    {
        ctype = CoreType_name;
        auto broker = AddBroker(CoreType_name, count);
        if (!broker) {
            throw(std::runtime_error("unable to add broker"));
        }
        if (!broker->isConnected()) {
            broker->disconnect();
            broker = nullptr;
            helics::cleanupHelicsLibrary();
            broker = AddBroker(CoreType_name, count);
            if (!broker->isConnected()) {
                throw(std::runtime_error("Unable to connect rootbroker"));
            }
        }
        AddFederates<FedType>(CoreType_name, count, broker, time_delta, name_prefix);
    }

    template<class FedType>
    void AddFederates(std::string_view CoreType_name,
                      int count,
                      std::shared_ptr<helics::Broker> broker,
                      helics::Time time_delta = helics::timeZero,
                      const std::string& name_prefix = defaultNamePrefix)
    {
        bool hasIndex = hasIndexCode(CoreType_name);
        int setup = (hasIndex) ? getIndexCode(CoreType_name) : 1;
        if (hasIndex) {
            CoreType_name.remove_suffix(2);
        }

        std::string initString = std::string("--broker=") + broker->getIdentifier() +
            " --broker_address=" + broker->getAddress();

        if (!extraCoreArgs.empty()) {
            initString.push_back(' ');
            initString.append(extraCoreArgs);
        }

        helics::FederateInfo fedInfo(helics::core::coreTypeFromString(CoreType_name));
        if (time_delta != helics::timeZero) {
            fedInfo.setProperty(HELICS_PROPERTY_TIME_DELTA, time_delta);
        }
        if (!extraFederateArgs.empty()) {
            fedInfo.loadInfoFromArgsIgnoreOutput(extraFederateArgs);
        }
        switch (setup) {
            case 1:
            default: {
                size_t offset = federates.size();
                auto CoreType = helics::core::coreTypeFromString(CoreType_name);
                //  auto core = helics::CoreFactory::create (CoreType, name_prefix + "_core_" +
                //  std::to_string (offset),
                //                                          initString + " --federates " +
                //                                          std::to_string (count));
                auto core = helics::CoreFactory::create(CoreType,
                                                        initString + " --federates " +
                                                            std::to_string(count));
                fedInfo.coreName = core->getIdentifier();

                federates.resize(count + offset);
                for (int ii = 0; ii < count; ++ii) {
                    auto fedname = name_prefix + std::to_string(ii + offset);
                    auto fed = std::make_shared<FedType>(fedname, fedInfo);
                    federates[ii + offset] = fed;
                }
            } break;
            case 2: {  // each federate has its own core
                auto CoreType = helics::core::coreTypeFromString(CoreType_name);
                size_t offset = federates.size();
                federates.resize(count + offset);
                for (int ii = 0; ii < count; ++ii) {
                    //     auto core =
                    //     helics::CoreFactory::create (CoreType, name_prefix + "_core_" +
                    //     std::to_string (ii + offset),
                    //                                    initString + " --federates 1");
                    auto core =
                        helics::CoreFactory::create(CoreType, initString + " --federates 1");
                    fedInfo.coreName = core->getIdentifier();

                    auto fedname = name_prefix + std::to_string(ii + offset);
                    auto fed = std::make_shared<FedType>(fedname, fedInfo);
                    federates[ii + offset] = fed;
                }
            } break;
            case 3: {
                auto subbroker =
                    AddBroker(CoreType_name, initString + " --federates " + std::to_string(count));
                //    auto subbroker = AddBroker(CoreType_name, initString + " --federates " +
                //    std::to_string(count) +
                //        " --name=subbroker_" + name_prefix);
                if (!subbroker->isConnected()) {
                    throw(std::runtime_error("Unable to connect subbroker"));
                }
                std::string newTypeString(CoreType_name);
                newTypeString.push_back('_');
                newTypeString.push_back('2');
                for (int ii = 0; ii < count; ++ii) {
                    AddFederates<FedType>(newTypeString, 1, subbroker, time_delta, name_prefix);
                }
            } break;
            case 4: {
                std::string newTypeString(CoreType_name);
                newTypeString.push_back('_');
                newTypeString.push_back('2');
                for (int ii = 0; ii < count; ++ii) {
                    auto subbroker = AddBroker(CoreType_name, initString + " --federates 1");
                    //    auto subbroker = AddBroker(CoreType_name, initString + " --federates 1
                    //    --name=subbroker_" +
                    //        name_prefix + std::to_string(ii));
                    if (!subbroker->isConnected()) {
                        throw(std::runtime_error("Unable to connect subbroker(mode 4)"));
                    }
                    AddFederates<FedType>(newTypeString, 1, subbroker, time_delta, name_prefix);
                }
            } break;
            case 5:  // pairs of federates per core
            {
                auto CoreType = helics::core::coreTypeFromString(CoreType_name);
                size_t offset = federates.size();
                federates.resize(count + offset);
                for (int ii = 0; ii < count; ii += 2) {
                    auto core =
                        helics::CoreFactory::create(CoreType,
                                                    initString + " --federates " +
                                                        std::to_string((ii < count - 1) ? 2 : 1));
                    fedInfo.coreName = core->getIdentifier();

                    auto fedname = name_prefix + std::to_string(ii + offset);
                    auto fed = std::make_shared<FedType>(fedname, fedInfo);
                    federates[ii + offset] = fed;
                    if (ii + 1 < count) {
                        auto fedname2 = name_prefix + std::to_string(ii + offset + 1);
                        auto fed2 = std::make_shared<FedType>(fedname2, fedInfo);
                        federates[ii + offset + 1] = fed2;
                    }
                }
            } break;
            case 6:  // pairs of cores per subbroker
            {
                std::string newTypeString{CoreType_name};
                newTypeString.push_back('_');
                newTypeString.push_back('5');
                for (int ii = 0; ii < count; ii += 4) {
                    int fedcnt = (ii > count - 3) ? 4 : (count - ii);
                    auto subbroker =
                        AddBroker(CoreType_name,
                                  initString + " --federates " + std::to_string(fedcnt));
                    if (!subbroker->isConnected()) {
                        throw(std::runtime_error("Unable to connect subbroker(mode 4)"));
                    }
                    AddFederates<FedType>(
                        newTypeString, fedcnt, subbroker, time_delta, name_prefix);
                }
            } break;
            case 7:  // two layers of subbrokers
            {
                std::string newTypeString{CoreType_name};
                newTypeString.push_back('_');
                newTypeString.push_back('4');
                for (int ii = 0; ii < count; ++ii) {
                    auto subbroker = AddBroker(CoreType_name, initString + " --federates 1");
                    if (!subbroker->isConnected()) {
                        throw(std::runtime_error("Unable to connect subbroker(mode 4)"));
                    }
                    AddFederates<FedType>(newTypeString, 1, subbroker, time_delta, name_prefix);
                }
            } break;
        }
    }

    template<class FedType>
    std::shared_ptr<FedType> GetFederateAs(int index)
    {
        return std::dynamic_pointer_cast<FedType>(federates[index]);
    }

    void FullDisconnect();

    std::vector<std::shared_ptr<helics::Broker>> brokers;
    std::vector<std::shared_ptr<helics::Federate>> federates;
    std::string extraCoreArgs;
    std::string extraBrokerArgs;
    std::string extraFederateArgs;
    std::string ctype;
    bool debugDiagnostic{false};
};

/** set an environment variable to a specific value */
void setEnvironmentVariable(const std::string& name, const std::string& value);
/** clear an environment variable */
void clearEnvironmentVariable(const std::string& name);
