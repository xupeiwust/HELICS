/*
Copyright (c) 2017-2025,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable
Energy, LLC.  See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

#include "helics/application_api/ValueFederate.hpp"
#include "helics/apps/helicsWebServer.hpp"
#include "helics/common/JsonProcessingFunctions.hpp"
#include "helics/core/BrokerFactory.hpp"
#include "helics/core/CoreFactory.hpp"
#include "helics/core/coreTypeOperations.hpp"

#include "gtest/gtest.h"
#include <algorithm>
#include <boost/asio/connect.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/config.hpp>
#include <boost/container/flat_map.hpp>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <streambuf>
#include <string>
#include <thread>
#include <vector>

namespace beast = boost::beast;  // from <boost/beast.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace http = beast::http;  // from <boost/beast/http.hpp>
namespace net = boost::asio;  // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

using namespace helics::fileops;
constexpr std::string_view localhost{"localhost"};

class webTest: public ::testing::Test {
  protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite()
    {
        webs = std::make_shared<helics::apps::WebServer>();
        webs->enableWebSocketServer(true);
        config["websocket"] = nlohmann::json::object();
        config["websocket"]["port"] = 26247;
        webs->startServer(&config, webs);

        // These objects perform our I/O
        tcp::resolver resolverObj(ioc);
        stream = std::make_unique<websocket::stream<tcp::socket>>(ioc);  // NOLINT

        // Look up the domain name
        auto const results = resolverObj.resolve(localhost, "26247");

        // Make the connection on the IP address we get from a lookup
        net::connect(stream->next_layer(), results.begin(), results.end());

        // Set a decorator to change the User-Agent of the handshake
        stream->set_option(websocket::stream_base::decorator([](websocket::request_type& req) {
            req.set(http::field::user_agent,
                    std::string(HELICS_VERSION_STRING) + " helics-websocket-test");
        }));

        // Perform the websocket handshake
        stream->handshake({localhost.data(), localhost.size()}, "/");
    }

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite()
    {
        // Close the WebSocket connection
        stream->close(websocket::close_code::normal);
        webs->stopServer();
        helics::BrokerFactory::terminateAllBrokers();
        stream.reset();
    }

    static std::string sendText(const std::string& message)
    {
        // Send the message
        stream->write(net::buffer(message));
        // Receive the HTTP response
        // Clear the buffer
        buffer.consume(buffer.size());

        stream->read(buffer);
        std::string result{reinterpret_cast<const char*>(buffer.data().data()), buffer.size()};
        if (result.empty()) {
            return "#invalid";
        }
        return result;
    }

    static std::shared_ptr<helics::Broker> addBroker(helics::CoreType ctype,
                                                     const std::string& init)
    {
        auto brk = helics::BrokerFactory::create(ctype, init);
        if (brk) {
            brks.push_back(brk);
        }
        return brk;
    }

    static std::shared_ptr<helics::Core> addCore(helics::CoreType ctype, const std::string& init)
    {
        auto core = helics::CoreFactory::create(ctype, init);
        if (core) {
            cores.push_back(core);
        }
        return core;
    }
    static void clearBrokers()
    {
        for (auto& brk : brks) {
            if (brk) {
                brk->disconnect();
            }
        }
        brks.clear();
        helics::BrokerFactory::cleanUpBrokers();
    }
    // You can define per-test tear-down logic as usual.
    void TearDown() final {}

  private:
    // Some expensive resource shared by all tests.
    static std::shared_ptr<helics::apps::WebServer> webs;
    static net::io_context ioc;

    // These objects perform our I/O

    static std::unique_ptr<websocket::stream<tcp::socket>> stream;
    static beast::flat_buffer buffer;

    static std::vector<std::shared_ptr<helics::Broker>> brks;
    static std::vector<std::shared_ptr<helics::Core>> cores;
    static nlohmann::json config;
};

std::shared_ptr<helics::apps::WebServer> webTest::webs;

std::vector<std::shared_ptr<helics::Broker>> webTest::brks;
std::vector<std::shared_ptr<helics::Core>> webTest::cores;
beast::flat_buffer webTest::buffer;
std::unique_ptr<websocket::stream<tcp::socket>> webTest::stream;
net::io_context webTest::ioc;
nlohmann::json webTest::config;

#ifdef HELICS_ENABLE_ZMQ_CORE
constexpr helics::CoreType tCore = helics::CoreType::ZMQ;
#    define CORE1 "zmq"
#    ifdef HELICS_ENABLE_TCP_CORE
#        define CORE2 "TCP"
#    else
#        define CORE2 "ZMQ"
#    endif
#elif defined(HELICS_ENABLE_TCP_CORE)
constexpr helics::CoreType tCore = helics::CoreType::TCP;
#    define CORE1 "tcp"
#    ifdef HELICS_ENABLE_UDP_CORE
#        define CORE2 "UDP"
#    else
#        define CORE2 "TCP"
#    endif
#else
constexpr helics::CoreType tCore = helics::CoreType::TEST;
#    define CORE1 "test"
#    define CORE2 "TEST"
#endif

TEST_F(webTest, test1)
{
    nlohmann::json query;
    query["command"] = "query";
    query["query"] = "brokers";
    auto result = sendText(generateJsonString(query));
    EXPECT_FALSE(result.empty());
    auto val = loadJson(result);
    EXPECT_TRUE(val["brokers"].is_array());
    EXPECT_EQ(val["brokers"].size(), 0U);
}

TEST_F(webTest, single)
{
    addBroker(tCore, "--name=brk1");
    nlohmann::json query;
    query["command"] = "query";
    query["query"] = "brokers";
    auto result = sendText(generateJsonString(query));
    EXPECT_FALSE(result.empty());
    auto val = loadJson(result);
    EXPECT_TRUE(val["brokers"].is_array());
    EXPECT_EQ(val["brokers"].size(), 1U);
    EXPECT_EQ(val["brokers"][0]["name"].get<std::string>(), "brk1");
}

TEST_F(webTest, pair)
{
    addBroker(helics::CoreType::TEST, "--name=brk2");
    nlohmann::json query;
    query["command"] = "query";
    query["broker"] = "brokers";
    auto result = sendText(generateJsonString(query));

    EXPECT_FALSE(result.empty());
    auto val = loadJson(result);
    EXPECT_TRUE(val["brokers"].is_array());
    EXPECT_EQ(val["brokers"].size(), 2U);
    EXPECT_EQ(val["brokers"][0]["name"].get<std::string>(), "brk1");
    EXPECT_EQ(val["brokers"][1]["name"].get<std::string>(), "brk2");
}

TEST_F(webTest, single_info)
{
    nlohmann::json query;
    query["command"] = "query";
    query["broker"] = "brk1";
    auto result = sendText(generateJsonString(query));
    EXPECT_FALSE(result.empty());
    auto val = loadJson(result);
    EXPECT_TRUE(val["brokers"].is_array());
    EXPECT_EQ(val["brokers"].size(), 0U);
    EXPECT_EQ(val["cores"].size(), 0U);
    EXPECT_EQ(val["federates"].size(), 0U);
    EXPECT_EQ(val["attributes"]["name"].get<std::string>(), "brk1");
    EXPECT_EQ(val["state"].get<std::string>(), "connected");
}

TEST_F(webTest, garbage)
{
    auto result = sendText("garbage");
    auto val = loadJson(result);
    EXPECT_NE(val["status"].get<int>(), 0);
}

TEST_F(webTest, singleNonJson)
{
    nlohmann::json query;
    query["command"] = "query";
    query["broker"] = "brk1";
    query["query"] = "isconnected";
    auto result = sendText(generateJsonString(query));
    EXPECT_FALSE(result.empty());
    auto val = loadJson(result);
    EXPECT_EQ(val["status"].get<int>(), 0);
    EXPECT_EQ(val["value"].get<std::string>(), "true");
}

TEST_F(webTest, core)
{
    auto core = addCore(helics::CoreType::TEST, "--name=cr1 -f2");
    EXPECT_TRUE(core->connect());

    nlohmann::json query;
    query["command"] = "query";
    query["broker"] = "brk2";
    auto result = sendText(generateJsonString(query));
    EXPECT_FALSE(result.empty());
    auto val = loadJson(result);
    if (val["cores"].empty()) {
        // on occasion the core might not be registered with the broker in low CPU count systems
        // so we need to wait.
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        result = sendText(generateJsonString(query));
        EXPECT_FALSE(result.empty());
        val = loadJson(result);
    }
    ASSERT_EQ(val["cores"].size(), 1U);
    EXPECT_EQ(val["cores"][0]["attributes"]["name"].get<std::string>(), "cr1");

    query["target"] = "cr1";
    result = sendText(generateJsonString(query));
    val = loadJson(result);
    EXPECT_EQ(val["attributes"]["name"].get<std::string>(), "cr1");
    query["query"] = "current_state";

    auto result2 = sendText(generateJsonString(query));
    EXPECT_EQ(result, result2);
}

TEST_F(webTest, create)
{
    nlohmann::json create;
    create["command"] = "create";
    create["broker"] = "brk3";
    create["type"] = CORE2;
    create["args"] = "-f1";

    sendText(generateJsonString(create));

    nlohmann::json query;
    query["command"] = "query";
    query["broker"] = "brokers";
    auto result = sendText(generateJsonString(query));

    EXPECT_FALSE(result.empty());
    auto val = loadJson(result);
    EXPECT_TRUE(val["brokers"].is_array());
    EXPECT_EQ(val["brokers"].size(), 3U);
    EXPECT_EQ(val["brokers"][2]["name"].get<std::string>(), "brk3");

    result = sendText(generateJsonString(create));
    val = loadJson(result);
    EXPECT_NE(val["status"].get<int>(), 0);

    create["type"] = "NULLCORE";
    create["broker"] = "brk_null";
    result = sendText(generateJsonString(create));
    val = loadJson(result);
    EXPECT_NE(val["status"].get<int>(), 0);
    EXPECT_TRUE(result.find("not available") != std::string::npos);
}

TEST_F(webTest, deleteBroker)
{
    nlohmann::json cmd1;
    cmd1["command"] = "delete";
    cmd1["broker"] = "brk2";
    sendText(helics::fileops::generateJsonString(cmd1));

    nlohmann::json cmd2;
    cmd2["command"] = "query";
    cmd2["query"] = "brokers";
    auto result = sendText(generateJsonString(cmd2));
    EXPECT_FALSE(result.empty());
    auto val = helics::fileops::loadJson(result);
    EXPECT_TRUE(val["brokers"].is_array());
    EXPECT_EQ(val["brokers"].size(), 2U);
    cmd1["broker"] = "brk1";
    sendText(generateJsonString(cmd1));
    result = sendText(generateJsonString(cmd2));
    EXPECT_FALSE(result.empty());
    val = helics::fileops::loadJson(result);
    EXPECT_TRUE(val["brokers"].is_array());
    EXPECT_EQ(val["brokers"].size(), 1U);
}

TEST_F(webTest, createBrokerUUID)
{
    nlohmann::json cmd1;
    cmd1["command"] = "query";
    cmd1["query"] = "brokers";

    auto result = sendText(generateJsonString(cmd1));
    EXPECT_FALSE(result.empty());
    auto val = loadJson(result);
    EXPECT_TRUE(val["brokers"].is_array());
    auto brksize = val["brokers"].size();

    nlohmann::json cmd2;
    cmd2["command"] = "create";
    cmd2["core_type"] = CORE1;
    cmd2["num_feds"] = 3;
    result = sendText(generateJsonString(cmd2));
    val = loadJson(result);
    EXPECT_TRUE(val["broker_uuid"].is_string());
    auto uuid = val["broker_uuid"].get<std::string>();

    nlohmann::json cmd3;
    cmd3["command"] = "get";
    cmd3["uuid"] = uuid;

    result = sendText(generateJsonString(cmd3));
    val = loadJson(result);
    EXPECT_TRUE(val["status"].get<bool>());

    nlohmann::json cmd4;
    cmd4["command"] = "delete";
    cmd4["broker_uuid"] = uuid;

    sendText(generateJsonString(cmd4));

    result = sendText(generateJsonString(cmd1));
    EXPECT_FALSE(result.empty());
    val = loadJson(result);
    EXPECT_TRUE(val["brokers"].is_array());
    EXPECT_EQ(val["brokers"].size(), brksize);
}

TEST_F(webTest, deleteJson)
{
    nlohmann::json cmd1;
    cmd1["command"] = "delete";
    cmd1["broker"] = "brk3";
    sendText(generateJsonString(cmd1));

    nlohmann::json cmd2;
    cmd2["command"] = "query";
    cmd2["query"] = "brokers";
    auto result = sendText(generateJsonString(cmd2));
    EXPECT_FALSE(result.empty());
    auto val = loadJson(result);
    EXPECT_TRUE(val["brokers"].is_array());
    EXPECT_EQ(val["brokers"].size(), 0U);
}

TEST_F(webTest, timeBlock)
{
    nlohmann::json create;
    create["broker"] = "brk_timerws";
    create["command"] = "create";
    create["core_type"] = CORE1;
    create["num_feds"] = 1;
    sendText(generateJsonString(create));
    auto core = addCore(tCore, "--name=c_timer -f1 --broker=brk_timerws");
    EXPECT_TRUE(core->connect());

    helics::ValueFederate vFed("fed1", core);

    nlohmann::json barrier;
    barrier["broker"] = "brk_timerws";
    barrier["command"] = "barrier";
    barrier["time"] = 2;
    sendText(generateJsonString(barrier));

    vFed.enterExecutingMode();
    auto treq = vFed.requestTime(1.75);
    EXPECT_EQ(treq, 1.75);
    vFed.requestTimeAsync(3.0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(vFed.isAsyncOperationCompleted());
    barrier["time"] = 5.0;
    sendText(generateJsonString(barrier));
    auto rtime = vFed.requestTimeComplete();
    EXPECT_EQ(rtime, 3.0);

    vFed.requestTimeAsync(6.0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(vFed.isAsyncOperationCompleted());
    barrier["command"] = "clearbarrier";
    sendText(generateJsonString(barrier));
    rtime = vFed.requestTimeComplete();
    EXPECT_EQ(rtime, 6.0);

    vFed.finalize();
    nlohmann::json json;
    json["command"] = "delete";
    json["broker"] = "brk_timerws";
    sendText(generateJsonString(json));
}
