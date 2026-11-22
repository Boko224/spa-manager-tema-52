#include "SpaService.h"

SpaService::SpaService()
    : m_name("Нова процедура"), m_type(ServiceType::Massage), m_duration(30), m_price(0),
      m_capacity(1), m_needsTherapist(false), m_timesBooked(0) {}

SpaService::SpaService(const std::string& name, ServiceType type, int duration, double price,
                       int capacity, bool needsTherapist)
    : m_name(name), m_type(type), m_duration(duration), m_price(price), m_capacity(capacity),
      m_needsTherapist(needsTherapist), m_timesBooked(0) {}

std::string SpaService::getTypeName() const {
    switch (m_type) {
        case ServiceType::Massage:     return "Масаж";
        case ServiceType::Sauna:       return "Сауна";
        case ServiceType::ThermalPool: return "Минерален басейн";
        case ServiceType::Therapy:     return "Терапия";
    }
    return "";
}
