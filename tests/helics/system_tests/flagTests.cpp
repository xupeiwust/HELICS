/*
Copyright (c) 2017-2025,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable
Energy, LLC.  See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

#include "../application_api/testFixtures.hpp"
#include "helics/ValueFederates.hpp"

#include "gtest/gtest.h"

/** tests for the different flag options and considerations*/

struct flag_tests: public FederateTestFixture, public ::testing::Test {};

class flag_type_tests: public ::testing::TestWithParam<const char*>, public FederateTestFixture {};

/** test simple creation and destruction*/
TEST_P(flag_type_tests, optional_pub)
{
    SetupTest<helics::ValueFederate>(GetParam(), 1, 1.0);
    auto vFed1 = GetFederateAs<helics::ValueFederate>(0);
    // register the publications
    auto& ipt1 = vFed1->registerInput<double>("ipt1");

    auto& ipt2 = vFed1->registerInput<double>("ipt2");
    auto& ipt3 = vFed1->registerInput<double>("ipt3");
    ipt1.setOption(helics::defs::Options::CONNECTION_OPTIONAL);
    ipt1.addTarget("bob");
    ipt2.addTarget("tom");
    ipt3.addTarget("harry");
    std::atomic<int> warnings{0};
    vFed1->setLoggingCallback(
        [&warnings](int level, std::string_view /*unused*/, std::string_view /*unused*/) {
            if (level == HELICS_LOG_LEVEL_WARNING) {
                ++warnings;
            }
        });

    vFed1->enterExecutingMode();
    EXPECT_EQ(warnings.load(), 2);
    vFed1->finalize();
}

TEST_P(flag_type_tests, optional_sub)
{
    SetupTest<helics::ValueFederate>(GetParam(), 1, 1.0);
    auto vFed1 = GetFederateAs<helics::ValueFederate>(0);
    // register the publications
    auto& pub1 = vFed1->registerPublication<double>("pub1");

    pub1.addTarget("bob");
    pub1.setOption(helics::defs::Options::CONNECTION_OPTIONAL);
    pub1.addTarget("tom");
    pub1.addTarget("harry");
    std::atomic<int> warnings{0};

    vFed1->setLoggingCallback(
        [&warnings](int level, std::string_view /*unused*/, std::string_view /*unused*/) {
            if (level == HELICS_LOG_LEVEL_WARNING) {
                ++warnings;
            }
        });

    vFed1->enterExecutingMode();
    EXPECT_EQ(warnings.load(), 1);
    EXPECT_TRUE(pub1.getOption(helics::defs::Options::CONNECTION_OPTIONAL));

    vFed1->finalize();
}

INSTANTIATE_TEST_SUITE_P(flag_tests, flag_type_tests, ::testing::ValuesIn(CoreTypes_single));

TEST_F(flag_tests, single_connection_test)
{
    SetupTest<helics::ValueFederate>("test", 1, 1.0);
    auto vFed1 = GetFederateAs<helics::ValueFederate>(0);
    // register the publications
    auto& ipt1 = vFed1->registerGlobalInput<double>("ipt1");

    auto& pub1 = vFed1->registerPublication<double>("pub1");
    auto& pub2 = vFed1->registerPublication<double>("pub2");
    ipt1.setOption(helics::defs::Options::SINGLE_CONNECTION_ONLY);
    pub1.addTarget("ipt1");
    pub2.addTarget("ipt1");

    EXPECT_THROW(vFed1->enterExecutingMode(), helics::ConnectionFailure);

    vFed1->finalize();
}

TEST_F(flag_tests, single_connection_test_pub)
{
    SetupTest<helics::ValueFederate>("test", 1, 1.0);
    auto vFed1 = GetFederateAs<helics::ValueFederate>(0);
    // register the publications
    auto& ipt1 = vFed1->registerGlobalInput<double>("ipt1");

    auto& ipt2 = vFed1->registerGlobalInput<double>("ipt2");
    auto& pub1 = vFed1->registerGlobalPublication<double>("pub1");
    pub1.setOption(helics::defs::Options::SINGLE_CONNECTION_ONLY);
    ipt1.addTarget("pub1");
    ipt2.addTarget("pub1");

    EXPECT_THROW(vFed1->enterExecutingMode(), helics::ConnectionFailure);

    vFed1->finalize();
}

TEST_F(flag_tests, type_match_check)
{
    SetupTest<helics::ValueFederate>("test", 1, 1.0);
    auto vFed1 = GetFederateAs<helics::ValueFederate>(0);
    // register the publications
    auto& ipt1 = vFed1->registerGlobalInput<double>("ipt1");

    vFed1->registerGlobalPublication<double>("pub1");
    ipt1.setOption(helics::defs::Options::STRICT_TYPE_CHECKING);
    ipt1.addTarget("pub1");
    vFed1->enterExecutingMode();
    EXPECT_TRUE(ipt1.getOption(helics::defs::Options::STRICT_TYPE_CHECKING));
    vFed1->finalize();
}

TEST_F(flag_tests, type_mismatch_error)
{
    SetupTest<helics::ValueFederate>("test", 1, 1.0);
    auto vFed1 = GetFederateAs<helics::ValueFederate>(0);
    // register the publications
    auto& ipt1 = vFed1->registerGlobalInput<double>("ipt1");

    vFed1->registerGlobalPublication<std::vector<double>>("pub1");
    ipt1.setOption(helics::defs::Options::STRICT_TYPE_CHECKING);
    ipt1.addTarget("pub1");
    EXPECT_THROW(vFed1->enterExecutingMode(), helics::ConnectionFailure);
    vFed1->finalize();
}

TEST_F(flag_tests, type_mismatch_error_fed_level)
{
    SetupTest<helics::ValueFederate>("test", 1, 1.0);
    auto vFed1 = GetFederateAs<helics::ValueFederate>(0);
    vFed1->setFlagOption(helics::defs::Flags::STRICT_INPUT_TYPE_CHECKING);
    // register the publications
    auto& ipt1 = vFed1->registerGlobalInput<double>("ipt1");

    vFed1->registerGlobalPublication<std::vector<double>>("pub1");
    ipt1.addTarget("pub1");
    EXPECT_THROW(vFed1->enterExecutingMode(), helics::ConnectionFailure);
    vFed1->finalize();
}

TEST_F(flag_tests, type_mismatch_error2)
{
    SetupTest<helics::ValueFederate>("test", 1, 1.0);
    auto vFed1 = GetFederateAs<helics::ValueFederate>(0);
    // register the publications
    auto& ipt1 = vFed1->registerGlobalInput("ipt1", "custom", "V");

    vFed1->registerGlobalPublication("pub1", "other");
    ipt1.setOption(helics::defs::Options::STRICT_TYPE_CHECKING);
    ipt1.addTarget("pub1");
    EXPECT_THROW(vFed1->enterExecutingMode(), helics::ConnectionFailure);
    vFed1->finalize();
}

TEST_F(flag_tests, slow_federate)
{
    // this flag doesn't do anything yet for federates, this is just to verify it is acknowledged
    // and captured
    SetupTest<helics::ValueFederate>("test", 1, 1.0);
    auto vFed1 = GetFederateAs<helics::ValueFederate>(0);
    vFed1->setFlagOption(HELICS_FLAG_SLOW_RESPONDING);
    vFed1->enterExecutingMode();
    EXPECT_TRUE(vFed1->getFlagOption(HELICS_FLAG_SLOW_RESPONDING));
    vFed1->finalize();
}

TEST_F(flag_tests, only_update_on_change_fedlevel)
{
    SetupTest<helics::ValueFederate>("test", 1, 1.0);
    auto vFed1 = GetFederateAs<helics::ValueFederate>(0);
    vFed1->setFlagOption(helics::defs::Options::HANDLE_ONLY_UPDATE_ON_CHANGE);

    // register the publications
    auto& ipt1 = vFed1->registerGlobalInput("ipt1", "double", "V");

    auto& pub1 = vFed1->registerGlobalPublication("pub1", "double");
    ipt1.addTarget("pub1");

    vFed1->enterExecutingMode();
    pub1.publish(45.7);
    vFed1->requestTime(1.0);
    EXPECT_TRUE(ipt1.isUpdated());
    double val = ipt1.getValue<double>();
    EXPECT_EQ(val, 45.7);
    EXPECT_TRUE(!ipt1.isUpdated());
    pub1.publish(45.7);
    vFed1->requestTime(2.0);
    EXPECT_TRUE(!ipt1.isUpdated());
    vFed1->finalize();
}

TEST_F(flag_tests, only_transmit_on_change_fedlevel)
{
    SetupTest<helics::ValueFederate>("test", 1, 1.0);
    auto vFed1 = GetFederateAs<helics::ValueFederate>(0);
    vFed1->setFlagOption(helics::defs::Options::HANDLE_ONLY_TRANSMIT_ON_CHANGE);

    // register the publications
    auto& ipt1 = vFed1->registerGlobalInput("ipt1", "double", "V");

    auto& pub1 = vFed1->registerGlobalPublication("pub1", "double");
    ipt1.addTarget("pub1");

    vFed1->enterExecutingMode();
    pub1.publish(45.7);
    vFed1->requestTime(1.0);
    EXPECT_TRUE(ipt1.isUpdated());
    double val = ipt1.getValue<double>();
    EXPECT_EQ(val, 45.7);
    EXPECT_TRUE(!ipt1.isUpdated());
    pub1.publish(45.7);
    vFed1->requestTime(2.0);
    EXPECT_TRUE(!ipt1.isUpdated());
    vFed1->finalize();
}
