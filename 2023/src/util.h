#ifndef _UTIL_H
#define _UTIL_H
#include <bits/stdc++.h>

class Stringlist {
  public:
    typedef std::vector<std::string>::const_iterator const_iterator;
    bool fromFile(std::string filename);
    Stringlist &fromString(const std::string &input);
    const std::vector<std::string> &get() const;
    const std::string &get(size_t index) const;

    const_iterator begin() const;
    const_iterator end() const;
    size_t size(void) const;

  private:
    std::vector<std::string> m_vect;
};

template <typename TResult1, typename TResult2>
bool runTest(TResult1 result, TResult2 expected, std::string testname = "") {
    std::cout << testname;
    if (result == expected) {
        std::cout << "\tOK\n";
    } else {
        std::cout << "\tFAIL expected:" << expected << " was:" << result << "\n";
    }

    return (result == expected);
}

class Position2D {
  public:
    int x;
    int y;
    Position2D(int x, int y) : x{x}, y{y} {};
    Position2D(void) : x{0}, y{0} {};

    bool operator==(const Position2D &rhs) const {
        return (x == rhs.x) && (y == rhs.y);
    }
    friend std::ostream &operator<<(std::ostream &output, const Position2D &p) {
        output << "(" << p.x << ", " << p.y << ")";
        return output;
    }

    bool operator<(const Position2D &rhs) const {
        return (y < rhs.y) || ((y == rhs.y) && (x < rhs.x));
    }

    Position2D operator+(const Position2D &rhs) const {
        return Position2D(x + rhs.x, y + rhs.y);
    }

    Position2D operator-(const Position2D &rhs) const {
        return Position2D(x - rhs.x, y - rhs.y);
    }
};

namespace std {

template <>
struct hash<Position2D> {
    std::size_t operator()(const Position2D &p) const {
        using std::hash;

        return (hash<int>()(p.x) ^ (hash<int>()(p.y) << 1));
    }
};
} // namespace std

#endif