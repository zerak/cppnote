#include <assert.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <string.h>

using namespace std;

int chooseFrom(int choose, int from) {
    int values[from];
    for (int i = 0; i < from; ++i) {
        values[i] = i+1;
    }

    int elements[sizeof(values)/ sizeof(values[0])];
    memset(elements, 0, sizeof(elements));
    for (int i = 0; i < choose; ++i) {
        elements[i] = 1;
    }
    const size_t N = sizeof(elements)/sizeof(elements[0]);
    std::vector<int> selectors(elements, elements + N);

    int count = 0;
    do {
        ++count;
        for (size_t i = 0; i < selectors.size(); ++i) {
//            std::cout << selectors[i] << ", ";
            if (selectors[i]) {
                std::cout << values[i] << ", ";
            }
        }
        std::cout << std::endl;
//        if (count==35){
//            break;
//        }
    } while (prev_permutation(selectors.begin(), selectors.end()));

    std::cout << "total count:" << count << std::endl;
    return count;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        cout << "usage:\n"
                "arg1,from count.\n"
                "arg2,choose count." << endl;
        return 0;
    }
    int from = atoi(argv[1]);
    int choose = atoi(argv[2]);
    cout << "from:" << from << " choose:" << choose << endl;
    chooseFrom(choose, from);
    return 0;
}