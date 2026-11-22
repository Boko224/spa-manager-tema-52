#pragma once

#include <string>
#include "Common.h"

class SpaService {
private:
    std::string m_name;
    ServiceType m_type;
    int m_duration;
    double m_price;
    int m_capacity;
    bool m_needsTherapist;
    int m_timesBooked;

public:
    SpaService();
    SpaService(const std::string& name, ServiceType type, int duration, double price, int capacity,
               bool needsTherapist);

    std::string getName() const {
        return m_name;
    }
    ServiceType getType() const {
        return m_type;
    }
    int getDuration() const {
        return m_duration;
    }
    double getPrice() const {
        return m_price;
    }
    int getCapacity() const {
        return m_capacity;
    }
    bool getNeedsTherapist() const {
        return m_needsTherapist;
    }
    int getTimesBooked() const {
        return m_timesBooked;
    }
    double getRevenue() const {
        return m_timesBooked * m_price;
    }
    std::string getTypeName() const;

    void setName(const std::string& name) {
        m_name = name;
    }
    void setType(ServiceType type) {
        m_type = type;
    }
    void setDuration(int duration) {
        m_duration = duration;
    }
    void setPrice(double price) {
        m_price = price;
    }
    void setCapacity(int capacity) {
        m_capacity = capacity;
    }
    void setNeedsTherapist(bool needs) {
        m_needsTherapist = needs;
    }
    void addSale() {
        ++m_timesBooked;
    }
};
