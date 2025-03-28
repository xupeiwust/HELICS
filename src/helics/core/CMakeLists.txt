# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Copyright (c) 2017-2025, Battelle Memorial Institute; Lawrence Livermore
# National Security, LLC; Alliance for Sustainable Energy, LLC.
# See the top-level NOTICE for additional details.
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(SRC_FILES
    CoreFactory.cpp
    BrokerFactory.cpp
    BrokerBase.cpp
    CommonCore.cpp
    FederateState.cpp
    PublicationInfo.cpp
    InputInfo.cpp
    InterfaceInfo.cpp
    EndpointInfo.cpp
    ActionMessage.cpp
    CoreBroker.cpp
    TimeCoordinator.cpp
    BaseTimeCoordinator.cpp
    ForwardingTimeCoordinator.cpp
    GlobalTimeCoordinator.cpp
    AsyncTimeCoordinator.cpp
    TimeDependencies.cpp
    HandleManager.cpp
    FilterInfo.cpp
    FilterCoordinator.cpp
    FilterFederate.cpp
    UnknownHandleManager.cpp
    LocalFederateId.cpp
    TimeoutMonitor.cpp
    coreTypeOperations.cpp
    helicsCLI11JsonConfig.cpp
    TranslatorFederate.cpp
    TimeCoordinatorProcessing.cpp
    BasicHandleInfo.cpp
    queryHelpers.cpp
    ProfilerBuffer.cpp
    EmptyCore.cpp
    helicsVersion.cpp
    TranslatorInfo.cpp
    LogManager.cpp
)

set(PUBLIC_INCLUDE_FILES
    Core.hpp
    Broker.hpp
    CoreFactory.hpp
    BrokerFactory.hpp
    core-exceptions.hpp
    CoreTypes.hpp
    core-data.hpp
    helicsTime.hpp
    CoreFederateInfo.hpp
    helicsVersion.hpp
    LocalFederateId.hpp
    helics_definitions.hpp
    helicsCLI11.hpp
    SmallBuffer.hpp
)

set(INCLUDE_FILES
    coreTypeOperations.hpp
    BrokerBase.hpp
    TimeDependencies.hpp
    TimeCoordinator.hpp
    BaseTimeCoordinator.hpp
    ForwardingTimeCoordinator.hpp
    GlobalTimeCoordinator.hpp
    AsyncTimeCoordinator.hpp
    loggingHelper.hpp
    GlobalFederateId.hpp
    basic_CoreTypes.hpp
    TimeoutMonitor.h
    CoreBroker.hpp
    InterfaceInfo.hpp
    ActionMessageDefintions.hpp
    ActionMessage.hpp
    CommonCore.hpp
    EmptyCore.hpp
    FederateState.hpp
    PublicationInfo.hpp
    InputInfo.hpp
    EndpointInfo.hpp
    TranslatorInfo.hpp
    flagOperations.hpp
    BasicHandleInfo.hpp
    FederateIdExtra.hpp
    FilterInfo.hpp
    FilterCoordinator.hpp
    FilterFederate.hpp
    TranslatorFederate.hpp
    HandleManager.hpp
    UnknownHandleManager.hpp
    queryHelpers.hpp
    fileConnections.hpp
    helicsCLI11JsonConfig.hpp
    TimeCoordinatorProcessing.hpp
    ProfilerBuffer.hpp
    LogManager.hpp
    ../helics_enums.h
)

if(NOT HELICS_DISABLE_ASIO)
    list(APPEND SRC_FILES MessageTimer.cpp)
    list(APPEND INCLUDE_FILES MessageTimer.hpp)
endif()

add_library(helics_core STATIC ${SRC_FILES} ${INCLUDE_FILES} ${PUBLIC_INCLUDE_FILES})

target_link_libraries(
    helics_core PUBLIC HELICS::common gmlc::networking PRIVATE spdlog::spdlog fmt::fmt
                                                               compile_flags_target
)

add_library(HELICS::core ALIAS helics_core)

target_compile_options(
    helics_core
    PRIVATE $<$<COMPILE_LANGUAGE:CXX>:$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-strict-aliasing>>
)

# adding a definition flag for some interaction with the shared library
target_compile_definitions(helics_core PUBLIC HELICS_STATIC_CORE_LIBRARY)
