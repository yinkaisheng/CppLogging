/*!
    \file format.cpp
    \brief Format logger example
    \author Ivan Shynkarenka
    \date 18.09.2016
    \copyright MIT License
*/

#include "logging/logger.h"

class Date
{
public:
    Date(int year, int month, int day) : _year(year), _month(month), _day(day) {}

    int year() const { return _year; }
    int month() const { return _month; }
    int day() const { return _day; }

    friend CppLogging::Record& operator<<(CppLogging::Record& record, const Date& date)
    { return record.StoreCustomFormat("{}-{}-{}", date._year, date._month, date._day); }

private:
    int _year, _month, _day;
};

template <>
struct fmt::formatter<Date>
{
     constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Date& date, FormatContext& ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "{}-{}-{}", date.year(), date.month(), date.day());
    }
};

class DateTime
{
public:
    DateTime(Date date, int hours, int minutes, int seconds) : _date(date), _hours(hours), _minutes(minutes), _seconds(seconds) {}

    Date date() const { return _date; }
    int hours() const { return _hours; }
    int minutes() const { return _minutes; }
    int seconds() const { return _seconds; }

    friend CppLogging::Record& operator<<(CppLogging::Record& record, const DateTime& datetime)
    {
        const size_t begin = record.StoreListBegin();
        record.StoreList(datetime._date);
        record.StoreList(' ');
        record.StoreList(datetime._hours);
        record.StoreList(':');
        record.StoreList(datetime._minutes);
        record.StoreList(':');
        record.StoreList(datetime._seconds);
        return record.StoreListEnd(begin);
    }

private:
    Date _date;
    int _hours, _minutes, _seconds;
};

template <>
struct fmt::formatter<DateTime>
{
     constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const DateTime& datetime, FormatContext& ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "{} {}:{}:{}", datetime.date(), datetime.hours(), datetime.minutes(), datetime.seconds());
    }
};

int main(int argc, char** argv)
{
    // Create default logger
    CppLogging::Logger logger;

    // Log some messages with format
    logger.Info("argc: {}, argv: {}", argc, (void*)argv);
    logger.Info("no arguments");
    logger.Info("{0}, {1}, {2}", -1, 0, 1);
    logger.Info("{0}, {1}, {2}", 'a', 'b', 'c');
    logger.Info("{}, {}, {}", 'a', 'b', 'c');
    logger.Info("{2}, {1}, {0}", 'a', 'b', 'c');
    logger.Info("{0}{1}{0}", "abra", "cad");
    logger.Info("{:<30}", "left aligned");
    logger.Info("{:>30}", "right aligned");
    logger.Info("{:^30}", "centered");
    logger.Info("{:*^30}", "centered");
    logger.Info("{:+f}; {:+f}", 3.14, -3.14);
    logger.Info("{: f}; {: f}", 3.14, -3.14);
    logger.Info("{:-f}; {:-f}", 3.14, -3.14);
    logger.Info("int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    logger.Info("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", 42);
    logger.Info("The date is {}", Date(2012, 12, 9));
    logger.Info("The datetime is {}", DateTime(Date(2012, 12, 9), 13, 15, 57));
    logger.Info("Elapsed time: {s:.2f} seconds", "s"_a = 1.23);

    return 0;
}
