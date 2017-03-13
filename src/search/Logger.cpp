//
//  Logger.cpp
//
//  by jiahuan.liu
//  11/24/2016
//

#include "Logger.h"

std::string LLogger::levelName[] =
{
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

LLogger::LLogger(std::string file, int line, Level level)
:_output(defaultOutput)
{
    init(level, file, line);
    _stream << "]";
}

LLogger::LLogger(std::string file, int line, std::string func, Level level)
:_output(defaultOutput)
{
    init(level, file, line);
    _stream << func << "]";
}

LLogger::LLogger(FILE* ofile, std::string file, int line, Level level)
{
    init(level, file, line);
    _stream << "]";
    _output = std::bind(fileOutput, ofile, std::placeholders::_1);
}

LLogger::LLogger(FILE* ofile, std::string file, int line, std::string func, Level level)
{
    init(level, file, line);
    _stream<< func << "]";
    _output = std::bind(fileOutput, ofile, std::placeholders::_1);
}

LLogger::LLogger(FILE* ofile)
{
    _output = std::bind(fileOutput, ofile, std::placeholders::_1);
}

void LLogger::init(Level level, std::string file, int line)
{
    if (level == L_DATA) return;
    _stream << "[" << Common::now2str("%H:%M:%S") << "][" << levelName[level]
    << "][" << file << ":" << line;
}

void LLogger::finish()
{
    _stream << END;
}

LLogger::~LLogger()
{
    finish();
    _output(_stream.str());
}

void LLogger::defaultOutput(const std::string& str)
{
    fwrite(str.c_str(), 1, str.length(), stdout);
}

void LLogger::fileOutput(FILE* file, const std::string& str)
{
    size_t n = fwrite(str.c_str(), 1, str.length(), file);
    if (file == NULL || n != str.length())
    {
        throw Exception("logger output file error.");
    }
}