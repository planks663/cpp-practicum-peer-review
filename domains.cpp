#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
    // разработайте класс домена
public:
    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
    Domain(string&& name)
        : name_(move(name)) {
        reverse(name_.begin(), name_.end());
        name_ += '.';
    }

    string_view GetDomainName() const {
        return name_;
    }

    // разработайте operator==
    bool operator==(const Domain& rhs) const {
        return name_ == rhs.name_;
    }

    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& domain) const {
        return domain.name_.substr(0, name_.size()) == name_;
    }

private:
    string name_;
};

class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template <typename Iterator>
    DomainChecker(Iterator first, Iterator last)
        : forbidden_domains_(first, last) {
        sort(forbidden_domains_.begin(), forbidden_domains_.end(), [] (const Domain& lhs, const Domain& rhs) {
            return lhs.GetDomainName() < rhs.GetDomainName();
        });
        auto it = unique(forbidden_domains_.begin(), forbidden_domains_.end(), [] (const Domain& lhs, const Domain& rhs) {
            return (rhs.IsSubdomain(lhs) || lhs.IsSubdomain(rhs));
        });
        forbidden_domains_.erase(it, forbidden_domains_.end());
    }

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& domain) const {
        auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain,
                              [] (const Domain& lhs, const Domain& rhs) {
            return (lhs.GetDomainName() < rhs.GetDomainName());
        });

        if (it == forbidden_domains_.begin()) {
            return false;
        }

        return (it - 1)->IsSubdomain(domain);
    }

private:
    vector<Domain> forbidden_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
std::vector<Domain> ReadDomains(istream& input, size_t num) {
    std::vector<Domain> res;
    string line;
    for (size_t i = 0; i < num; ++i) {
        getline(input, line);
        res.push_back({move(line)});
    }
    return res;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
