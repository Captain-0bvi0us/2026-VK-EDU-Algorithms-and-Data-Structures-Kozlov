/*
 * Задача № 1
 *
 * 1_1. Множество строк на динамической хеш-таблице с открытой адресацией.
 * Хранимые строки непустые, только строчные латинские буквы.
 * Функция хеширования строки — полином по методу Горнера.
 * Начальный размер таблицы 8; размер m всегда степень двойки.
 * При добавлении выполнять рехеш, если коэффициент заполнения достигает 3/4.
 * Для разрешения коллизий — квадратичное пробирование: g(k,i) = (g(k,i-1) + i) mod m.
 * В таблице запрещено хранить указатели на дескрипторы элементов; ключи — в ячейках.
 * Операции: добавление (+), удаление (-), проверка принадлежности (?); выход OK/FAIL на запрос.
 *
 * Скорость работы - О(1) в среднем на операцию (амортизированно).
 * Потребляемая память - О(n), где n — число занятых ячеек множества.
 */

#include <iostream>
#include <string>
#include <utility>
#include <vector>

class StringHashTable {
public:
    StringHashTable() : capacity_(8), occupied_(0), cells_(capacity_) {}

    bool Contains(const std::string& s) const {
        return FindIndex(s).first;
    }

    bool Add(const std::string& s) {
        if (Contains(s)) {
            return false;
        }
        if (NeedRehashBeforeInsert()) {
            Rehash(capacity_ * 2);
        }
        return InsertIntoTable(s);
    }

    bool Remove(const std::string& s) {
        const auto result = FindIndex(s);
        if (!result.first) {
            return false;
        }
        cells_[result.second].state = State::Deleted;
        cells_[result.second].key.clear();
        --occupied_;
        return true;
    }

private:
    enum class State { Empty, Deleted, Occupied };

    struct Cell {
        State state = State::Empty;
        std::string key;
    };

    static size_t HornerHash(const std::string& s) {
        const unsigned long long p = 31ULL;
        unsigned long long h = 0;
        for (unsigned char ch : s) {
            h = h * p + static_cast<unsigned long long>(ch - 'a' + 1);
        }
        return static_cast<size_t>(h);
    }

    bool NeedRehashBeforeInsert() const {
        return (occupied_ + 1) * 4ULL > capacity_ * 3ULL;
    }

    std::pair<bool, size_t> FindIndex(const std::string& s) const {
        const size_t h0 = HornerHash(s) % capacity_;
        size_t idx = h0;
        for (int step = 1; step <= static_cast<int>(capacity_); ++step) {
            const Cell& c = cells_[idx];
            if (c.state == State::Empty) {
                return {false, idx};
            }
            if (c.state == State::Occupied && c.key == s) {
                return {true, idx};
            }
            idx = (idx + static_cast<size_t>(step)) % capacity_;
        }
        return {false, 0};
    }

    bool InsertIntoTable(const std::string& s) {
        const size_t h0 = HornerHash(s) % capacity_;
        size_t idx = h0;
        for (int step = 1; step <= static_cast<int>(capacity_); ++step) {
            Cell& c = cells_[idx];
            if (c.state != State::Occupied) {
                c.state = State::Occupied;
                c.key = s;
                ++occupied_;
                return true;
            }
            idx = (idx + static_cast<size_t>(step)) % capacity_;
        }
        return false;
    }

    void Rehash(size_t new_cap) {
        std::vector<std::string> keys;
        keys.reserve(occupied_);
        for (const Cell& c : cells_) {
            if (c.state == State::Occupied) {
                keys.push_back(c.key);
            }
        }
        capacity_ = new_cap;
        occupied_ = 0;
        cells_.assign(capacity_, Cell{});
        for (const std::string& k : keys) {
            InsertIntoTable(k);
        }
    }

    size_t capacity_;
    size_t occupied_;
    std::vector<Cell> cells_;
};

int main() {
    StringHashTable set;
    char op = 0;
    std::string s;

    while (std::cin >> op >> s) {
        if (op == '+') {
            if (set.Add(s)) {
                std::cout << "OK\n";
            } else {
                std::cout << "FAIL\n";
            }
        } else if (op == '-') {
            if (set.Remove(s)) {
                std::cout << "OK\n";
            } else {
                std::cout << "FAIL\n";
            }
        } else if (op == '?') {
            if (set.Contains(s)) {
                std::cout << "OK\n";
            } else {
                std::cout << "FAIL\n";
            }
        }
    }
    return 0;
}
