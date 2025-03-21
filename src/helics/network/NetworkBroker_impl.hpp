/*
Copyright (c) 2017-2025,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable
Energy, LLC.  See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "../core/CoreTypes.hpp"
#include "../core/helicsCLI11.hpp"
#include "NetworkBroker.hpp"

#include <iostream>
#include <memory>
#include <string>

namespace helics {
constexpr const char* tstr[] = {"default",
                                "ZeroMQ",
                                "MPI",
                                "TEST",
                                "IPC",
                                "interprocess",
                                "TCP",
                                "UDP",
                                "undef",
                                "nng",
                                "ZMQ_SS",
                                "TCPSS",
                                "undef",
                                "undef",
                                "http",
                                "unknown"};

constexpr const char* tcodeStr(int tcode)
{
    return ((tcode >= 0) && (tcode < 15)) ? tstr[tcode] : tstr[15];
}

constexpr const char* defInterface[] = {"127.0.0.1",
                                        "127.0.0.1",
                                        "tcp://127.0.0.1",
                                        "_ipc_broker",
                                        ""};

template<class COMMS, gmlc::networking::InterfaceTypes baseline, int tcode>
NetworkBroker<COMMS, baseline, tcode>::NetworkBroker(bool rootBroker) noexcept:
    CommsBroker<COMMS, CoreBroker>(rootBroker)
{
    netInfo.server_mode = NetworkBrokerData::ServerModeOptions::SERVER_DEFAULT_ACTIVE;
}

template<class COMMS, gmlc::networking::InterfaceTypes baseline, int tcode>
NetworkBroker<COMMS, baseline, tcode>::NetworkBroker(std::string_view broker_name):
    CommsBroker<COMMS, CoreBroker>(broker_name)
{
    netInfo.server_mode = NetworkBrokerData::ServerModeOptions::SERVER_DEFAULT_ACTIVE;
}

template<class COMMS, gmlc::networking::InterfaceTypes baseline, int tcode>
std::shared_ptr<helicsCLI11App> NetworkBroker<COMMS, baseline, tcode>::generateCLI()
{
    auto app = CoreBroker::generateCLI();
    CLI::App_p netApp = netInfo.commandLineParser(defInterface[static_cast<int>(baseline)], false);
    app->add_subcommand(netApp);
    return app;
}

template<class COMMS, gmlc::networking::InterfaceTypes baseline, int tcode>
bool NetworkBroker<COMMS, baseline, tcode>::brokerConnect()
{
    std::lock_guard<std::mutex> lock(dataMutex);
    if ((netInfo.brokerName.empty()) && (netInfo.brokerAddress.empty())) {
        CoreBroker::setAsRoot();
    }
    netInfo.useJsonSerialization = BrokerBase::useJsonSerialization;
    netInfo.observer = BrokerBase::observer;
    CommsBroker<COMMS, CoreBroker>::comms->setName(CoreBroker::getIdentifier());
    CommsBroker<COMMS, CoreBroker>::comms->loadNetworkInfo(netInfo);
    CommsBroker<COMMS, CoreBroker>::comms->setTimeout(BrokerBase::networkTimeout.to_ms());

    auto res = CommsBroker<COMMS, CoreBroker>::comms->connect();
    if (res) {
        if (netInfo.portNumber < 0) {
            netInfo.portNumber = CommsBroker<COMMS, CoreBroker>::comms->getPort();
        }
    }
    return res;
}

template<class COMMS, gmlc::networking::InterfaceTypes baseline, int tcode>
std::string NetworkBroker<COMMS, baseline, tcode>::generateLocalAddressString() const
{
    using InterfaceTypes = gmlc::networking::InterfaceTypes;
    std::string add;
    if (CommsBroker<COMMS, CoreBroker>::comms->isConnected()) {
        add = CommsBroker<COMMS, CoreBroker>::comms->getAddress();
    } else {
        std::lock_guard<std::mutex> lock(dataMutex);
        switch (baseline) {
            case InterfaceTypes::TCP:
            case InterfaceTypes::IP:
            case InterfaceTypes::UDP:
                if (!netInfo.localInterface.empty() && (netInfo.localInterface.back() == '*')) {
                    add = gmlc::networking::makePortAddress(
                        netInfo.localInterface.substr(0, netInfo.localInterface.size() - 1),
                        netInfo.portNumber);
                } else {
                    add = gmlc::networking::makePortAddress(netInfo.localInterface,
                                                            netInfo.portNumber);
                }
                break;
            case InterfaceTypes::INPROC:
            case InterfaceTypes::IPC:
            default:
                if (!netInfo.localInterface.empty()) {
                    add = netInfo.localInterface;
                } else {
                    add = CoreBroker::getIdentifier();
                }
                break;
        }
    }
    return add;
}

}  // namespace helics
