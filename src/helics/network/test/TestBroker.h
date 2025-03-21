/*
Copyright (c) 2017-2025,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable
Energy, LLC.  See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include "../NetworkBroker.hpp"

namespace helics {
namespace testcore {
    class TestComms;

    /** implementation for the core that uses IPC messages to communicate*/
    using TestBroker = NetworkBroker<TestComms,
                                     gmlc::networking::InterfaceTypes::INPROC,
                                     static_cast<int>(CoreType::TEST)>;

}  // namespace testcore
}  // namespace helics
