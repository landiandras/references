#include <iostream>
#include "IndexingArray.h"

int main() {
    IndexingArray<int> arr{10, 20, 30, 40, 50};
    arr.add_sorting(std::greater<>());
    arr.set_indexing(0);
    std::cout<<arr << std::endl;
    arr.set_indexing(1);
    arr.push_back(std::move(60));
    std::cout<<arr << std::endl;
    arr.add_sorting(std::less_equal<>());
    arr.set_indexing(2);
    std::cout<<arr << std::endl;
    auto haha = arr.pop_back();
    IndexingArray<int> test{1};
    std::cout << arr[2] << std::endl;
    auto add = arr[1] + arr[2];
    test = std::move(arr);
    std::cout<<test << std::endl;

    return 0;
}
