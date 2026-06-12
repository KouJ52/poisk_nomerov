#include <iostream>
#include <string>
#include <regex>
#include <set>
#include <cctype>

std::string extractDigits(const std::string& s) {
    std::string digits;
    for (char c : s) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            digits.push_back(c);
        }
    }
    return digits;
}

std::string normalizePhoneNumber(const std::string& raw) {
    static const std::regex shortPattern(R"(^\d{3}-\d{3}$)");
    if (std::regex_match(raw, shortPattern)) {
        return raw;
    }
    std::string digits = extractDigits(raw);
    if (digits.length() == 11 && (digits[0] == '7' || digits[0] == '8')) {
        return "+7" + digits.substr(1);
    }
    if (raw.length() >= 2 && raw[0] == '+' && raw[1] == '7' && digits.length() == 11 && digits[0] == '7') {
        return "+7" + digits.substr(1);
    }
    return "";
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "Введите текст";
    std::cin >> std::noskipws;
    std::string text((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());

    std::regex emailadres(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    std::regex korotkiynomer(R"(\b\d{3}-\d{3}\b)");
    std::regex longnomer(R"((?:\+7|8)?[\s\-]?\(?\d{3}\)?[\s\-]?\d{3}[\s\-]?\d{2}[\s\-]?\d{2})");

    std::set<std::string> emails;
    std::set<std::string> phones;

    auto emailnachalo = std::sregex_iterator(text.begin(), text.end(), emailadres);
    auto emailkonec = std::sregex_iterator();
    for (auto it = emailnachalo; it != emailkonec; ++it) {
        emails.insert(it->str());
    }

    auto korotkiynacahalo = std::sregex_iterator(text.begin(), text.end(), korotkiynomer);
    auto korotkiykonec = std::sregex_iterator();
    for (auto it = korotkiynacahalo; it != korotkiykonec; ++it) {
        std::string norm = normalizePhoneNumber(it->str());
        if (!norm.empty()) phones.insert(norm);
    }

    auto longnachalo = std::sregex_iterator(text.begin(), text.end(), longnomer);
    auto longkonec = std::sregex_iterator();
    for (auto it = longnachalo; it != longkonec; ++it) {
        std::string norm = normalizePhoneNumber(it->str());
        if (!norm.empty()) phones.insert(norm);
    }

    std::cout << "Найденные email-адреса:" << std::endl;
    if (emails.empty()) std::cout << "нет" << std::endl;
    else for (const auto& email : emails) std::cout << "  " << email << std::endl;

    std::cout << "\nНайденные номера телефонов:" << std::endl;
    if (phones.empty()) std::cout << "нет" << std::endl;
    else for (const auto& phone : phones) std::cout << "  " << phone << std::endl;

    return 0;
}
