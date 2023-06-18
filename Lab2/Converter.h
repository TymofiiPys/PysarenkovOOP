#ifndef CONVERTER_H
#define CONVERTER_H
#include <string>

template <typename T>
class Converter {
private:
    T key_;
public:
    Converter() {};
    Converter(T key) : key_(key) {};
    operator int() {
        return int(key_);
    }
    operator std::string() {
        return std::to_string(key_);
    }
    operator double() {
        return double(key_);
    }
};

template<>
Converter<std::string>::operator int() {
    return std::stoi(key_);
}

template<>
Converter<std::string>::operator double() {
    return std::stod(key_);
}

template<>
Converter<std::string>::operator std::string() {
    return key_;
}

#endif // CONVERTER_H
