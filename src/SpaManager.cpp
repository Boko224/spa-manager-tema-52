#include "SpaManager.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

SpaManager::SpaManager() : SpaManager("СПА комплекс") {}

SpaManager::SpaManager(const string& name) : m_name(name) {
    m_services = {
        SpaService("Класически масаж", ServiceType::Massage, 60, 45, 3, true),
        SpaService("Финландска сауна", ServiceType::Sauna, 30, 12, 8, false),
        SpaService("Парна баня", ServiceType::Sauna, 30, 12, 6, false),
        SpaService("Голям минерален басейн", ServiceType::ThermalPool, 90, 18, 20, false),
        SpaService("Малък минерален басейн", ServiceType::ThermalPool, 60, 15, 2, false),
        SpaService("Рефлексотерапия", ServiceType::Therapy, 60, 42, 2, true),
    };
    m_therapists = {
        TherapistSchedule("Мария Иванова", "Масаж"),
        TherapistSchedule("Георги Петров", "Масаж"),
        TherapistSchedule("Елена Димитрова", "Терапия"),
    };
    m_packages = {{"Половин ден", 15}, {"Цял ден", 25}, {"Цял ден с обяд", 40}};
}

void SpaManager::registerClient(const string& firstName, const string& middleName, const string& phone,
                                const string& package, int stayStart) {
    if (firstName.empty() || middleName.empty() || phone.empty()) {
        throw SpaException("Името, презимето и телефонът са задължителни.");
    }
    if (m_packages.count(package) == 0) throw SpaException("Няма такъв пакет.");
    if (package == "Половин ден" && (stayStart < HALF_DAY_START || stayStart >= HALF_DAY_END)) {
        throw SpaException("Пакетът Половин ден е от " + toTime(HALF_DAY_START) + " до "
                           + toTime(HALF_DAY_END) + ".");
    }
    m_clients.push_back(SpaClient(firstName, middleName, phone, package, m_packages[package], stayStart));
}

void SpaManager::bookProcedure(int clientIndex, const string& serviceName, const string& therapistName,
                               int start) {
    SpaClient& client = m_clients.at(clientIndex);

    int serviceIndex = -1;
    for (size_t i = 0; i < m_services.size(); ++i) {
        if (m_services[i].getName() == serviceName) {
            serviceIndex = static_cast<int>(i);
            break;
        }
    }
    if (serviceIndex == -1) throw SpaException("Няма такава процедура.");

    SpaService& service = m_services[serviceIndex];
    const int end = start + service.getDuration();

    int stayEnd = CLOSE;
    if (client.getPackage() == "Половин ден") {
        stayEnd = HALF_DAY_END;
    }
    if (start < client.getStayStart() || end > stayEnd) {
        throw SpaException("Часът е извън престоя на клиента (" + toTime(client.getStayStart()) + " - "
                           + toTime(stayEnd) + ").");
    }
    if (client.isBusy(start, end)) throw SpaException("Клиентът вече има процедура по това време.");

    for (int minute = start; minute < end; ++minute) {
        if (occupancyAt(serviceName, minute) >= service.getCapacity()) {
            throw SpaException("Препълване: " + serviceName + " е пълна в " + toTime(minute) + ".");
        }
    }

    if (service.getNeedsTherapist()) {
        int therapistIndex = -1;
        for (size_t i = 0; i < m_therapists.size(); ++i) {
            if (m_therapists[i].getName() == therapistName) {
                therapistIndex = static_cast<int>(i);
                break;
            }
        }
        if (therapistIndex == -1) throw SpaException("Няма такъв терапевт.");

        TherapistSchedule& therapist = m_therapists[therapistIndex];
        if (therapist.getSpecialty() != service.getTypeName()) {
            throw SpaException(therapistName + " е със специалност " + therapist.getSpecialty()
                               + " и не може да извършва " + serviceName + ".");
        }
        therapist.reserve(start, end);
    }

    client.addBooking({serviceName, therapistName, start, end, service.getPrice()});
    service.addSale();
}

void SpaManager::toggleBreak(int therapistIndex) {
    TherapistSchedule& t = m_therapists.at(therapistIndex);
    if (t.getStatus() != TherapistStatus::OnBreak) {
        t.setStatus(TherapistStatus::OnBreak);
    } else {
        t.setStatus(t.getSlots().empty() ? TherapistStatus::Free : TherapistStatus::Busy);
    }
}

int SpaManager::occupancyAt(const string& serviceName, int minute) const {
    int count = 0;
    for (const SpaClient& client : m_clients) {
        for (const Booking& b : client.getBookings()) {
            if (b.service == serviceName && b.start <= minute && minute < b.end) ++count;
        }
    }
    return count;
}

vector<SpaService> SpaManager::getServicesSorted(bool byRevenue) const {
    vector<SpaService> sorted = m_services;
    sort(sorted.begin(), sorted.end(), [byRevenue](const SpaService& a, const SpaService& b) {
        return byRevenue ? a.getRevenue() > b.getRevenue() : a.getTimesBooked() > b.getTimesBooked();
    });
    return sorted;
}

void SpaManager::printReport(ostream& out) const {
    double packages = 0, total = 0;
    for (const SpaClient& client : m_clients) {
        packages += client.getPackagePrice();
        total += client.getBill();
    }
    out << "===== ДНЕВЕН ФИНАНСОВ ОТЧЕТ: " << m_name << " =====\n";
    out << "Посетители: " << m_clients.size() << "\n";
    for (const SpaService& s : getServicesSorted(true)) {
        out << s.getName() << " | брой: " << s.getTimesBooked() << " | приход: " << s.getRevenue() << " EUR\n";
    }
    out << "Приходи от пакети: " << packages << " EUR\n";
    out << "Приходи от процедури: " << total - packages << " EUR\n";
    out << "ОБЩ ПРИХОД: " << total << " EUR\n";
}

void SpaManager::save() const {
    ofstream registerFile(DATA_DIR + "register.csv");
    ofstream salesFile(DATA_DIR + "sales.csv");
    if (!registerFile || !salesFile) throw SpaException("Файловете не могат да се запишат.");

    for (size_t i = 0; i < m_clients.size(); ++i) {
        const SpaClient& c = m_clients[i];
        registerFile << c.getFirstName() << ';' << c.getMiddleName() << ';' << c.getPhone() << ';'
                     << c.getPackage() << ';' << c.getStayStart() << '\n';
        for (const Booking& b : c.getBookings()) {
            salesFile << i << ';' << b.service << ';' << b.therapist << ';' << b.start << ';' << b.price << '\n';
        }
    }
}

static vector<string> split(const string& line) {
    vector<string> fields;
    string field;
    istringstream in(line);
    while (getline(in, field, ';')) fields.push_back(field);
    return fields;
}

void SpaManager::load() {
    string line;
    vector<int> loadedIndex;
    ifstream registerFile(DATA_DIR + "register.csv");
    while (getline(registerFile, line)) {
        try {
            const vector<string> f = split(line);
            registerClient(f.at(0), f.at(1), f.at(2), f.at(3), stoi(f.at(4)));
            loadedIndex.push_back(static_cast<int>(m_clients.size()) - 1);
        } catch (const exception& e) {
            loadedIndex.push_back(-1);
            cout << "Пропуснат ред в register.csv: " << e.what() << "\n";
        }
    }
    ifstream salesFile(DATA_DIR + "sales.csv");
    while (getline(salesFile, line)) {
        try {
            const vector<string> f = split(line);
            const int fileIndex = stoi(f.at(0));
            if (fileIndex < 0 || fileIndex >= static_cast<int>(loadedIndex.size())
                || loadedIndex[fileIndex] == -1) {
                throw SpaException("клиентът не е зареден.");
            }
            bookProcedure(loadedIndex[fileIndex], f.at(1), f.at(2), stoi(f.at(3)));
        } catch (const exception& e) {
            cout << "Пропуснат ред в sales.csv: " << e.what() << "\n";
        }
    }
}
