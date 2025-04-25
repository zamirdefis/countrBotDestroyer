//this code has not been refactored since the repository was created :/
#include "dependencies.hpp"
#include "defines.hpp"
#include "utils.hpp"

namespace nl = nlohmann;

int main(int argc, char* argv[]) {

    std::string myUserID_ss = "";
    std::cout << "Enter user ID : ";
    std::cin >> myUserID_ss;
    std::string channelID_ss = "";
    std::cout << "Enter channel ID : ";
    std::cin >> channelID_ss;
    std::string authorizationKey_ss = "";
    std::cout << "Enter authorization key : ";
    std::cin >> authorizationKey_ss;

    std::string strLastNum = "0";

    CURL* curl = curl_easy_init();

    if (!curl) {
        printf_s("[!] Curl has not initialized"); endline;
    }

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, std::string("Authorization: " + authorizationKey_ss).c_str());

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);

    std::string strRes = "";
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strRes);

    while (true) {
        while (true) {
            nl::json jGET;
            curl_easy_setopt(curl, CURLOPT_HTTPGET, true);
            curl_easy_setopt(curl, CURLOPT_URL, std::string("https://discord.com/api/v9/channels/" + channelID_ss + "/messages?limit=1").c_str());
            CURLcode resCode = curl_easy_perform(curl);

            if (resCode != CURLE_OK) { printf_s("[!] (GET) Request returned an error"); endline; }
            jGET = nl::json::parse(strRes);
            if (!jGET.is_array()) {
                if (!jGET["message"].is_null()) {
                    int errorCode = jGET["code"].get<int>();
                    if (errorCode == 20028 || errorCode == 20016) {
                        double waitTime = jGET["retry_after"].get<double>();
                        std::this_thread::sleep_for(std::chrono::seconds(uint32_t(waitTime) + 1));
#ifdef DBG
                        std::cout << "[...] [[" + std::to_string(errorCode) + "]] (GET) Cooldown [" << std::to_string(waitTime) << "s :(" + std::to_string(uint32_t(waitTime) + 1) + "s): ]" << std::endl;
#endif
                        jGET.clear();
                        strRes.clear();
                        continue;
                    }
                    else {
                        std::cout << "[!] [[" + std::to_string(errorCode) + "]] (GET) Req error code : " << jGET["code"].get<uint32_t>() << std::endl;
                        jGET.clear();
                        strRes.clear();
                        continue;
                    }
                }
            }
            if (jGET[NULL]["author"]["id"].get<std::string>() == myUserID_ss) {
                jGET.clear();
                strRes.clear();
                continue;
            }
            strLastNum = jGET[NULL]["content"].get<std::string>();
            if (!isNum(strLastNum)) {
#ifdef DBG
                std::cout << "[...] \"" + strLastNum + "\" - Is not a number >:[" << std::endl;
#endif
                jGET.clear();
                strRes.clear();
                continue;
            }
            if (strLastNum.length() > 9) {
#ifdef DBG
                std::cout << "[...] \"" + strLastNum + "\" - Number is too big >:[" << std::endl;
#endif
                jGET.clear();
                strRes.clear();
                continue;
            }
            jGET.clear();
            strRes.clear();
            break;
        }
        nl::json jPOST;
        std::string body = "{\"mobile_network_type\":\"unknown\",\"content\":\"" + std::to_string(std::stoi(strLastNum) + 1) + std::string("\",\"tts\":false,\"flags\":0}");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, std::string("https://discord.com/api/v9/channels/" + channelID_ss + "/messages").c_str());
        CURLcode resCodePOST = curl_easy_perform(curl);
        if (resCodePOST != CURLE_OK) { printf_s("[!] (POST) Request returned an error"); endline; }
        jPOST = nl::json::parse(strRes);
        if (!jPOST["message"].is_null()) {
            int errorCode = jPOST["code"].get<int>();
            if (errorCode == 20028 || errorCode == 20016) {
                double waitTime = jPOST["retry_after"].get<double>();
                std::this_thread::sleep_for(std::chrono::seconds(uint32_t(waitTime) + 1));
#ifdef DBG
                std::cout << "[...] [[" + std::to_string(errorCode) + "]] (POST) Cooldown [" << std::to_string(waitTime) << "s :(" + std::to_string(uint32_t(waitTime) + 1) + "s): ]" << std::endl;
#endif
            }
            else {
                std::cout << strRes << std::endl;
                std::cout << "[...] [[" + std::to_string(errorCode) + "]] (POST) Req error code : " << jPOST["code"].get<uint32_t>() << std::endl;
            }
        }
        strRes.clear();
        jPOST.clear();
    }
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
	return EXIT_SUCCESS;
}
