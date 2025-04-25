#include "dependencies.hpp"
#include "utils.hpp"

bool isNum(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end()) {
        bool isn = false;
        for (char c : nums) {
            if (c == *it) {
                isn = true;
            }
        }
        if (isn == false) {
            return false;
        }
        ++it;
    }
    return true;
}

size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)buffer, size * nmemb);
    return size * nmemb;
}