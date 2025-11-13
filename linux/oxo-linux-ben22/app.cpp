#include <iostream>
#include <cstring>
#include <cstdlib>

class Record {
public:
    virtual void display() {
        std::cout << "Generic record" << std::endl;
    }
    
    virtual ~Record() {}
};

class CustomerRecord : public Record {
private:
    char name[32];
    void (*notifyCallback)();
    
public:
    CustomerRecord(const char* customerName) {
        strncpy(name, customerName, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
        notifyCallback = sendNotification;
    }
    
    static void sendNotification() {
        std::cout << "Customer notification sent" << std::endl;
    }
    
    void display() override {
        std::cout << "Customer: " << name << std::endl;
    }
    
    void notify() {
        std::cout << "Sending notification to " << name << std::endl;
        if (notifyCallback) {
            notifyCallback();
        }
    }
};

class TransactionRecord : public Record {
private:
    char reference[32];
    int amount;
    
public:
    TransactionRecord(const char* ref) : amount(0) {
        strncpy(reference, ref, sizeof(reference) - 1);
        reference[sizeof(reference) - 1] = '\0';
    }
    
    void display() override {
        std::cout << "Transaction: " << reference << " - Amount: $" << amount << std::endl;
    }
    
    void setAmount(int amt) {
        amount = amt;
    }
    
    int getAmount() {
        return amount;
    }
};

void handleRecord(Record* rec, int recordType) {
    if (recordType == 1) {
        CustomerRecord* cust = (CustomerRecord*)rec;
        cust->display();
        cust->notify();
    } else if (recordType == 2) {
        TransactionRecord* trans = (TransactionRecord*)rec;
        trans->display();
    } else {
        rec->display();
    }
}

void showMenu() {
    std::cout << "\n=== Record Management System ===" << std::endl;
    std::cout << "1. Create Customer Record" << std::endl;
    std::cout << "2. Create Transaction Record" << std::endl;
    std::cout << "3. Handle as Customer" << std::endl;
    std::cout << "4. Handle as Transaction" << std::endl;
    std::cout << "5. Handle Default" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << ">> ";
}

int main() {
    Record* currentRecord = nullptr;
    int choice;
    char input[64];
    
    std::cout << "Record Management System v1.8" << std::endl;
    
    while (true) {
        showMenu();
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1:
                std::cout << "Customer name: ";
                std::cin.getline(input, sizeof(input));
                delete currentRecord;
                currentRecord = new CustomerRecord(input);
                std::cout << "Customer record created." << std::endl;
                break;
                
            case 2:
                std::cout << "Transaction reference: ";
                std::cin.getline(input, sizeof(input));
                delete currentRecord;
                currentRecord = new TransactionRecord(input);
                std::cout << "Transaction record created." << std::endl;
                break;
                
            case 3:
                if (currentRecord) {
                    handleRecord(currentRecord, 1);
                } else {
                    std::cout << "No record available!" << std::endl;
                }
                break;
                
            case 4:
                if (currentRecord) {
                    handleRecord(currentRecord, 2);
                } else {
                    std::cout << "No record available!" << std::endl;
                }
                break;
                
            case 5:
                if (currentRecord) {
                    currentRecord->display();
                } else {
                    std::cout << "No record available!" << std::endl;
                }
                break;
                
            case 6:
                delete currentRecord;
                return 0;
                
            default:
                std::cout << "Invalid option!" << std::endl;
        }
    }
    
    return 0;
}
