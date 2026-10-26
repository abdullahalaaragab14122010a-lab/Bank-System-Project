#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <iomanip>

using namespace std;

// ==================== Validation ====================
class Validation {
public:
    static bool isValidName(const string& name) {
        if (name.length() < 3 || name.length() > 20)
            return false;

        bool hasLetter = false;

        for (unsigned char c : name) {
            if (isalpha(c))
                hasLetter = true;
            else if (c != ' ')
                return false;
        }

        return hasLetter;
    }

    static bool isValidPassword(const string& password) {
        return password.length() >= 8 &&
               password.length() <= 20 &&
               password.find(' ') == string::npos;
    }

    static bool isValidId(int id) {
        return id > 0;
    }

    static bool isValidBalance(double balance) {
        return balance >= 1500.0;
    }

    static bool isValidSalary(double salary) {
        return salary >= 5000.0;
    }
};

// ==================== Person ====================
class Person {
protected:
    int id;
    string name;
    string password;

public:
    Person() : id(0), name(""), password("") {}

    Person(int id, const string& name, const string& password)
        : id(0), name(""), password("") {
        setId(id);
        setName(name);
        setPassword(password);
    }

    virtual ~Person() {}

    void setId(int value) {
        if (Validation::isValidId(value))
            id = value;
    }

    void setName(const string& value) {
        if (Validation::isValidName(value))
            name = value;
    }

    void setPassword(const string& value) {
        if (Validation::isValidPassword(value))
            password = value;
    }

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    string getPassword() const {
        return password;
    }
};

// ==================== Client ====================
class Client : public Person {
private:
    double balance;

public:
    Client() : Person(), balance(1500.0) {}

    Client(int id, const string& name,
           const string& password, double balance)
        : Person(id, name, password), balance(1500.0) {
        setBalance(balance);
    }

    void setBalance(double value) {
        if (Validation::isValidBalance(value))
            balance = value;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid amount!\n";
            return;
        }

        balance += amount;
        cout << "Deposit successful.\n";
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid amount!\n";
        }
        else if (balance - amount < 1500.0) {
            cout << "Minimum balance is 1500.\n";
        }
        else {
            balance -= amount;
            cout << "Withdrawal successful.\n";
        }
    }

    void transferTo(double amount, Client& recipient) {
        if (&recipient == this) {
            cout << "Cannot transfer to yourself!\n";
        }
        else if (amount <= 0) {
            cout << "Invalid amount!\n";
        }
        else if (balance - amount < 1500.0) {
            cout << "Insufficient balance!\n";
        }
        else {
            balance -= amount;
            recipient.balance += amount;
            cout << "Transfer successful.\n";
        }
    }

    void checkBalance() const {
        cout << "Balance: "
             << fixed << setprecision(2)
             << balance << '\n';
    }

    void display() const {
        cout << "\n--- Client Information ---\n";
        cout << "ID: " << id << '\n';
        cout << "Name: " << name << '\n';
        cout << "Balance: "
             << fixed << setprecision(2)
             << balance << '\n';
    }
};

// ==================== Employee ====================
class Employee : public Person {
private:
    double salary;

public:
    Employee() : Person(), salary(5000.0) {}

    Employee(int id, const string& name,
             const string& password, double salary)
        : Person(id, name, password), salary(5000.0) {
        setSalary(salary);
    }

    void setSalary(double value) {
        if (Validation::isValidSalary(value))
            salary = value;
    }

    double getSalary() const {
        return salary;
    }

    void display() const {
        cout << "\n--- Employee Information ---\n";
        cout << "ID: " << id << '\n';
        cout << "Name: " << name << '\n';
        cout << "Salary: "
             << fixed << setprecision(2)
             << salary << '\n';
    }

    void addClient(Client& client);
    Client* searchClient(int id);
    void listClient();

    void editClient(int id, string name,
                    string password, double balance);
};

// ==================== Admin ====================
class Admin : public Employee {
public:
    Admin() : Employee() {}

    Admin(int id, const string& name,
          const string& password, double salary)
        : Employee(id, name, password, salary) {}

    void display() const {
        cout << "\n--- Admin Information ---\n";
        cout << "ID: " << getId() << '\n';
        cout << "Name: " << getName() << '\n';
        cout << "Salary: "
             << fixed << setprecision(2)
             << getSalary() << '\n';
    }

    Client* searchClient(int id);
    void listClient();

    void editClient(int id, string name,
                    string password, double balance);

    void addEmployee(Employee& employee);
    Employee* searchEmployee(int id);

    void editEmployee(int id, string name,
                      string password, double salary);

    void listEmployee();
};
// ==================== Parser ====================
class Parser {
public:
    static vector<string> split(string line) {
        vector<string> result;
        string part;
        stringstream ss(line);

        while (getline(ss, part, '|')) {
            result.push_back(part);
        }

        return result;
    }

    static Client parseToClient(string line) {
        vector<string> data = split(line);

        if (data.size() != 4)
            throw invalid_argument("Invalid client data");

        return Client(stoi(data[0]), data[1],
                      data[2], stod(data[3]));
    }

    static Employee parseToEmployee(string line) {
        vector<string> data = split(line);

        if (data.size() != 4)
            throw invalid_argument("Invalid employee data");

        return Employee(stoi(data[0]), data[1],
                        data[2], stod(data[3]));
    }

    static Admin parseToAdmin(string line) {
        vector<string> data = split(line);

        if (data.size() != 4)
            throw invalid_argument("Invalid admin data");

        return Admin(stoi(data[0]), data[1],
                     data[2], stod(data[3]));
    }
};

// ==================== FilesHelper ====================
class FilesHelper {
public:
    static const string CLIENTS_FILE;
    static const string EMPLOYEES_FILE;
    static const string ADMINS_FILE;

    static const string CLIENT_LAST_ID_FILE;
    static const string EMPLOYEE_LAST_ID_FILE;
    static const string ADMIN_LAST_ID_FILE;

    static void saveLast(string fileName, int id) {
        ofstream file(fileName);

        if (file)
            file << id;
    }

    static int getLast(string fileName) {
        ifstream file(fileName);
        int id = 0;

        if (file >> id)
            return id;

        return 0;
    }

    static void saveClient(Client c) {
        ofstream file(CLIENTS_FILE, ios::app);

        if (!file) {
            cout << "Cannot open Clients.txt\n";
            return;
        }

        file << c.getId() << '|'
             << c.getName() << '|'
             << c.getPassword() << '|'
             << setprecision(15) << c.getBalance()
             << '\n';

        saveLast(CLIENT_LAST_ID_FILE,
                 max(getLast(CLIENT_LAST_ID_FILE),
                     c.getId()));
    }

    static void saveEmployee(string fileName,
                             string lastIdFile,
                             Employee e) {
        ofstream file(fileName, ios::app);

        if (!file) {
            cout << "Cannot open employee file\n";
            return;
        }

        file << e.getId() << '|'
             << e.getName() << '|'
             << e.getPassword() << '|'
             << setprecision(15) << e.getSalary()
             << '\n';

        saveLast(lastIdFile,
                 max(getLast(lastIdFile), e.getId()));
    }

    static void saveAdmin(Admin a) {
        ofstream file(ADMINS_FILE, ios::app);

        if (!file) {
            cout << "Cannot open Admins.txt\n";
            return;
        }

        file << a.getId() << '|'
             << a.getName() << '|'
             << a.getPassword() << '|'
             << setprecision(15) << a.getSalary()
             << '\n';

        saveLast(ADMIN_LAST_ID_FILE,
                 max(getLast(ADMIN_LAST_ID_FILE),
                     a.getId()));
    }

    static vector<Client> fetchClients() {
        vector<Client> clients;
        ifstream file(CLIENTS_FILE);
        string line;

        while (getline(file, line)) {
            if (line.empty())
                continue;

            try {
                clients.push_back(
                    Parser::parseToClient(line));
            }
            catch (...) {
                cout << "Invalid client record skipped.\n";
            }
        }

        return clients;
    }

    static vector<Employee> fetchEmployees() {
        vector<Employee> employees;
        ifstream file(EMPLOYEES_FILE);
        string line;

        while (getline(file, line)) {
            if (line.empty())
                continue;

            try {
                employees.push_back(
                    Parser::parseToEmployee(line));
            }
            catch (...) {
                cout << "Invalid employee record skipped.\n";
            }
        }

        return employees;
    }

    static vector<Admin> fetchAdmins() {
        vector<Admin> admins;
        ifstream file(ADMINS_FILE);
        string line;

        while (getline(file, line)) {
            if (line.empty())
                continue;

            try {
                admins.push_back(
                    Parser::parseToAdmin(line));
            }
            catch (...) {
                cout << "Invalid admin record skipped.\n";
            }
        }

        return admins;
    }

    static void clearFile(string fileName,
                          string lastIdFile) {
        ofstream file(fileName, ios::trunc);

        if (file)
            saveLast(lastIdFile, 0);
    }
};

// File names
const string FilesHelper::CLIENTS_FILE = "Clients.txt";
const string FilesHelper::EMPLOYEES_FILE = "Employees.txt";
const string FilesHelper::ADMINS_FILE = "Admins.txt";

const string FilesHelper::CLIENT_LAST_ID_FILE =
    "LastClientId.txt";

const string FilesHelper::EMPLOYEE_LAST_ID_FILE =
    "LastEmployeeId.txt";

const string FilesHelper::ADMIN_LAST_ID_FILE =
    "LastAdminId.txt";
    // ==================== DataSourceInterface ====================
class DataSourceInterface {
public:
    virtual void addClient(Client obj) = 0;
    virtual void addEmployee(Employee obj) = 0;
    virtual void addAdmin(Admin obj) = 0;

    virtual vector<Client> getAllClients() = 0;
    virtual vector<Employee> getAllEmployees() = 0;
    virtual vector<Admin> getAllAdmins() = 0;

    virtual void removeAllClients() = 0;
    virtual void removeAllEmployees() = 0;
    virtual void removeAllAdmins() = 0;

    virtual ~DataSourceInterface() {}
};

// ==================== FileManager ====================
class FileManager : public DataSourceInterface {
private:
    bool clientIdExists(int id) {
        vector<Client> clients = FilesHelper::fetchClients();

        for (const auto& c : clients) {
            if (c.getId() == id)
                return true;
        }

        return false;
    }

    bool employeeIdExists(int id) {
        vector<Employee> employees = FilesHelper::fetchEmployees();

        for (const auto& e : employees) {
            if (e.getId() == id)
                return true;
        }

        return false;
    }

    bool adminIdExists(int id) {
        vector<Admin> admins = FilesHelper::fetchAdmins();

        for (const auto& a : admins) {
            if (a.getId() == id)
                return true;
        }

        return false;
    }

    void rewriteClients(const vector<Client>& clients) {
        ofstream file(FilesHelper::CLIENTS_FILE, ios::trunc);

        if (!file) {
            cout << "Cannot update Clients.txt\n";
            return;
        }

        int lastId = 0;

        for (const auto& c : clients) {
            file << c.getId() << '|'
                 << c.getName() << '|'
                 << c.getPassword() << '|'
                 << setprecision(15) << c.getBalance()
                 << '\n';

            lastId = max(lastId, c.getId());
        }

        FilesHelper::saveLast(
            FilesHelper::CLIENT_LAST_ID_FILE, lastId);
    }

    void rewriteEmployees(const vector<Employee>& employees) {
        ofstream file(FilesHelper::EMPLOYEES_FILE, ios::trunc);

        if (!file) {
            cout << "Cannot update Employees.txt\n";
            return;
        }

        int lastId = 0;

        for (const auto& e : employees) {
            file << e.getId() << '|'
                 << e.getName() << '|'
                 << e.getPassword() << '|'
                 << setprecision(15) << e.getSalary()
                 << '\n';

            lastId = max(lastId, e.getId());
        }

        FilesHelper::saveLast(
            FilesHelper::EMPLOYEE_LAST_ID_FILE, lastId);
    }

    void rewriteAdmins(const vector<Admin>& admins) {
        ofstream file(FilesHelper::ADMINS_FILE, ios::trunc);

        if (!file) {
            cout << "Cannot update Admins.txt\n";
            return;
        }

        int lastId = 0;

        for (const auto& a : admins) {
            file << a.getId() << '|'
                 << a.getName() << '|'
                 << a.getPassword() << '|'
                 << setprecision(15) << a.getSalary()
                 << '\n';

            lastId = max(lastId, a.getId());
        }

        FilesHelper::saveLast(
            FilesHelper::ADMIN_LAST_ID_FILE, lastId);
    }

public:
    // Add Client
    void addClient(Client obj) override {
        if (!Validation::isValidId(obj.getId()) ||
            !Validation::isValidName(obj.getName()) ||
            !Validation::isValidPassword(obj.getPassword()) ||
            !Validation::isValidBalance(obj.getBalance())) {
            cout << "Invalid client data!\n";
            return;
        }

        if (clientIdExists(obj.getId())) {
            cout << "Client ID already exists!\n";
            return;
        }

        FilesHelper::saveClient(obj);
        cout << "Client added successfully.\n";
    }

    // Add Employee
    void addEmployee(Employee obj) override {
        if (!Validation::isValidId(obj.getId()) ||
            !Validation::isValidName(obj.getName()) ||
            !Validation::isValidPassword(obj.getPassword()) ||
            !Validation::isValidSalary(obj.getSalary())) {
            cout << "Invalid employee data!\n";
            return;
        }

        if (employeeIdExists(obj.getId())) {
            cout << "Employee ID already exists!\n";
            return;
        }

        FilesHelper::saveEmployee(
            FilesHelper::EMPLOYEES_FILE,
            FilesHelper::EMPLOYEE_LAST_ID_FILE,
            obj
        );

        cout << "Employee added successfully.\n";
    }

    // Add Admin
    void addAdmin(Admin obj) override {
        if (!Validation::isValidId(obj.getId()) ||
            !Validation::isValidName(obj.getName()) ||
            !Validation::isValidPassword(obj.getPassword()) ||
            !Validation::isValidSalary(obj.getSalary())) {
            cout << "Invalid admin data!\n";
            return;
        }

        if (adminIdExists(obj.getId())) {
            cout << "Admin ID already exists!\n";
            return;
        }

        FilesHelper::saveAdmin(obj);
        cout << "Admin added successfully.\n";
    }

    // Get all records
    vector<Client> getAllClients() override {
        return FilesHelper::fetchClients();
    }

    vector<Employee> getAllEmployees() override {
        return FilesHelper::fetchEmployees();
    }

    vector<Admin> getAllAdmins() override {
        return FilesHelper::fetchAdmins();
    }

    // Remove all records
    void removeAllClients() override {
        FilesHelper::clearFile(
            FilesHelper::CLIENTS_FILE,
            FilesHelper::CLIENT_LAST_ID_FILE
        );
    }

    void removeAllEmployees() override {
        FilesHelper::clearFile(
            FilesHelper::EMPLOYEES_FILE,
            FilesHelper::EMPLOYEE_LAST_ID_FILE
        );
    }

    void removeAllAdmins() override {
        FilesHelper::clearFile(
            FilesHelper::ADMINS_FILE,
            FilesHelper::ADMIN_LAST_ID_FILE
        );
    }

    // Update Client
    bool updateClient(int id, string name,
                      string password, double balance) {
        if (!Validation::isValidName(name) ||
            !Validation::isValidPassword(password) ||
            !Validation::isValidBalance(balance)) {
            cout << "Invalid client information!\n";
            return false;
        }

        vector<Client> clients = getAllClients();

        for (auto& c : clients) {
            if (c.getId() == id) {
                c.setName(name);
                c.setPassword(password);
                c.setBalance(balance);

                rewriteClients(clients);
                cout << "Client updated successfully.\n";
                return true;
            }
        }

        cout << "Client not found!\n";
        return false;
    }

    // Update Employee
    bool updateEmployee(int id, string name,
                        string password, double salary) {
        if (!Validation::isValidName(name) ||
            !Validation::isValidPassword(password) ||
            !Validation::isValidSalary(salary)) {
            cout << "Invalid employee information!\n";
            return false;
        }

        vector<Employee> employees = getAllEmployees();

        for (auto& e : employees) {
            if (e.getId() == id) {
                e.setName(name);
                e.setPassword(password);
                e.setSalary(salary);

                rewriteEmployees(employees);
                cout << "Employee updated successfully.\n";
                return true;
            }
        }

        cout << "Employee not found!\n";
        return false;
    }

    // Update Admin
    bool updateAdmin(int id, string name,
                     string password, double salary) {
        if (!Validation::isValidName(name) ||
            !Validation::isValidPassword(password) ||
            !Validation::isValidSalary(salary)) {
            cout << "Invalid admin information!\n";
            return false;
        }

        vector<Admin> admins = getAllAdmins();

        for (auto& a : admins) {
            if (a.getId() == id) {
                a.setName(name);
                a.setPassword(password);
                a.setSalary(salary);

                rewriteAdmins(admins);
                cout << "Admin updated successfully.\n";
                return true;
            }
        }

        cout << "Admin not found!\n";
        return false;
    }
};
// ==================== Employee Functions ====================

void Employee::addClient(Client& client) {
    FileManager manager;
    manager.addClient(client);
}

Client* Employee::searchClient(int id) {
    static vector<Client> clients;
    clients = FilesHelper::fetchClients();

    for (auto& c : clients) {
        if (c.getId() == id)
            return &c;
    }

    return nullptr;
}

void Employee::listClient() {
    vector<Client> clients = FilesHelper::fetchClients();

    if (clients.empty()) {
        cout << "No clients found.\n";
        return;
    }

    for (const auto& c : clients)
        c.display();
}

void Employee::editClient(int id, string name,
                          string password, double balance) {
    FileManager manager;
    manager.updateClient(id, name, password, balance);
}


// ==================== Admin Functions ====================

Client* Admin::searchClient(int id) {
    static vector<Client> clients;
    clients = FilesHelper::fetchClients();

    for (auto& c : clients) {
        if (c.getId() == id)
            return &c;
    }

    return nullptr;
}

void Admin::listClient() {
    vector<Client> clients = FilesHelper::fetchClients();

    if (clients.empty()) {
        cout << "No clients found.\n";
        return;
    }

    for (const auto& c : clients)
        c.display();
}

void Admin::editClient(int id, string name,
                       string password, double balance) {
    FileManager manager;
    manager.updateClient(id, name, password, balance);
}

void Admin::addEmployee(Employee& employee) {
    FileManager manager;
    manager.addEmployee(employee);
}

Employee* Admin::searchEmployee(int id) {
    static vector<Employee> employees;
    employees = FilesHelper::fetchEmployees();

    for (auto& e : employees) {
        if (e.getId() == id)
            return &e;
    }

    return nullptr;
}

void Admin::editEmployee(int id, string name,
                         string password, double salary) {
    FileManager manager;
    manager.updateEmployee(id, name, password, salary);
}

void Admin::listEmployee() {
    vector<Employee> employees = FilesHelper::fetchEmployees();

    if (employees.empty()) {
        cout << "No employees found.\n";
        return;
    }

    for (const auto& e : employees)
        e.display();
}


// ==================== ClientManager ====================

class ClientManager {
public:
    static void showClientMenu(Client& client) {
        int choice;

        do {
            cout << "\n===== CLIENT MENU =====\n";
            cout << "1. Display information\n";
            cout << "2. Check balance\n";
            cout << "3. Deposit\n";
            cout << "4. Withdraw\n";
            cout << "5. Transfer\n";
            cout << "0. Logout\n";
            cout << "Choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = -1;
            }

            if (choice == 1) {
                client.display();
            }
            else if (choice == 2) {
                client.checkBalance();
            }
            else if (choice == 3) {
                double amount;
                cout << "Amount: ";
                if (cin >> amount && amount > 0) {
                    client.deposit(amount);
                    FileManager manager;
                    manager.updateClient(
                        client.getId(), client.getName(),
                        client.getPassword(), client.getBalance());
                }
                else {
                    cout << "Invalid amount.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            else if (choice == 4) {
                double amount;
                cout << "Amount: ";
                if (cin >> amount && amount > 0) {
                    client.withdraw(amount);
                    FileManager manager;
                    manager.updateClient(
                        client.getId(), client.getName(),
                        client.getPassword(), client.getBalance());
                }
                else {
                    cout << "Invalid amount.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            else if (choice == 5) {
                int receiverId;
                double amount;

                cout << "Receiver ID: ";
                if (!(cin >> receiverId)) {
                    cout << "Invalid ID.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                cout << "Amount: ";
                if (!(cin >> amount) || amount <= 0) {
                    cout << "Invalid amount.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                vector<Client> clients = FilesHelper::fetchClients();
                bool found = false;

                for (auto& receiver : clients) {
                    if (receiver.getId() == receiverId) {
                        found = true;
client.transferTo(amount, receiver);

FileManager manager;

manager.updateClient(
    client.getId(),
    client.getName(),
    client.getPassword(),
    client.getBalance()
);

manager.updateClient(
    receiver.getId(),
    receiver.getName(),
    receiver.getPassword(),
    receiver.getBalance()
);


                        break;
                    }
                }

                if (!found)
                    cout << "Receiver not found.\n";
            }
            else if (choice != 0) {
                cout << "Invalid choice.\n";
            }

        } while (choice != 0);
    }

    static void login() {
        int id;
        string password;

        cout << "Client ID: ";
        if (!(cin >> id)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid ID.\n";
            return;
        }

        cout << "Password: ";
        cin >> password;

        vector<Client> clients = FilesHelper::fetchClients();

        for (auto& client : clients) {
            if (client.getId() == id &&
                client.getPassword() == password) {
                showClientMenu(client);
                return;
            }
        }

        cout << "Invalid ID or password.\n";
    }
};


// ==================== EmployeeManager ====================

class EmployeeManager {
public:
    static void login() {
        int id;
        string password;

        cout << "Employee ID: ";
        if (!(cin >> id)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid ID.\n";
            return;
        }

        cout << "Password: ";
        cin >> password;

        vector<Employee> employees = FilesHelper::fetchEmployees();

        for (auto& employee : employees) {
            if (employee.getId() == id &&
                employee.getPassword() == password) {
                showMenu(employee);
                return;
            }
        }

        cout << "Invalid ID or password.\n";
    }

    static void showMenu(Employee& employee) {
        int choice;

        do {
            cout << "\n===== EMPLOYEE MENU =====\n";
            cout << "1. Display employee information\n";
            cout << "2. List clients\n";
            cout << "3. Search client\n";
            cout << "4. Add client\n";
            cout << "5. Edit client\n";
            cout << "0. Logout\n";
            cout << "Choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = -1;
            }

            if (choice == 1) {
                employee.display();
            }
            else if (choice == 2) {
                employee.listClient();
            }
            else if (choice == 3) {
                int id;
                cout << "Client ID: ";
                if (cin >> id) {
                    Client* client = employee.searchClient(id);
                    if (client)
                        client->display();
                    else
                        cout << "Client not found.\n";
                }
                else {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID.\n";
                }
            }
            else if (choice == 4) {
                int id;
                string name, password;
                double balance;

                cout << "Client ID: ";
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID.\n";
                    continue;
                }

                cout << "Name (no spaces): ";
                cin >> name;

                cout << "Password: ";
                cin >> password;

                cout << "Balance (minimum 1500): ";
                if (!(cin >> balance)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid balance.\n";
                    continue;
                }

                Client client(id, name, password, balance);
                employee.addClient(client);
            }
            else if (choice == 5) {
                int id;
                string name, password;
                double balance;

                cout << "Client ID to edit: ";
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID.\n";
                    continue;
                }

                cout << "New name (no spaces): ";
                cin >> name;

                cout << "New password: ";
                cin >> password;

                cout << "New balance: ";
                if (!(cin >> balance)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid balance.\n";
                    continue;
                }

                employee.editClient(id, name, password, balance);
            }
            else if (choice != 0) {
                cout << "Invalid choice.\n";
            }

        } while (choice != 0);
    }
};


// ==================== AdminManager ====================

class AdminManager {
public:
    static void login() {
        int id;
        string password;

        cout << "Admin ID: ";
        if (!(cin >> id)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid ID.\n";
            return;
        }

        cout << "Password: ";
        cin >> password;

        vector<Admin> admins = FilesHelper::fetchAdmins();

        for (auto& admin : admins) {
            if (admin.getId() == id &&
                admin.getPassword() == password) {
                showMenu(admin);
                return;
            }
        }

        cout << "Invalid ID or password.\n";
    }

    static void showMenu(Admin& admin) {
        int choice;

        do {
            cout << "\n===== ADMIN MENU =====\n";
            cout << "1. Display admin information\n";
            cout << "2. List clients\n";
            cout << "3. Search client\n";
            cout << "4. Add client\n";
            cout << "5. Edit client\n";
            cout << "6. List employees\n";
            cout << "7. Search employee\n";
            cout << "8. Add employee\n";
            cout << "9. Edit employee\n";
            cout << "0. Logout\n";
            cout << "Choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = -1;
            }

            if (choice == 1) {
                admin.display();
            }
            else if (choice == 2) {
                admin.listClient();
            }
            else if (choice == 3) {
                int id;
                cout << "Client ID: ";
                if (cin >> id) {
                    Client* client = admin.searchClient(id);
                    if (client)
                        client->display();
                    else
                        cout << "Client not found.\n";
                }
                else {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID.\n";
                }
            }
            else if (choice == 4) {
                int id;
                string name, password;
                double balance;

                cout << "Client ID: ";
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID.\n";
                    continue;
                }

                cout << "Name (no spaces): ";
                cin >> name;

                cout << "Password: ";
                cin >> password;

                cout << "Balance (minimum 1500): ";
                if (!(cin >> balance)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid balance.\n";
                    continue;
                }

                Client client(id, name, password, balance);
                admin.editClient(-1, "", "", 0); // No operation; removed below
                FileManager manager;
                manager.addClient(client);
            }
            else if (choice == 5) {
                int id;
                string name, password;
                double balance;

                cout << "Client ID to edit: ";
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID.\n";
                    continue;
                }

                cout << "New name (no spaces): ";
                cin >> name;

                cout << "New password: ";
                cin >> password;

                cout << "New balance: ";
                if (!(cin >> balance)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid balance.\n";
                    continue;
                }

                admin.editClient(id, name, password, balance);
            }
            else if (choice == 6) {
                admin.listEmployee();
            }
            else if (choice == 7) {
                int id;
                cout << "Employee ID: ";
                if (cin >> id) {
                    Employee* employee = admin.searchEmployee(id);
                    if (employee)
                        employee->display();
                    else
                        cout << "Employee not found.\n";
                }
                else {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID.\n";
                }
            }
            else if (choice == 8) {
                int id;
                string name, password;
                double salary;

                cout << "Employee ID: ";
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID.\n";
                    continue;
                }

                cout << "Name (no spaces): ";
                cin >> name;

                cout << "Password: ";
                cin >> password;

                cout << "Salary (minimum 5000): ";
                if (!(cin >> salary)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid salary.\n";
                    continue;
                }

                Employee employee(id, name, password, salary);
                admin.addEmployee(employee);
            }
            else if (choice == 9) {
                int id;
                string name, password;
                double salary;

                cout << "Employee ID to edit: ";
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID.\n";
                    continue;
                }

                cout << "New name (no spaces): ";
                cin >> name;

                cout << "New password: ";
                cin >> password;

                cout << "New salary: ";
                if (!(cin >> salary)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid salary.\n";
                    continue;
                }

                admin.editEmployee(id, name, password, salary);
            }
            else if (choice != 0) {
                cout << "Invalid choice.\n";
            }

        } while (choice != 0);
    }
};


// ==================== Screens ====================

class Screens {
public:
    static void showMainMenu() {
        int choice;

        do {
            cout << "\n========== BANK SYSTEM ==========\n";
            cout << "1. Client Login\n";
            cout << "2. Employee Login\n";
            cout << "3. Admin Login\n";
            cout << "0. Exit\n";
            cout << "Choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = -1;
            }

            switch (choice) {
                case 1:
                    ClientManager::login();
                    break;

                case 2:
                    EmployeeManager::login();
                    break;

                case 3:
                    AdminManager::login();
                    break;

                case 0:
                    cout << "Goodbye!\n";
                    break;

                default:
                    cout << "Invalid choice.\n";
            }

        } while (choice != 0);
    }
};


// ==================== Main ====================

int main() {
    // Create a default admin if no admin exists.
    vector<Admin> admins = FilesHelper::fetchAdmins();

    if (admins.empty()) {
        Admin firstAdmin(1, "Admin", "Admin1234", 5000);
        FileManager manager;
        manager.addAdmin(firstAdmin);

        cout << "\nDefault admin created:\n";
        cout << "ID: 1\n";
        cout << "Password: Admin1234\n";
    }

    Screens::showMainMenu();

    return 0;
}
