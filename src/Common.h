#pragma once

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <stdexcept>
#include <string>

enum class ServiceType { Massage, Sauna, ThermalPool, Therapy };
enum class TherapistStatus { Free, Busy, OnBreak };

const int OPEN = 8 * 60;
const int CLOSE = 22 * 60;
const int HALF_DAY_START = 12 * 60;
const int HALF_DAY_END = 21 * 60;

const std::string DATA_DIR = "data/";

class SpaException : public std::runtime_error {
public:
    explicit SpaException(const std::string& message) : std::runtime_error(message) {}
};

inline std::string toTime(int minutes) {
    std::string m = std::to_string(minutes % 60);
    if (m.size() < 2){
         m = "0" + m;
    }
    return std::to_string(minutes / 60) + ":" + m;
}
