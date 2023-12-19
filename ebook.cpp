#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

using namespace std;

class EbookStats
{
public:
    const uint32_t MAX_ID = 100000;
    const uint16_t MAX_PAGE = 1000;

    EbookStats() {
        id_to_page_.resize(MAX_ID + 1, 0);
        page_to_readers_.resize(MAX_PAGE + 1, 0);
    }

    void Read(const uint32_t id, const uint16_t page) {
        max_page_ = max(max_page_, page);
        if (page == 0) {
            return;
        }
        uint16_t last_page = id_to_page_[id];
        id_to_page_[id] = page;
        if (last_page) {
            --page_to_readers_[last_page];
        }
        ++page_to_readers_[page];
    }

    double Cheer(const uint32_t id) {
        uint16_t current_page = id_to_page_[id];
        if (current_page == 0)  {
            return 0.0;
        }
        uint32_t readers_less = accumulate(page_to_readers_.begin(), page_to_readers_.begin() + current_page, 0);
        uint32_t readers = accumulate(page_to_readers_.begin(), page_to_readers_.end(), 0);
        if (readers == 1) {
            return 1.0;
        }
        return static_cast<double>(readers_less) / (readers - 1);
    }

private:
    uint16_t max_page_{0};
    vector<uint16_t> id_to_page_;
    vector<uint32_t> page_to_readers_;
};

int main()
{
    EbookStats ebook_stats;
    uint32_t Q;
    cin >> Q;
    for (uint32_t i = 0; i < Q; ++i) {
        string query;
        cin >> query;
        uint32_t id;
        cin >> id;
        if ("READ"sv == query) {
           uint16_t page;
           cin >> page;
            ebook_stats.Read(id, page);
        } else if ("CHEER"sv == query) {
            cout << setprecision(6) << ebook_stats.Cheer(id) << endl;
        }
    }
}
