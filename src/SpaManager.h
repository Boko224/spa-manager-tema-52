#pragma once

#include <map>
#include <ostream>
#include <string>
#include <vector>
#include "SpaClient.h"
#include "SpaService.h"
#include "TherapistSchedule.h"

class SpaManager {
private:
    std::string m_name;
    std::vector<SpaService> m_services;
    std::vector<TherapistSchedule> m_therapists;
    std::vector<SpaClient> m_clients;
    std::map<std::string, double> m_packages;

public:
    SpaManager();
    explicit SpaManager(const std::string& name);

    std::string getName() const {
        return m_name;
    }
    const std::vector<SpaService>& getServices() const {
        return m_services;
    }
    const std::vector<TherapistSchedule>& getTherapists() const {
        return m_therapists;
    }
    const std::vector<SpaClient>& getClients() const {
        return m_clients;
    }
    const std::map<std::string, double>& getPackages() const {
        return m_packages;
    }
    void setName(const std::string& name) {
        m_name = name;
    }

    void registerClient(const std::string& firstName, const std::string& middleName,
                        const std::string& phone, const std::string& package, int stayStart);
    void bookProcedure(int clientIndex, const std::string& serviceName,
                       const std::string& therapistName, int start);
    void toggleBreak(int therapistIndex);

    int occupancyAt(const std::string& serviceName, int minute) const;
    std::vector<SpaService> getServicesSorted(bool byRevenue) const;
    void printReport(std::ostream& out) const;

    void save() const;
    void load();
};
