#include <iostream>
#include <cstring>
#include <vector>
#include <map>

class DataObject {
public:
    virtual void export_data(void* buffer) = 0;
    virtual void import_data(void* buffer) = 0;
    virtual size_t data_size() const = 0;
    virtual int type_code() const = 0;
    virtual ~DataObject() {}
};

class UserAccount : public DataObject {
private:
    char username[32];
    char email[32];
    bool premium;
    
public:
    UserAccount() : premium(false) {
        memset(username, 0, sizeof(username));
        memset(email, 0, sizeof(email));
    }
    
    void setAccount(const char* user, const char* mail) {
        strncpy(username, user, sizeof(username) - 1);
        strncpy(email, mail, sizeof(email) - 1);
    }
    
    void setPremium(bool p) {
        premium = p;
    }
    
    void export_data(void* buffer) override {
        memcpy(buffer, this, sizeof(UserAccount));
    }
    
    void import_data(void* buffer) override {
        memcpy(this, buffer, sizeof(UserAccount));
    }
    
    size_t data_size() const override {
        return sizeof(UserAccount);
    }
    
    int type_code() const override {
        return 1;
    }
    
    void show() {
        std::cout << "User: " << username << std::endl;
        std::cout << "Email: " << email << std::endl;
        std::cout << "Premium: " << (premium ? "Yes" : "No") << std::endl;
    }
};

class TaskItem : public DataObject {
private:
    char description[48];
    void (*callback)(const char*);
    
public:
    TaskItem() {
        memset(description, 0, sizeof(description));
        callback = execute_task;
    }
    
    static void execute_task(const char* desc) {
        std::cout << "Executing: " << desc << std::endl;
    }
    
    void setDescription(const char* desc) {
        strncpy(description, desc, sizeof(description) - 1);
    }
    
    void setCallback(void (*cb)(const char*)) {
        callback = cb;
    }
    
    void export_data(void* buffer) override {
        memcpy(buffer, this, sizeof(TaskItem));
    }
    
    void import_data(void* buffer) override {
        memcpy(this, buffer, sizeof(TaskItem));
    }
    
    size_t data_size() const override {
        return sizeof(TaskItem);
    }
    
    int type_code() const override {
        return 2;
    }
    
    void run() {
        if (callback) {
            callback(description);
        }
    }
    
    void show() {
        std::cout << "Task: " << description << std::endl;
    }
};

class DataStore {
private:
    std::vector<void*> records;
    std::vector<int> record_types;
    
public:
    ~DataStore() {
        for (void* rec : records) {
            free(rec);
        }
    }
    
    int save(DataObject* obj) {
        size_t sz = obj->data_size();
        void* buf = malloc(sz);
        obj->export_data(buf);
        
        records.push_back(buf);
        record_types.push_back(obj->type_code());
        
        return records.size() - 1;
    }
    
    void* retrieve(int idx, int type_hint = -1) {
        if (idx < 0 || idx >= records.size()) {
            return nullptr;
        }
        
        int use_type = (type_hint != -1) ? type_hint : record_types[idx];
        
        void* obj = nullptr;
        
        if (use_type == 1) {
            obj = new UserAccount();
            ((UserAccount*)obj)->import_data(records[idx]);
        } else if (use_type == 2) {
            obj = new TaskItem();
            ((TaskItem*)obj)->import_data(records[idx]);
        }
        
        return obj;
    }
    
    int get_type(int idx) {
        if (idx < 0 || idx >= record_types.size()) {
            return -1;
        }
        return record_types[idx];
    }
};

void show_menu() {
    std::cout << "\n=== Data Management System ===" << std::endl;
    std::cout << "1. Save User Account" << std::endl;
    std::cout << "2. Save Task" << std::endl;
    std::cout << "3. Load Record" << std::endl;
    std::cout << "4. Load As Specific Type" << std::endl;
    std::cout << "5. Exit" << std::endl;
    std::cout << ">> ";
}

int main() {
    DataStore store;
    int option;
    char input[128];
    
    std::cout << "Data Management System v2.1" << std::endl;
    
    while (true) {
        show_menu();
        std::cin >> option;
        std::cin.ignore();
        
        switch (option) {
            case 1: {
                UserAccount acc;
                std::cout << "Username: ";
                std::cin.getline(input, sizeof(input));
                char user[32];
                strncpy(user, input, sizeof(user) - 1);
                
                std::cout << "Email: ";
                std::cin.getline(input, sizeof(input));
                acc.setAccount(user, input);
                
                std::cout << "Premium (y/n): ";
                std::cin.getline(input, sizeof(input));
                acc.setPremium(input[0] == 'y');
                
                int id = store.save(&acc);
                std::cout << "Saved as ID: " << id << std::endl;
                break;
            }
            
            case 2: {
                TaskItem task;
                std::cout << "Description: ";
                std::cin.getline(input, sizeof(input));
                task.setDescription(input);
                
                int id = store.save(&task);
                std::cout << "Saved as ID: " << id << std::endl;
                break;
            }
            
            case 3: {
                int id;
                std::cout << "Record ID: ";
                std::cin >> id;
                std::cin.ignore();
                
                int type = store.get_type(id);
                void* obj = store.retrieve(id);
                
                if (obj) {
                    if (type == 1) {
                        ((UserAccount*)obj)->show();
                        delete (UserAccount*)obj;
                    } else if (type == 2) {
                        ((TaskItem*)obj)->show();
                        ((TaskItem*)obj)->run();
                        delete (TaskItem*)obj;
                    }
                } else {
                    std::cout << "Record not found!" << std::endl;
                }
                break;
            }
            
            case 4: {
                int id, type;
                std::cout << "Record ID: ";
                std::cin >> id;
                std::cout << "Type (1=Account, 2=Task): ";
                std::cin >> type;
                std::cin.ignore();
                
                void* obj = store.retrieve(id, type);
                
                if (obj) {
                    if (type == 1) {
                        ((UserAccount*)obj)->show();
                        delete (UserAccount*)obj;
                    } else if (type == 2) {
                        ((TaskItem*)obj)->show();
                        ((TaskItem*)obj)->run();
                        delete (TaskItem*)obj;
                    }
                } else {
                    std::cout << "Failed to load!" << std::endl;
                }
                break;
            }
            
            case 5:
                return 0;
                
            default:
                std::cout << "Invalid option!" << std::endl;
        }
    }
    
    return 0;
}
