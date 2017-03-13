//
//  Logger.h
//
//  by jiahuan.liu
//  10/12/2016
//

#ifndef _LOGGER_H
#define _LOGGER_H

#include <iostream>
#include <sstream>
#include <functional>
#include <string>

#include "Common.h"
#include "Exception.h"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

typedef std::ostringstream Stream;

#define LOG std::cout
#define END std::endl

class LLogger
{
public:
    //output file error
    struct Exception: public ::Exception
    {
        Exception(const std::string& strerr) : ::Exception(strerr) {}
        Exception(const std::string& strerr, int ierr)
        : ::Exception(strerr, ierr) {}
        ~Exception() noexcept {}
    };
public:
    enum Level
    {
        L_DEBUG,
        L_INFO,
        L_WARN,
        L_ERROR,
        L_FATAL,
        L_DATA
    };
    static std::string levelName[];
    //log to default output
    LLogger(std::string file, int line, Level level = L_INFO);
    LLogger(std::string file, int line, std::string func, Level level);
    //log to file output
    LLogger(FILE* ofile, std::string file, int line, Level level = L_INFO);
    LLogger(FILE* ofile, std::string file, int line, std::string func, Level level);
    LLogger(FILE* ofile);
    ~LLogger();
    //encode beginning and ending of a log line
    void init(Level level, std::string file, int line);
    void finish();
    
    Stream& stream() {return _stream;}
protected:
    //stdout
    static void defaultOutput(const std::string& str);
    //throw if file not opened or error
    static void fileOutput(FILE* file, const std::string& str);
protected:
    Stream _stream;
    std::function<void(const std::string&)> _output;
};

#define LOG_DEBUG LLogger(__FILENAME__, __LINE__, __func__, LLogger::Level::L_DEBUG).stream()
#define LOG_INFO LLogger(__FILENAME__, __LINE__, LLogger::Level::L_INFO).stream()
#define LOG_WARN LLogger(__FILENAME__, __LINE__, LLogger::Level::L_WARN).stream()
#define LOG_ERROR LLogger(__FILENAME__, __LINE__, LLogger::Level::L_ERROR).stream()
#define LOG_FATAL LLogger(__FILENAME__, __LINE__, LLogger::Level::L_FATAL).stream()

#define FLOG_DEBUG(x) LLogger(x, __FILENAME__, __LINE__, __func__, LLogger::Level::L_DEBUG).stream()
#define FLOG_INFO(x) LLogger(x, __FILENAME__, __LINE__, LLogger::Level::L_INFO).stream()
#define FLOG_WARN(x) LLogger(x, __FILENAME__, __LINE__, LLogger::Level::L_WARN).stream()
#define FLOG_ERROR(x) LLogger(x, __FILENAME__, __LINE__, LLogger::Level::L_ERROR).stream()
#define FLOG_FATAL(x) LLogger(x, __FILENAME__, __LINE__, LLogger::Level::L_FATAL).stream()
#define FLOG_DATA(x) LLogger(x).stream()

#endif//_LOGGER_H
