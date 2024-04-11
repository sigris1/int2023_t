#include "int2023_t.h"
#include "cstring"
#include "cmath"

const uint32_t kPositiveRate = 64;
const uint32_t kMaxIndex = 252;
const uint32_t kMaxDigitNumber = 256;

int2023_t SwapMinusToPlus(int2023_t number){
    if (number == from_int(0)){
        return from_int(0);
    }
    number.number_saving[0] += 64;
    return number + from_int(1);
}

int2023_t SwapPlusToMinus(int2023_t number){
    if (number == from_int(0)){
        return from_int(0);
    }
    number = number - from_int(1);
    number.number_saving[0] %= 64;
    return number;
}

bool IsPositive(int2023_t number){
    return (number.number_saving[0] >= 64);
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
    int point = 0;
    while (lhs.number_saving[point] == rhs.number_saving[point]){
        ++point;
    }
    if (lhs.number_saving[point] < rhs.number_saving[point]){
        return true;
    } else {
        return false;
    }
}

bool IsMoreThanZero(std::string buffer){
    for (uint64_t j = 0; j < buffer.length(); ++j){
        if (buffer[j] != '0'){
            return true;
        }
    }
    return false;
}

std::string StringDivision(std::string& buffer){
    int32_t digit_overflow = 0;
    for (uint32_t i = 0; i < buffer.length(); ++i){
        uint64_t checked = (buffer[i] - '0' + digit_overflow) % 2;
        buffer[i] = (buffer[i] - '0' + digit_overflow) / 2 + '0';
        if (checked == 1){
            digit_overflow = 10;
        } else {
            digit_overflow = 0;
        }
    }
    return buffer;
}

int2023_t from_int(int32_t i){
    int2023_t number{};
    if (i >= 0){
        number.number_saving[0] = kPositiveRate;
    } else {
        i *= -1;
        --i;
    }
    uint32_t digit = 0;
    uint32_t value_at_position = 0;
    uint32_t position = kMaxIndex;
    while (i > 0){
        if (i % 2 == 1) {
            value_at_position += pow(2, digit);
        }
        ++digit;
        if (digit == 8){
            digit = 0;
            number.number_saving[position] = value_at_position;
            value_at_position = 0;
            --position;
        }
        i /= 2;
    }
    number.number_saving[position] = value_at_position;
    return number;
}

int2023_t from_string(const char* buff) {
    int2023_t number{};
    std::string buffer = buff;
    if (buff[0] != '-'){
        number.number_saving[0] = kPositiveRate;
    } else {
        buffer[0] = '0';
        uint32_t pos = strlen(buff) - 1;
        while (buffer[pos] == '0'){
            buffer[pos] = '9';
            --pos;
        }
        buffer[pos] = char(buffer[pos] - 1);
    }
    uint32_t digit = 0;
    uint32_t value_at_position = 0;
    uint32_t position = kMaxIndex;
    while (IsMoreThanZero(buffer)){
        if ((buffer[buffer.length() - 1] - '0') % 2 == 1) {
            value_at_position += pow(2, digit);
        }
        ++digit;
        if (digit == 8){
            digit = 0;
            number.number_saving[position] = value_at_position;
            value_at_position = 0;
            --position;
        }
        buffer = StringDivision(buffer);
    }
    number.number_saving[position] = value_at_position;
    return number;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t numbers_sum{};
    uint32_t digit_overflow = 0;
    if (IsPositive(lhs) && IsPositive(rhs)){
        for (uint32_t i = kMaxIndex; i > 0; --i){
            numbers_sum.number_saving[i] = (lhs.number_saving[i] + rhs.number_saving[i] + digit_overflow) % kMaxDigitNumber;
            digit_overflow = (lhs.number_saving[i] + rhs.number_saving[i] + digit_overflow) / kMaxDigitNumber;
        }
        numbers_sum.number_saving[0] = rhs.number_saving[0] + lhs.number_saving[0] + digit_overflow - kPositiveRate;
    } else if (!IsPositive(lhs) && !IsPositive(rhs)){
        int2023_t rhs_current = SwapMinusToPlus(rhs);
        int2023_t lhs_current = SwapMinusToPlus(lhs);
        return SwapPlusToMinus(rhs_current + lhs_current);
    } else if (!IsPositive(lhs) && IsPositive(rhs)){
        int2023_t current = SwapMinusToPlus(lhs);
        return rhs - current;
    } else if (IsPositive(lhs) && !IsPositive(rhs)){
        int2023_t current = SwapMinusToPlus(rhs);
        return lhs - current;
    }
    return numbers_sum;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t numbers_dif{};
    if (lhs == rhs){
        return from_int(0);
    }
    if (IsPositive(lhs) && IsPositive(rhs)){
        if (lhs < rhs){
            return SwapPlusToMinus(rhs - lhs);
        } else {
            bool digit_overflow = false;
            for (uint32_t i = kMaxIndex; i > 0; --i){
                numbers_dif.number_saving[i] = lhs.number_saving[i] - rhs.number_saving[i] - digit_overflow + kMaxDigitNumber * digit_overflow;
                digit_overflow = lhs.number_saving[i] - rhs.number_saving[i] - digit_overflow < 0;
            }
            numbers_dif.number_saving[0] = lhs.number_saving[0] - rhs.number_saving[0] - digit_overflow + kPositiveRate;
        }
    } else if (!IsPositive(lhs) && !IsPositive(rhs)){
        int2023_t rhs_current = SwapMinusToPlus(rhs);
        int2023_t lhs_current = SwapMinusToPlus(lhs);
        return rhs_current - lhs_current;
    } else if (!IsPositive(lhs) && IsPositive(rhs)){
        int2023_t current = SwapPlusToMinus(rhs);
        return lhs + current;
    } else if (IsPositive(lhs) && !IsPositive(rhs)){
        int2023_t current = SwapMinusToPlus(rhs);
        return lhs + current;
    }
    return numbers_dif;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t numbers_multipl{};
    uint64_t temporary_multiplication[kInt2023MassiveLength];
    for (uint64_t & i : temporary_multiplication){
        i = 0;
    }
    if (IsPositive(lhs) && IsPositive(rhs)){
        for (uint64_t i = kMaxIndex; i > 0; --i){
            for (uint64_t j = kMaxIndex; j > 0; --j){
                temporary_multiplication[(i + j + 1) % kInt2023MassiveLength] += rhs.number_saving[j] * lhs.number_saving[i];
            }
        }
        for (uint64_t i = kMaxIndex; i > 0; --i){
            temporary_multiplication[i - 1] += temporary_multiplication[i] / kMaxDigitNumber;
            temporary_multiplication[i] = temporary_multiplication[i] % kMaxDigitNumber;
        }
        for (uint64_t i = kMaxIndex; i > 0; --i){
            numbers_multipl.number_saving[i] = temporary_multiplication[i];
        }
        numbers_multipl.number_saving[0] += kPositiveRate;
    } else if (!IsPositive(lhs) && !IsPositive(rhs)) {
        int2023_t rhs_current = SwapMinusToPlus(rhs);
        int2023_t lhs_current = SwapMinusToPlus(lhs);
        return rhs_current * lhs_current;
    } else if (!IsPositive(lhs) && IsPositive(rhs)){
        int2023_t current = SwapMinusToPlus(lhs);
        return SwapPlusToMinus(current * rhs);
    } else if (IsPositive(lhs) && !IsPositive(rhs)){
        int2023_t current = SwapMinusToPlus(rhs);
        return SwapPlusToMinus(current * lhs);
    }
    return numbers_multipl;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    if (rhs == from_int(1)){
        return lhs;
    }
    int2023_t numbers_division{};
    if (IsPositive(lhs) && IsPositive(rhs)){
        if (lhs < rhs){
            return from_int(0);
        }
        int2023_t left = from_int(0);
        int2023_t right = lhs;
        while (left < (right - from_int(1))){
            int2023_t middle = (right + left);
            bool digit_overflow = false;
            for (uint64_t i = 1; i < kInt2023MassiveLength; ++i){
                bool mew = middle.number_saving[i] % 2;
                middle.number_saving[i] /= 2;
                middle.number_saving[i] += digit_overflow * 128;
                digit_overflow = mew;
            }
            if (!(lhs < middle * rhs) && ((lhs - middle * rhs) < rhs)) {
                return middle;
            } else if (lhs < (middle * rhs)){
                right = middle;
            } else {
                left = middle;
            }
        }
    } else if (!IsPositive(lhs) && !IsPositive(rhs)) {
        int2023_t rhs_current = SwapMinusToPlus(rhs);
        int2023_t lhs_current = SwapMinusToPlus(lhs);
        return lhs_current / rhs_current;
    } else if (!IsPositive(lhs) && IsPositive(rhs)){
        int2023_t current = SwapMinusToPlus(lhs);
        return SwapPlusToMinus(current / rhs);
    } else if (IsPositive(lhs) && !IsPositive(rhs)){
        int2023_t current = SwapMinusToPlus(rhs);
        return SwapPlusToMinus(lhs / current);
    }
    return numbers_division;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    for (int i = 0; i < kInt2023MassiveLength; ++i){
        if (lhs.number_saving[i] == rhs.number_saving[i]){
            continue;
        } else {
            return false;
        }
    }
    return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    int i = 0;
    while (lhs.number_saving[i] != rhs.number_saving[i]){
        i++;
    }
    if (i == kInt2023MassiveLength){
        return false;
    } else {
        return true;
    }
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    for (uint8_t i : value.number_saving) {
        stream << int(i) << ' ';
    }
    return stream;
}
