#include <iostream>
//#include <array>
#include <vector>
#include <map>

using namespace std;
//
//void note_array() {
//    std::array<int, 5> mat = {};
//    mat.fill(5);
//
//    array<int, 5> mat0 = {};
//
//    // like plain 2D array
//    array<array<int, 5>, 5> mat1 = {
//            1,2,3,4,5,
//            1,2,3,4,5,
//            1,2,3,4,5,
//            1,2,3,4,5,
//            1,2,3,4,5,
//    };
//
//    // construct with 1D arys.
//    array<int, 5> ary = {1, 2, 3, 4, 5};
//    array<array<int, 5>, 5> mat2 = { ary, ary, ary, ary, ary};
//
//    // just like plain 2D array, but can ommit some value some each div.
//    array<array<int, 5>, 5> mat3 = {
//            array<int, 5>{ 1, 2, 3, 4, 5},
//            array<int, 5>{ 1, 2, 3, 4},
//            array<int, 5>{ 1, 2, 3},
//            array<int, 5>{ 1, 2,},
//            array<int, 5>{ 1, }
//    };
//
//    // util function to print matrix.
//    auto printMatrix = [] (const array<array<int, 5>, 5>& mat) {
//        for (const auto& ary : mat) {
//            for (const auto& item : ary) {
//                cout << item << " ";
//            }
//            cout << endl;
//        }
//    };
//
//    auto print = [](const array<array<int, 5>, 5> mat){
//        for ( const auto &ary : mat){
//            for (const auto &item : ary) {
//                cout << item << " ";
//            }
//            cout << endl;
//        }
//    };
//
//    print(mat1);
//}
//
//void note_for() {
//    vector<int> vec = {1,2,3,4,5};
//    for(auto &i : vec){
//        cout<< i<< " ";
//    }
//    cout<<endl;
//
//    auto print = [](const int n){cout<< n << " ";};
//    for_each(vec.begin(),vec.end(),print);
//    cout<<endl;
//
//    // add item
//    for_each(vec.begin(),vec.end(),[](int &n){n++;});
//
//    for_each(vec.begin(),vec.end(),print);
//    cout<<endl;
//}
//
//void note_map() {
//    map<string,string> addr;
//    addr["a"] = 'a';
//    addr["b"] = 'b';
//        for (auto &&item : addr) {
//            cout<< "first:" << item.first << " second:" << item.second << endl;
//        }
//}

int main() {
    std::cout << "Hello, World!" << std::endl;

//    // array
//    cout<< "array note\n";
//    note_array();
//
//    // for for_each
//    cout<< "for for_each note\n";
//    note_for();
//
//    // map for earch
//    cout<< "note map\n";
//    note_map();

    // vector::at
    std::vector<int> myvector (10);   // 10 zero-initialized ints

    // assign some values:
    for (unsigned i=0; i<myvector.size(); i++)
        myvector.at(i)=i;

    std::cout << "myvector contains:";
    for (unsigned i=0; i<myvector.size(); i++)
        std::cout << ' ' << myvector.at(i);
    std::cout << '\n';
}
