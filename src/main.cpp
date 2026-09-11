
#include "logger.hpp"

#include <atomic>
#include <csignal>
#include <iostream>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

std::atomic<bool> running{true};

static void onSignal(int) { running.store(false); }

void init()
{
    LOG_DEBUG("Main", "Starting up…");
    LOG_INFO("Main", "Robot initialised");
    LOG_WARNING("MotorController", "Left encoder jitter detected");
    LOG_ERROR("PowerModule", "Battery voltage critical");
}
namespace fs = std::filesystem;

int main()
{
    std::signal(SIGINT, onSignal);
    std::signal(SIGTERM, onSignal);

    // Get current time
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::tm localTime = *std::localtime(&time);

    // Get current user's home directory
    const char *home = std::getenv("HOME");

    std::ostringstream timestamp;
    timestamp << std::put_time(&localTime, "%Y%m%d_%H%M%S");

    std::string value = timestamp.str();

    // Create date directory: 27_sep_2016
    std::ostringstream date;
    date << std::put_time(&localTime, "%d_%b_%Y");

    std::string logDir = std::string(home) + "/logs/mcu/" + date.str();

    // Create directory if it doesn't exist
    fs::create_directories(logDir);

    // Create log file
    std::string logFile = logDir + "/" + value + ".log";

    std::ofstream file(logFile, std::ios::app);

    // if (!file.is_open())
    // {
    //     return 1;
    // }

    // file << "Log file created/opened\n";

    // file.close();

    // default folder ($HOME/log/robot.log)

    // Logger::getInstance().configure(Logger::LogLevel::DEBUG, "robot.log", false);

    // Pass an absolute path directly as the filename, no logDir needed

    Logger::getInstance().configure(Logger::LogLevel::DEBUG, logFile, false, "", false);

    Logger &log = Logger::getInstance();

    LOG_DEBUG("Main", "Starting up…");

    LOG_INFO("Main", "Robot initialised");
    LOG_WARNING("MotorController", "Left encoder jitter detected");
    LOG_ERROR("PowerModule", "Battery voltage critical");
    return 0;
}
