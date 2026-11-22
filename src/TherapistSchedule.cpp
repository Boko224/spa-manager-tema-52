#include "TherapistSchedule.h"

TherapistSchedule::TherapistSchedule()
    : m_name("Нов терапевт"), m_specialty("Масаж"), m_status(TherapistStatus::Free) {}

TherapistSchedule::TherapistSchedule(const std::string& name, const std::string& specialty)
    : m_name(name), m_specialty(specialty), m_status(TherapistStatus::Free) {}

std::string TherapistSchedule::getStatusName() const {
    switch (m_status) {
        case TherapistStatus::Free:    return "свободен";
        case TherapistStatus::Busy:    return "зает";
        case TherapistStatus::OnBreak: return "в почивка";
    }
    return "";
}

void TherapistSchedule::reserve(int start, int end) {
    if (m_status == TherapistStatus::OnBreak) throw SpaException(m_name + " е в почивка.");
    for (const auto& slot : m_slots) {
        if (start < slot.second && slot.first < end) {
            throw SpaException("Дублиран час: " + m_name + " вече е зает от " + toTime(slot.first)
                               + " до " + toTime(slot.second) + ".");
        }
    }
    m_slots.push_back({start, end});
    m_status = TherapistStatus::Busy;
}
