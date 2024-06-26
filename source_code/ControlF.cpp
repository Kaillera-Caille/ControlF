#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <string>
#include <fstream>

#pragma comment(lib, "Ws2_32.lib")

void initializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        exit(1);
    }
}

void cleanupWinsock() {
    WSACleanup();
}

bool queryDNS(const std::string& hostname) {
    addrinfo hints = {}, * info = nullptr;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int result = getaddrinfo(hostname.c_str(), "http", &hints, &info);
    if (result == 0 && info != nullptr) {
        freeaddrinfo(info);
        return true;
    }
    else {
        return false;
    }
}

std::vector<std::string> readWordlist(const std::string& filePath) {
    std::vector<std::string> subdomains;
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Failed to open wordlist file: " << filePath << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            subdomains.push_back(line);
        }
    }
    file.close();
    return subdomains;
}

int main() {
    initializeWinsock();

    std::string title = R"(     
            
                   ::::::::   ::::::::  ::::    ::: ::::::::::: :::::::::   ::::::::  :::        :::::::::: 
                 :+:    :+: :+:    :+: :+:+:   :+:     :+:     :+:    :+: :+:    :+: :+:        :+:         
                +:+        +:+    +:+ :+:+:+  +:+     +:+     +:+    +:+ +:+    +:+ +:+        +:+          
               +#+        +#+    +:+ +#+ +:+ +#+     +#+     +#++:++#:  +#+    +:+ +#+        :#::+::#      
              +#+        +#+    +#+ +#+  +#+#+#     +#+     +#+    +#+ +#+    +#+ +#+        +#+            
             #+#    #+# #+#    #+# #+#   #+#+#     #+#     #+#    #+# #+#    #+# #+#        #+#             
             ########   ########  ###    ####     ###     ###    ###  ########  ########## ###                  
                       
    )";
std::cout << title << std::endl;

std::string usage = R"(
Usage:

1 - Insert target domain: example.com

2 - Insert wordlist path: c:/wordlist/wordlist.txt

)";

    std::cout << usage << std::endl;

    std::string domain;
    std::cout << "Target: ";
    std::getline(std::cin, domain);

    std::string wordlistPath;
    std::cout << "Wordlist path: ";
    std::getline(std::cin, wordlistPath);

    std::vector<std::string> subdomains = readWordlist(wordlistPath);

    for (const std::string& subdomain : subdomains) {
        std::string fullDomain = subdomain + "." + domain;
        if (queryDNS(fullDomain)) {
            std::cout << "Subdomain found: " << fullDomain << std::endl;
        }
    }

    cleanupWinsock();
    std::cout << "\n";
    std::cout << "Scanning complete! \n";
    std::cout << "\n";
    std::cin.get(); 

    return 0;
}

