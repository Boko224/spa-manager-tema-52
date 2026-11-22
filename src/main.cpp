#ifdef _WIN32
#include <windows.h>
#endif

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "SpaManager.h"

using namespace std;

string ask(const string& prompt) {
    cout << prompt;
    string line;
    if (!getline(cin, line)){
        exit(0);
    }
    return line;
}

int askNumber(const string& prompt, int min, int max) {
    try {
        const int value = stoi(ask(prompt));
        if (value >= min && value <= max) {
            return value;
        }
    } catch (const exception&) {
    }
    throw SpaException("Въведете число от " + to_string(min) + " до " + to_string(max) + ".");
}

int chooseClient(const SpaManager& spa) {
    const vector<SpaClient>& clients = spa.getClients();
    if (clients.empty()) throw SpaException("Няма регистрирани посетители.");
    for (size_t i = 0; i < clients.size(); ++i) {
        cout << i + 1 << ". " << clients[i].getFirstName() << " " << clients[i].getMiddleName()
             << " | сметка: " << clients[i].getBill() << " EUR\n";
    }
    return askNumber("Посетител: ", 1, static_cast<int>(clients.size())) - 1;
}

void registerVisitor(SpaManager& spa) {
    const string firstName = ask("Име (на латиница): ");
    const string middleName = ask("Презиме (на латиница): ");
    const string phone = ask("Телефон: ");

    vector<string> names;
    for (const auto& package : spa.getPackages()) {
        names.push_back(package.first);
        cout << names.size() << ". " << package.first << " - " << package.second << " EUR\n";
    }
    const string package = names[askNumber("Пакет: ", 1, static_cast<int>(names.size())) - 1];
    int hour;
    if (package == "Половин ден") {
        hour = askNumber("Начален час на престоя (12-20): ", 12, 20);
    } else {
        hour = askNumber("Начален час на престоя (8-21): ", 8, 21);
    }

    spa.registerClient(firstName, middleName, phone, package, hour * 60);
    cout << "Посетителят е регистриран.\n";
}

void showTherapists(const SpaManager& spa) {
    const vector<TherapistSchedule>& therapists = spa.getTherapists();
    for (size_t i = 0; i < therapists.size(); ++i) {
        cout << i + 1 << ". " << therapists[i].getName() << " | " << therapists[i].getSpecialty()
             << " | " << therapists[i].getStatusName() << " |";
        for (const auto& slot : therapists[i].getSlots()) {
            cout << " " << toTime(slot.first) << "-" << toTime(slot.second);
        }
        cout << "\n";
    }
}

void bookProcedure(SpaManager& spa) {
    const int client = chooseClient(spa);

    const vector<SpaService>& services = spa.getServices();
    for (size_t i = 0; i < services.size(); ++i) {
        cout << i + 1 << ". " << services[i].getName() << " | " << services[i].getTypeName() << " | "
             << services[i].getDuration() << " мин | " << services[i].getPrice() << " EUR\n";
    }
    const SpaService& service = services[askNumber("Процедура: ", 1, static_cast<int>(services.size())) - 1];
    const int hour = askNumber("Начален час (8-21): ", 8, 21);

    string therapist;
    if (service.getNeedsTherapist()) {
        showTherapists(spa);
        therapist = spa.getTherapists()[askNumber("Терапевт: ", 1, static_cast<int>(spa.getTherapists().size())) - 1].getName();
    }

    spa.bookProcedure(client, service.getName(), therapist, hour * 60);
    cout << "Часът е запазен. Сметка: " << spa.getClients()[client].getBill() << " EUR\n";
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::filesystem::create_directories(DATA_DIR);
    SpaManager spa;
    spa.load();

    while (true) {
        cout << "\n===== " << spa.getName() << " =====\n"
             << "1. Регистрация и продажба на пакет\n"
             << "2. Запазване на час за процедура\n"
             << "3. Заетост на зоните и басейните\n"
             << "4. Графици на терапевтите\n"
             << "5. Почивка на терапевт (вкл./изкл.)\n"
             << "6. Фактура/ваучер\n"
             << "7. Сортиране на процедурите\n"
             << "8. Дневен финансов отчет\n"
             << "0. Изход\n";
        try {
            const int choice = askNumber("Избор: ", 0, 8);
            if (choice == 0) break;
            if (choice == 1) { registerVisitor(spa); spa.save(); }
            if (choice == 2) { bookProcedure(spa); spa.save(); }
            if (choice == 3) {
                const int hour = askNumber("Заетост към час (8-21): ", 8, 21);
                for (const SpaService& s : spa.getServices()) {
                    cout << s.getName() << ": " << spa.occupancyAt(s.getName(), hour * 60) << " от "
                         << s.getCapacity() << " места\n";
                }
            }
            if (choice == 4) showTherapists(spa);
            if (choice == 5) {
                showTherapists(spa);
                spa.toggleBreak(askNumber("Терапевт: ", 1, static_cast<int>(spa.getTherapists().size())) - 1);
            }
            if (choice == 6) cout << spa.getClients()[chooseClient(spa)];
            if (choice == 7) {
                const bool byRevenue = askNumber("1. По популярност  2. По приход: ", 1, 2) == 2;
                for (const SpaService& s : spa.getServicesSorted(byRevenue)) {
                    cout << s.getName() << " | брой: " << s.getTimesBooked() << " | приход: "
                         << s.getRevenue() << " EUR\n";
                }
            }
            if (choice == 8) {
                spa.printReport(cout);
                ofstream file(DATA_DIR + "report.txt");
                spa.printReport(file);
                cout << "Отчетът е записан в " << DATA_DIR << "report.txt.\n";
            }
        } catch (const SpaException& e) {
            cout << "ГРЕШКА: " << e.what() << "\n";
        }
    }
    return 0;
}
