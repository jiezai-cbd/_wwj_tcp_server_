#ifndef _TIMESTAMP_H_
#define _TIMESTAMP_H_
#include <stdint.h>
#include <string>

namespace wwj
{
class Timestamp
{
public:
    Timestamp();
    Timestamp(int64_t microSecondsSinceEpoch);
    static Timestamp now();
    static const int kMicroSecondsPerSecond = 1000*1000;
    std::string toString() const;
    int64_t microSecondsSinceEpoch() const {return microSecondsSinceEpoch_;}
    bool valid() const { return microSecondsSinceEpoch_ > 0; }
    static Timestamp invalid();
    /*
    bool operator<(Timestamp rhs)
    {
        return this->microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
    }

    bool operator==(Timestamp rhs)
    {
        return this->microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
    }
    */
private:
    int64_t microSecondsSinceEpoch_;
};

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
  int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}
}

#endif