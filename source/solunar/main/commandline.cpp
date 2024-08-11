#include "pch.h"
#include "main/main.h"

namespace solunar
{

CommandLine g_commandLine;
    
void CommandLine::Init(int argc, char* argv[])
{
    m_argc = argc;
    m_argv = argv;
}

void CommandLine::Init(const char* str)
{
    
}

bool CommandLine::hasOption(const char* name)
{
    for (int i = 0; i < m_argc; i++) {
        if (m_argv[i]) {
            if (strcmp(m_argv[i], name) == 0)
                return true;
        }
    }

    return false;
}

const char* CommandLine::getOptionParameter(const char* name)
{
    for (int i = 0; i < m_argc; i++) {
        if (m_argv[i]) {
            if (strcmp(m_argv[i], name) == 0) {
                if (!m_argv[i + 1]) {
                    printf("Commandline option %s doesn't have parameter!", name);
                }
                else {
                    return m_argv[i + 1];
                }
            }
        }
    }

    return nullptr;
}

std::string CommandLine::getCommandLine()
{
    std::string buffer;

    for (int i = 0; i < m_argc; i++)
    {
        if (m_argv[i])
        {
            buffer += m_argv[i];
            buffer += " ";
        }
    }

    return buffer;
}
    
}
