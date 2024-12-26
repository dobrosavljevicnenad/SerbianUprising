#ifndef CULTURE_H
#define CULTURE_H

#include <QString>
#include <stdexcept>

enum class CultureType {
    SERBIAN,
    TURKISH,
    GREEK,
    ROMANIAN,
    BULGARIAN
};

class Culture {
public:

    friend std::ostream &operator<<(std::ostream &os, const CultureType &culture) {
        os << toString(culture).toStdString();
        return os;
    }

    static QString toString(CultureType culture) {
        switch (culture) {
        case CultureType::SERBIAN: return "SERBIAN";
        case CultureType::TURKISH: return "TURKISH";
        case CultureType::GREEK: return "GREEK";
        case CultureType::ROMANIAN: return "ROMANIAN";
        case CultureType::BULGARIAN: return "BULGARIAN";
        default: throw std::invalid_argument("Unknown CultureType");
        }
    }

    static CultureType fromString(const QString &str) {
        if (str == "SERBIAN") return CultureType::SERBIAN;
        if (str == "TURKISH") return CultureType::TURKISH;
        if (str == "GREEK") return CultureType::GREEK;
        if (str == "ROMANIAN") return CultureType::ROMANIAN;
        if (str == "BULGARIAN") return CultureType::BULGARIAN;
        throw std::invalid_argument("Invalid CultureType string");
    }

};

#endif // CULTURE_H
