#pragma once

#include <ostream>
#include <string>
#include <vector>
#include "Common.h"

struct Booking {
    std::string service;
    std::string therapist;
    int start;
    int end;
    double price;
};

class SpaClient {
private:
    std::string m_firstName;
    std::string m_middleName;
    std::string m_phone;
    std::string m_package;
    double m_packagePrice;
    std::vector<Booking> m_bookings;
    int m_stayStart;
    double m_bill;

public:
    SpaClient();
    SpaClient(const std::string& firstName, const std::string& middleName, const std::string& phone,
              const std::string& package, double packagePrice, int stayStart);

    std::string getFirstName() const {
        return m_firstName;
    }
    std::string getMiddleName() const {
        return m_middleName;
    }
    std::string getPhone() const {
        return m_phone;
    }
    std::string getPackage() const {
        return m_package;
    }
    double getPackagePrice() const {
        return m_packagePrice;
    }
    const std::vector<Booking>& getBookings() const {
        return m_bookings;
    }
    int getStayStart() const {
        return m_stayStart;
    }
    double getBill() const {
        return m_bill;
    }

    void setFirstName(const std::string& name) {
        m_firstName = name;
    }
    void setMiddleName(const std::string& name) {
        m_middleName = name;
    }
    void setPhone(const std::string& phone) {
        m_phone = phone;
    }
    void setStayStart(int minute) {
        m_stayStart = minute;
    }

    bool isBusy(int start, int end) const;
    void addBooking(const Booking& booking);

    friend std::ostream& operator<<(std::ostream& out, const SpaClient& client);
};
