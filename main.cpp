#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <memory>
 
 //Abstrakcyjna klasa Log
 class Log {
public:
    virtual ~Log() = default;
    virtual std::string getMessage() const = 0;  // Metoda do pobierania wiadomości logu
};

// Klasa reprezentująca log info
class InfoLog : public Log {
private:
    std::string message;
public:
    InfoLog(const std::string& msg) : message(msg) {}
    std::string getMessage() const override {
        return "[INFO]: " + message;  // Sformatowana wiadomość
    }
};

// Klasa reprezentująca log ostrzeżenia
class WarningLog : public Log {
private:
    std::string message;
public:
    WarningLog(const std::string& msg) : message(msg) {}
    std::string getMessage() const override {
        return "[WARNING]: " + message;  // Sformatowana wiadomość
    }
};

// Klasa reprezentująca log błędu
class ErrorLog : public Log {
private:
    std::string message;
public:
    ErrorLog(const std::string& msg) : message(msg) {}
    std::string getMessage() const override {
        return "[ERROR]: " + message;  // Sformatowana wiadomość
    }
};

// Fabryka logów
class LogFactory {
public:
    static std::unique_ptr<Log> createLog(const std::string& type, const std::string& message) {
        if (type == "info") {
            return std::make_unique<InfoLog>(message);
        } else if (type == "warning") {
            return std::make_unique<WarningLog>(message);
        } else if (type == "error") {
            return std::make_unique<ErrorLog>(message);
        }
        return nullptr;  // Jeśli typ jest nieznany
    }
};


//Singleton Logger
class Logger {
private:
    Logger() {}
    std::vector<std::string> logs;
    std::mutex mutex;
    static Logger* instance;
 
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    //Statyczna metoda zwracająca instancję logegra
    static Logger& getInstance() {
        if (instance == nullptr) {
            instance = new Logger();
        }
        return *instance;
    }
    //Logowanie wiadomości przy użyciu fabryki
    void logMessage(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex);
        logs.push_back(message);
    }
    //Wyświetlanie logów
    void displayLogs() {
        std::lock_guard<std::mutex> lock(mutex);
        for (const auto& log : logs) {
            std::cout << log << std::endl;
        }
    }
};
 
// Inicjalizacja instancji Loggera
Logger* Logger::instance = nullptr;

int main() {
    // Pobranie instancji loggera
    Logger& logger = Logger::getInstance();

    while (true) {
        std::cout << "Menu:\n";
        std::cout << "1. Dodaj log\n";
        std::cout << "2. Wyświetl logi\n";
        std::cout << "3. Wyjście\n";
        std::cout << "Wybierz opcję: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();  // Ignorowanie ewentualnych znaków nowej linii

        if (choice == 1) {
            std::string type, message;
            std::cout << "Podaj typ logu (info, warning, error): ";
            std::getline(std::cin, type);
            std::cout << "Podaj wiadomość: ";
            std::getline(std::cin, message);

            // Dodanie logu
            logger.logMessage(type, message);
        } else if (choice == 2) {
            // Wyświetlenie logów
            logger.displayLogs();
        } else if (choice == 3) {
            // Zakończenie programu
            break;
        } else {
            std::cout << "Nieprawidłowa opcja, spróbuj ponownie.\n";
        }
    }

    return 0;
}
