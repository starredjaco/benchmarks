#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <memory>
#include <typeinfo>

template<typename T>
struct DataTraits {
    static constexpr size_t storage_size = sizeof(T);
    static constexpr bool needs_init = false;
};

class IExtension {
public:
    virtual void init() = 0;
    virtual void process() = 0;
    virtual void* getPayload() = 0;
    virtual size_t getPayloadSize() = 0;
    virtual ~IExtension() {}
};

struct ConfigPayload {
    char module[32];
    char param[32];
    char setting[64];
    int level;
};

template<>
struct DataTraits<ConfigPayload> {
    static constexpr size_t storage_size = sizeof(ConfigPayload);
    static constexpr bool needs_init = false;
};

class ConfigExtension : public IExtension {
private:
    ConfigPayload data;
    
public:
    ConfigExtension() {
        memset(&data, 0, sizeof(data));
        data.level = 1;
    }
    
    void init() override {
        std::cout << "Config extension ready" << std::endl;
    }
    
    void setConfig(const char* mod, const char* par, const char* val) {
        strncpy(data.module, mod, sizeof(data.module) - 1);
        strncpy(data.param, par, sizeof(data.param) - 1);
        strncpy(data.setting, val, sizeof(data.setting) - 1);
    }
    
    void process() override {
        std::cout << "[" << data.module << "] "
                  << data.param << " = " << data.setting
                  << " (level: " << data.level << ")" << std::endl;
    }
    
    void* getPayload() override {
        return &data;
    }
    
    size_t getPayloadSize() override {
        return sizeof(data);
    }
};

struct ActionPayload {
    char identifier[64];
    char metadata[64];
    void (*execute)(void* arg);
    void* argument;
};

template<>
struct DataTraits<ActionPayload> {
    static constexpr size_t storage_size = sizeof(ActionPayload);
    static constexpr bool needs_init = true;
};

class ActionExtension : public IExtension {
private:
    ActionPayload data;
    
    static void defaultAction(void* arg) {
        std::cout << "Default action executed" << std::endl;
    }
    
public:
    ActionExtension() {
        memset(&data, 0, sizeof(data));
        data.execute = defaultAction;
        data.argument = nullptr;
    }
    
    void init() override {
        std::cout << "Action extension ready" << std::endl;
    }
    
    void setAction(const char* id, const char* meta) {
        strncpy(data.identifier, id, sizeof(data.identifier) - 1);
        strncpy(data.metadata, meta, sizeof(data.metadata) - 1);
    }
    
    void setExecutor(void (*exec)(void*), void* arg) {
        data.execute = exec;
        data.argument = arg;
    }
    
    void process() override {
        std::cout << "Running action: " << data.identifier << std::endl;
        if (data.execute) {
            data.execute(data.argument);
        }
    }
    
    void* getPayload() override {
        return &data;
    }
    
    size_t getPayloadSize() override {
        return sizeof(data);
    }
};

template<typename T>
class ExtensionWrapper {
private:
    T* extension;
    
public:
    ExtensionWrapper(T* ext) : extension(ext) {}
    
    ~ExtensionWrapper() {
        delete extension;
    }
    
    IExtension* get() {
        return dynamic_cast<IExtension*>(extension);
    }
    
    void* extractPayload() {
        return extension->getPayload();
    }
    
    size_t payloadSize() {
        return DataTraits<typename std::remove_pointer<decltype(extension->getPayload())>::type>::storage_size;
    }
};

class ExtensionManager {
private:
    std::map<int, IExtension*> active;
    std::map<int, void*> archived;
    std::map<int, size_t> sizes;
    int sequence;
    
public:
    ExtensionManager() : sequence(0) {}
    
    ~ExtensionManager() {
        for (auto& entry : active) {
            delete entry.second;
        }
        for (auto& entry : archived) {
            free(entry.second);
        }
    }
    
    int add(IExtension* ext) {
        int id = sequence++;
        
        active[id] = ext;
        
        size_t sz = ext->getPayloadSize();
        void* storage = malloc(sz);
        memcpy(storage, ext->getPayload(), sz);
        
        archived[id] = storage;
        sizes[id] = sz;
        
        return id;
    }
    
    template<typename ExtType>
    ExtType* restore(int id) {
        auto it = archived.find(id);
        if (it == archived.end()) {
            return nullptr;
        }
        
        ExtType* ext = new ExtType();
        
        size_t transfer_sz = std::min(sizes[id], ext->getPayloadSize());
        memcpy(ext->getPayload(), it->second, transfer_sz);
        
        return ext;
    }
    
    IExtension* fetch(int id) {
        auto it = active.find(id);
        return (it != active.end()) ? it->second : nullptr;
    }
    
    void enumerate() {
        std::cout << "\n=== Active Extensions ===" << std::endl;
        for (const auto& entry : active) {
            std::cout << "ID " << entry.first << ": "
                      << typeid(*entry.second).name() << std::endl;
        }
    }
};

void displayOptions() {
    std::cout << "\n=== Extension Manager v3.0 ===" << std::endl;
    std::cout << "1. Add Config Extension" << std::endl;
    std::cout << "2. Add Action Extension" << std::endl;
    std::cout << "3. Process Extension" << std::endl;
    std::cout << "4. Restore as Config" << std::endl;
    std::cout << "5. Restore as Action" << std::endl;
    std::cout << "6. List Extensions" << std::endl;
    std::cout << "7. Exit" << std::endl;
    std::cout << ">> ";
}

int main() {
    ExtensionManager manager;
    int selection;
    char input[128];
    
    std::cout << "Extension Management Framework" << std::endl;
    
    while (true) {
        displayOptions();
        std::cin >> selection;
        std::cin.ignore();
        
        switch (selection) {
            case 1: {
                ConfigExtension* ext = new ConfigExtension();
                ext->init();
                
                std::cout << "Module: ";
                std::cin.getline(input, sizeof(input));
                char mod[32];
                strncpy(mod, input, sizeof(mod) - 1);
                
                std::cout << "Parameter: ";
                std::cin.getline(input, sizeof(input));
                char par[32];
                strncpy(par, input, sizeof(par) - 1);
                
                std::cout << "Setting: ";
                std::cin.getline(input, sizeof(input));
                
                ext->setConfig(mod, par, input);
                
                int id = manager.add(ext);
                std::cout << "Added extension ID: " << id << std::endl;
                break;
            }
            
            case 2: {
                ActionExtension* ext = new ActionExtension();
                ext->init();
                
                std::cout << "Action ID: ";
                std::cin.getline(input, sizeof(input));
                char id_str[64];
                strncpy(id_str, input, sizeof(id_str) - 1);
                
                std::cout << "Metadata: ";
                std::cin.getline(input, sizeof(input));
                
                ext->setAction(id_str, input);
                
                int id = manager.add(ext);
                std::cout << "Added extension ID: " << id << std::endl;
                break;
            }
            
            case 3: {
                int id;
                std::cout << "Extension ID: ";
                std::cin >> id;
                std::cin.ignore();
                
                IExtension* ext = manager.fetch(id);
                if (ext) {
                    ext->process();
                } else {
                    std::cout << "Extension not found!" << std::endl;
                }
                break;
            }
            
            case 4: {
                int id;
                std::cout << "Extension ID: ";
                std::cin >> id;
                std::cin.ignore();
                
                ConfigExtension* ext = manager.restore<ConfigExtension>(id);
                if (ext) {
                    std::cout << "\n=== Restored as Config ===" << std::endl;
                    ext->process();
                    delete ext;
                } else {
                    std::cout << "Restore failed!" << std::endl;
                }
                break;
            }
            
            case 5: {
                int id;
                std::cout << "Extension ID: ";
                std::cin >> id;
                std::cin.ignore();
                
                ActionExtension* ext = manager.restore<ActionExtension>(id);
                if (ext) {
                    std::cout << "\n=== Restored as Action ===" << std::endl;
                    ext->process();
                    delete ext;
                } else {
                    std::cout << "Restore failed!" << std::endl;
                }
                break;
            }
            
            case 6:
                manager.enumerate();
                break;
                
            case 7:
                return 0;
                
            default:
                std::cout << "Invalid selection!" << std::endl;
        }
    }
    
    return 0;
}
