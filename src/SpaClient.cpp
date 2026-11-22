#include "SpaClient.h"

using namespace std;

SpaClient::SpaClient() : m_packagePrice(0), m_stayStart(OPEN), m_bill(0) {}

SpaClient::SpaClient(const string& firstName, const string& middleName, const string& phone,
                     const string& package, double packagePrice, int stayStart)
    : m_firstName(firstName), m_middleName(middleName), m_phone(phone), m_package(package),
      m_packagePrice(packagePrice), m_stayStart(stayStart), m_bill(packagePrice) {}

bool SpaClient::isBusy(int start, int end) const {
    for (const Booking& b : m_bookings) {
        if (start < b.end && b.start < end) {
            return true;
        }
    }
    return false;
}

void SpaClient::addBooking(const Booking& booking) {
    m_bookings.push_back(booking);
    m_bill += booking.price;
}

ostream& operator<<(ostream& out, const SpaClient& client) {
    out << "========== ФАКТУРА / ВАУЧЕР ==========\n";
    out << "Клиент: " << client.m_firstName << " " << client.m_middleName << "\n";
    out << "Телефон: " << client.m_phone << "\n";
    out << "Начало на престоя: " << toTime(client.m_stayStart) << "\n";
    out << "Пакет: " << client.m_package << " - " << client.m_packagePrice << " EUR\n";
    out << "--- График на процедурите ---\n";
    for (const Booking& b : client.m_bookings) {
        out << toTime(b.start) << "-" << toTime(b.end) << " | " << b.service;
        if (!b.therapist.empty()) out << " | терапевт: " << b.therapist;
        out << " | " << b.price << " EUR\n";
    }
    out << "ОБЩО ЗА ПЛАЩАНЕ: " << client.m_bill << " EUR\n";
    out << "======================================\n";
    return out;
}
