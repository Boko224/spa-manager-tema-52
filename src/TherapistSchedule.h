#pragma once

#include <string>
#include <utility>
#include <vector>
#include "Common.h"

class TherapistSchedule {
private:
    std::string m_name;
    std::string m_specialty;
    std::vector<std::pair<int, int>> m_slots;
    TherapistStatus m_status;

public:
    TherapistSchedule();
    TherapistSchedule(const std::string& name, const std::string& specialty);

    std::string getName() const {
        return m_name;
    }
    std::string getSpecialty() const {
        return m_specialty;
    }
    const std::vector<std::pair<int, int>>& getSlots() const {
        return m_slots;
    }
    TherapistStatus getStatus() const {
        return m_status;
    }
    std::string getStatusName() const;

    void setName(const std::string& name) {
        m_name = name;
    }
    void setSpecialty(const std::string& specialty) {
        m_specialty = specialty;
    }
    void setStatus(TherapistStatus status) {
        m_status = status;
    }

    void reserve(int start, int end);
};
