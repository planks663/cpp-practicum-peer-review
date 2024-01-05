#define CHECK_MIN_MAX(param_name, val, min, max) {              \
        if ((val) < (min)) {                                    \
            throw domain_error(#param_name + " is too small"s); \
        }                                                       \
        if ((val) > (max)) {                                    \
            throw domain_error(#param_name + " is too big"s);   \
        }                                                       \
    }

void CheckDateTimeValidity(const DateTime& dt) {
    CHECK_MIN_MAX(year,   dt.year,   1, 9999);
    CHECK_MIN_MAX(month,  dt.month,  1, 12);

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    CHECK_MIN_MAX(day,    dt.day,    1, month_lengths[dt.month - 1]);
    CHECK_MIN_MAX(hour,   dt.hour,   0, 23);
    CHECK_MIN_MAX(minute, dt.minute, 0, 59);
    CHECK_MIN_MAX(second, dt.second, 0, 59);
}