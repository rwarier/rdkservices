/**
 * If not stated otherwise in this file or this component's LICENSE
 * file the following copyright and licenses apply:
 *
 * Copyright 2022 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#include "gtest/gtest.h"
#include "Timer.h"
#include "ServiceMock.h"

using namespace WPEFramework;

class TimerTestFixture : public ::testing::Test {
protected:
    Core::ProxyType<Plugin::Timer> plugin;
    Core::JSONRPC::Handler& handler;
    Core::JSONRPC::Connection connection;
    string response;
    ServiceMock service;

    TimerTestFixture()
        : plugin(Core::ProxyType<Plugin::Timer>::Create())
        , handler(*(plugin))
        , connection(1, 0)
    {
    }
    virtual ~TimerTestFixture()
    {
    }
<<<<<<< HEAD

    Core::ProxyType<TimerTestMock> plugin;
    Core::JSONRPC::Handler& handler;
    Core::JSONRPC::Connection connection;
<<<<<<< HEAD
=======
    string response;
>>>>>>> 89632f91 (RDK-36576: Add Unit Tests for RDKServices Timer Plugin)
=======
>>>>>>> b21f3702 (Modified to use ServiceMock)
};

TEST_F(TimerTestFixture, registeredMethods)
{
<<<<<<< HEAD
    GTEST_SKIP();
}
=======
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("startTimer")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("cancel")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("suspend")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("resume")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getTimerStatus")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getTimers")));
}

TEST_F(TimerTestFixture, paramsMissing)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("startTimer"), _T("{}"), response));
    EXPECT_EQ(response, _T("{\"success\":false}"));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("cancel"), _T("{}"), response));
    EXPECT_EQ(response, _T("{\"success\":false}"));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("suspend"), _T("{}"), response));
    EXPECT_EQ(response, _T("{\"success\":false}"));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("resume"), _T("{}"), response));
    EXPECT_EQ(response, _T("{\"success\":false}"));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getTimerStatus"), _T("{}"), response));
    EXPECT_EQ(response, _T("{\"success\":false}"));
}

TEST_F(TimerTestFixture, jsonRpc)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("startTimer"), _T("{\"interval\":10}"), response));
    //EXPECT_THAT(response, testing::MatchesRegex("timerId*"));
    EXPECT_THAT(response, testing::HasSubstr("timerId"));
    EXPECT_THAT(response, testing::HasSubstr("\"success\":true"));

    //Extract timer id from response
    JsonObject params;
    EXPECT_TRUE(params.FromString(response));
    EXPECT_TRUE(params.HasLabel(_T("timerId")));
    string stimerID = params["timerId"].String();
    std::string str = "{\"timerId\":" + stimerID + "}";

    //get timer status
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getTimerStatus"), str.c_str(), response));
    EXPECT_THAT(response, testing::HasSubstr("\"success\":true"));

    //get all timers
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getTimers"), _T("{}"), response));
    EXPECT_THAT(response, testing::HasSubstr("timerId"));

    //Suspend the timer
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("suspend"), str.c_str(), response));
    EXPECT_EQ(response, _T("{\"success\":true}"));

    //Resume the timer
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("resume"), str.c_str(), response));
    EXPECT_EQ(response, _T("{\"success\":true}"));

    //Cancel the timer
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("cancel"), str.c_str(), response));
    EXPECT_EQ(response, _T("{\"success\":true}"));
}

TEST_F(TimerTestFixture, timerExpiry)
{
    // called by Timer::sendTimerExpired
    EXPECT_CALL(service, Submit(::testing::_, ::testing::_))
        .Times(1)
        .WillOnce(::testing::Invoke(
            [&](const uint32_t, const Core::ProxyType<Core::JSON::IElement>& json) {
                string text;
                EXPECT_TRUE(json->ToString(text));
                EXPECT_EQ(text, string(_T("{"
                                          "\"jsonrpc\":\"2.0\","
                                          "\"method\":\"client.events.1.timerExpired\","
                                          "\"params\":{\"timerId\":0,"
                                           "\"mode\":\"GENERIC\","
                                           "\"status\":0}"
                                          "}")));

                return Core::ERROR_NONE;
            }));


    // Initialize
    EXPECT_EQ(string(""), plugin->Initialize(nullptr));

    //Create a timer of 1 sec with a reminder of 0.2 sec
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("startTimer"), 
                        _T("{\"interval\":1, \"remindBefore\": 0.2}"), response));
    EXPECT_THAT(response, testing::HasSubstr("timerId"));
    EXPECT_THAT(response, testing::HasSubstr("\"success\":true"));

    //Extract timer id from response
    JsonObject params;
    EXPECT_TRUE(params.FromString(response));
    EXPECT_TRUE(params.HasLabel(_T("timerId")));
<<<<<<< HEAD
<<<<<<< HEAD
    string stimerID = params["timerId"].String();
    int nTimerId = std::stoi(stimerID);
=======
    string stimerId = params["timerId"].String();
    int nTimerId = std::stoi(stimerId);
>>>>>>> c26ed4e4 (RDK-36576 : Add Unit Tests for RDKServices Timer Plugin)
=======
//    string stimerId = params["timerId"].String();
//    int nTimerId = std::stoi(stimerId);
>>>>>>> b21f3702 (Modified to use ServiceMock)


    // JSON-RPC events
    auto dispatcher = static_cast<PluginHost::IDispatcher*>(
        plugin->QueryInterface(PluginHost::IDispatcher::ID));
    EXPECT_TRUE(dispatcher != nullptr);

    dispatcher->Activate(&service);

<<<<<<< HEAD
=======
    //Wait for timer expiry & reminder calls
>>>>>>> c26ed4e4 (RDK-36576 : Add Unit Tests for RDKServices Timer Plugin)
   sleep(2);

    dispatcher->Deactivate();
    dispatcher->Release();

    // Deinitialize
    plugin->Deinitialize(nullptr);

}

>>>>>>> 89632f91 (RDK-36576: Add Unit Tests for RDKServices Timer Plugin)
