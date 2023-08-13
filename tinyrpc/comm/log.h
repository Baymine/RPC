#ifndef TINYRPC_COMM_LOG_H
#define TINYRPC_COMM_LOG_H

#include <sstream>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>
#include <memory>
#include <vector>
#include <queue>
#include <semaphore.h>
#include "tinyrpc/net/mutex.h"
#include "tinyrpc/comm/config.h"

namespace tinyrpc{
    extern tinyrpc::Config::ptr gRpcConfig;
    /*
    #define DebugLog \
        if(
            tinyrpc::OpenLog() \  // open a log file
            && \ 
            tinyrpc::LogLevel::DEBUG >= tinyrpc::gRpcConfig->m_log_level)\ // the minimal level that should be record

            tinyrpc::LogTmp(
                tinyrpc::LogEvent::ptr(
                    // The __FILE__, __LINE__, and __func__ are preprocessor macros in C++ that represent
                    // the current file name, line number, and function name, respectively.
                    // These are used to provide context for the log. 
                    
                    new tinyrpc::LogEvent(
                        tinyrpc::LogLevel::DEBUG, 
                        __FILE__, __LINE__, __func__,
                        tinyrpc::LogType::RPC_LOG  
                        ))).getStringStream()
    #define DebugLog \
        if( tinyrpc::OpenLog() && tinyrpc::LogLevel::DEBUG >= tinyrpc::gRpcConfig->m_log_level)tinyrpc::LogTmp(tinyrpc::LogEvent::ptr(new tinyrpc::LogEvent(tinyrpc::LogLevel::DEBUG, __FILE__, __LINE__, __func__,tinyrpc::LogType::RPC_LOG  ))).getStringStream()
    
    */

    enum LogType{
        RPC_LOG = 1,
        APP_LOG = 2,
    };


    #define TINYRPC_LOG(LEVEL, TYPE) \
        if (tinyrpc::OpenLog() && LEVEL >= (TYPE == tinyrpc::LogType::RPC_LOG ? tinyrpc::gRpcConfig->m_log_level : tinyrpc::gRpcConfig->m_app_log_level)) \
            tinyrpc::LogTmp(tinyrpc::LogEvent::ptr(new tinyrpc::LogEvent(LEVEL, __FILE__, __LINE__, __func__, TYPE))).getStringStream()

    #define DebugLog TINYRPC_LOG(tinyrpc::LogLevel::DEBUG, tinyrpc::LogType::RPC_LOG)
    #define InfoLog TINYRPC_LOG(tinyrpc::LogLevel::INFO, tinyrpc::LogType::RPC_LOG)
    #define WarnLog TINYRPC_LOG(tinyrpc::LogLevel::WARN, tinyrpc::LogType::RPC_LOG)
    #define ErrorLog TINYRPC_LOG(tinyrpc::LogLevel::ERROR, tinyrpc::LogType::RPC_LOG)

    #define AppDebugLog TINYRPC_LOG(tinyrpc::LogLevel::DEBUG, tinyrpc::LogType::APP_LOG)
    #define AppInfoLog TINYRPC_LOG(tinyrpc::LogLevel::INFO, tinyrpc::LogType::APP_LOG)
    #define AppWarnLog TINYRPC_LOG(tinyrpc::LogLevel::WARN, tinyrpc::LogType::APP_LOG)
    #define AppErrorLog TINYRPC_LOG(tinyrpc::LogLevel::ERROR, tinyrpc::LogType::APP_LOG)





    pid_t gettid();

    LogLevel stringToLevel(const std::string& str);

    std::string levelToString(LogLevel level);

    bool OpenLog();

    class LogEvent{
    public:
        typedef std::shared_ptr<LogEvent> ptr;
        LogEvent(
            LogLevel level, const char* file_name, int line, 
            const char* func_name, LogType type);

        ~LogEvent();

        std::stringstream& getStringStream();

        void log();

    private:
        // uint64_t m_timestamp;
        timeval m_timeval;
        LogLevel m_level;
        pid_t m_pid {0};
        pid_t m_tid {0};
        int m_cor_id {0};

        const char* m_file_name;
        int m_line {0};
        const char* m_func_name;
        LogType m_type;
        std::string m_msg_no;
        std::stringstream m_ss;
    };

    class LogTmp{
    public:
        explicit LogTmp(LogEvent::ptr event);

        ~LogTmp();

        std::stringstream& getStringStream();

    private:
        LogEvent::ptr m_event;
    };

    class AsyncLogger{
    public:
        typedef std::shared_ptr<AsyncLogger> ptr;

        AsyncLogger(const char* file_name, const char* file_path, int max_size, LogType logtype);
        ~AsyncLogger();

        void push(std::vector<std::string>& buffer);

        void flush();

        static void* execute(void*);

        void stop();

    public:
        std::queue<std::vector<std::string>> m_tasks;

    private:
        const char* m_file_name;
        const char* m_file_path;
        int m_max_size{0};
        LogType m_logtype;
        int m_no{0};
        bool m_need_handle{false};
        bool m_need_reopen {false};
        FILE* m_file_handle{nullptr};
        std::string m_date;

        Mutex m_mutex;
        pthread_cond_t m_condition;
        bool m_stop{false};

    public:
        pthread_t m_thread;
        sem_t m_semaphore;
    };

    class Logger{
    public:
        typedef std::shared_ptr<Logger> ptr;
        Logger();
        ~Logger();

        void init(const char* file_name, const char* file_path, int max_size, int sync_inteval);

        void log();

        void pushRpcLog(const std::string& log_msg);

        void pushAppLog(const std::string& log_msg);

        void loopFunc();

        void flush();

        void start();

        AsyncLogger::ptr getAsyncLogger(){
            return m_async_rpc_logger;
        }

        AsyncLogger::ptr getAsyncAppLogger(){
            return m_async_app_logger;
        }

    public:
        std::vector<std::string> m_buffer;
        std::vector<std::string> m_app_buffer;

    private:
        Mutex m_app_buffer_mutex;
        Mutex m_buff_mutex;
        bool m_is_init{false};
        AsyncLogger::ptr m_async_rpc_logger;
        AsyncLogger::ptr m_async_app_logger;

        int m_sync_inteval{0};
    };

    void Exit(int code);
}


#endif