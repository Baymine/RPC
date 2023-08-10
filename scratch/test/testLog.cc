#include <gtest/gtest.h>
#include "tinyrpc/comm/log.h"


bool mockOpenLog = false;
tinyrpc::LogLevel mockLogLevel = tinyrpc::LogLevel::DEBUG;

bool OpenLogMock() {
    return mockOpenLog;
}

#define tinyrpc::OpenLog OpenLogMock

// Assume that gRpcConfig is a global pointer to a configuration object in tinyrpc. 
extern decltype(tinyrpc::gRpcConfig) gRpcConfigMock;  // If it's not a pointer, adjust this declaration.

#define tinyrpc::gRpcConfig gRpcConfigMock  // Redirect the macro to use our mock

TEST(DebugLogTest, LogOpened) {
    mockOpenLog = true;
    gRpcConfigMock->m_log_level = mockLogLevel;  // Set the log level here.

    DebugLog << "test log";  // Use the macro as intended.
    
    // Here, you'll need a way to check if "test log" was actually added to your log system.
    // This check depends on the internals of tinyrpc.
}

// ... Repeat for other test cases ...



