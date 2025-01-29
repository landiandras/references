#ifndef INDEXINGARRAY_H
#define INDEXINGARRAY_H
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

//Nem mindenhol a legegyszerűbb megoldást válaszottam, mert igyekeztem valamennyire effektívre csinálni, hogy ne szortírozgassa jobbra balra folyamatosan, csak ha szükséges.
//
//
template<typename T>
class IndexingArray {
private:
    //A proxy class oldja meg a problémát, hogy az operator[] által kiadott referencia módosítását érzékelhessük, és ez alapján frissülhessen az indexing.
    template <typename TP>
    class IndexingArrayProxy {
    private:
        TP& element;
        IndexingArray& array;
    public:
        IndexingArrayProxy(TP &element, IndexingArray<TP> &array)
            : element(element),
              array(array) {
        }
        IndexingArrayProxy& operator=(const TP&);
        IndexingArrayProxy& operator=(TP&&);
        IndexingArrayProxy& operator=(const IndexingArrayProxy<TP> &);
        IndexingArrayProxy& operator=(IndexingArrayProxy&&) noexcept;
        operator TP() const {return element;}
    };
    std::vector<T> Data;
    std::vector<std::pair<std::vector<size_t>, std::function<bool(T,T)>>> Indexes;
    size_t CurrentIndex = 0;
public:
    IndexingArray(const IndexingArray&);
    IndexingArray(IndexingArray&&) noexcept;
    template<typename... Args>
    explicit IndexingArray(Args...);
    void update();
    void push_back(T&&);
    T pop(const size_t&);
    T pop_back();
    void add_sorting(std::function<bool(T,T)>);
    void set_indexing(const size_t&);
    size_t size() const;
    IndexingArrayProxy<T> operator[](const size_t&);
    IndexingArray<T>& operator=(const IndexingArray&) = default;
    IndexingArray<T>& operator=(IndexingArray&&) noexcept;
    template<typename F>
    friend std::ostream& operator<<(std::ostream&, IndexingArray<F>&);
    template<typename F>
    friend std::istream& operator>>(std::istream&, IndexingArray<F>&);
};


template<typename T>
template<typename TP>
typename IndexingArray<T>::template IndexingArrayProxy<TP> & IndexingArray<T>::IndexingArrayProxy<TP>::operator=(const TP & other) {
    element = other.element;
    array = other.array;
    return *this;
}

template<typename T>
template<typename TP>
typename IndexingArray<T>::template IndexingArrayProxy<TP> & IndexingArray<T>::IndexingArrayProxy<TP>::operator=(TP && other) {
    element = std::move(other.element);
    array = std::move(other.array);
    return *this;
}

template<typename T>
template<typename TP>
typename IndexingArray<T>::template IndexingArrayProxy<TP> & IndexingArray<T>::IndexingArrayProxy<TP>::operator=(
    const IndexingArrayProxy<TP> & value) {
    element = value;
    array.update();
    return *this;
}

template<typename T>
template<typename TP>
typename IndexingArray<T>::template IndexingArrayProxy<TP> & IndexingArray<T>::IndexingArrayProxy<TP>::operator=(
    IndexingArrayProxy && value) noexcept {
    element = std::move(value);
    array.update();
    return *this;
}

template<typename T>
IndexingArray<T>::IndexingArray(const IndexingArray &other) : Data(other.Data), Indexes(other.Indexes), CurrentIndex(other.CurrentIndex) {
}

template<typename T>
IndexingArray<T>::IndexingArray(IndexingArray &&other) noexcept : Data(std::move(other.Data)), Indexes(std::move(other.Indexes)), CurrentIndex(other.CurrentIndex) {
}

template<typename T>
template<typename... Args>
IndexingArray<T>::IndexingArray(Args... args) : Data{args...} {
    std::pair<std::vector<size_t>, std::function<bool(T,T)>> temppair;
    temppair.second = nullptr;                                              //should never try sorting Indexes[0], because it stores the physical order of the elements.
    for(size_t i = 0; i < Data.size(); ++i) temppair.first.push_back(i);
    Indexes.push_back(temppair);
}

//call when changing CurrentIndex or if data has changed.
//only updates Indexes[CurrentIndex].first, other indexing becomes invalid.
template<typename T>
void IndexingArray<T>::update() {
    if(CurrentIndex == 0) return;
        auto s = [this, func = Indexes[CurrentIndex].second](size_t lhs, size_t rhs) {
            return func(Data[lhs], Data[rhs]);
        };
        std::sort(Indexes[CurrentIndex].first.begin(), Indexes[CurrentIndex].first.end(), s);

}

//adds a Data.size()-1 to the end of each indexing, but only CurrentIndex is sorted again!
template<typename T>
void IndexingArray<T>::push_back(T && value) {
    Data.push_back(std::forward<T>(value));
    for(auto& i : Indexes) {
        i.first.push_back(Data.size()-1);
    }
    update();
}


template<typename T>
T IndexingArray<T>::pop(const size_t & index) {
    if(index >= Data.size()) throw std::out_of_range("Index out of range in pop()!");
    T temp = Data[Indexes[CurrentIndex].first[index]];
    Data.erase(Data.begin() + Indexes[CurrentIndex].first[index]);
    for(auto i : Indexes) {
        if(auto f = std::find(i.first.begin(), i.first.end(), Indexes[CurrentIndex].first[index] )) i.first.erase(f);
        else throw std::invalid_argument("Could not find that element in data!");
    }
    update();
    return temp;
}

template<typename T>
T IndexingArray<T>::pop_back() {
    if(Data.empty()) throw std::underflow_error("Tried popping an empty array!");
    T temp{std::move(Data[Indexes[0].first. back()])};
    Data.pop_back();
    for(size_t i = Indexes.size()-1; i > 0; --i) {
        auto f = std::find(Indexes[i].first.begin(), Indexes[i].first.end(), Indexes[0].first.back() );
        Indexes[i].first.erase(f);
    }
    update();
    return temp;
}

//after adding the new sorting, it switches CurrentIndexing to the new one!
template<typename T>
void IndexingArray<T>::add_sorting(std::function<bool(T, T)> func) {
    std::vector<size_t> temp;
    for(size_t i = 0; i < Data.size(); ++i) temp.push_back(i);
    std::pair<std::vector<size_t>, std::function<bool(T,T)>> temppair{temp, func};
    Indexes.push_back(temppair);
    CurrentIndex = Indexes.size()-1;
    update();
}

template<typename T>
void IndexingArray<T>::set_indexing(const size_t &index) {
    if(index > Indexes.size()) throw std::out_of_range("Index out of range");
    CurrentIndex = index;
    update();
}

template<typename T>
size_t IndexingArray<T>::size() const {
    return Data.size();
}

template<typename T>
typename IndexingArray<T>::template IndexingArrayProxy<T>  IndexingArray<T>::operator[](const size_t & index) {
    return IndexingArrayProxy<T>{Data[Indexes[CurrentIndex].first[index]],*this};
}


template<typename T>
IndexingArray<T> & IndexingArray<T>::operator=(IndexingArray &&other) noexcept {
    Data = std::move(other.Data);
    Indexes = std::move(other.Indexes);
    CurrentIndex = other.CurrentIndex;
    return *this;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, IndexingArray<T>& array) {
    array.update();
    for(size_t i = 0; i < array.Data.size(); ++i) {
        os<< array.Data[array.Indexes[array.CurrentIndex].first[i]] << " ";
    }
    return os;
}

template<typename T>
std::istream& operator>>(std::istream& is, IndexingArray<T>& array) {
    T temp;
    is >> temp;
    array.push_back(std::move(temp));
    return is;
}



#endif //INDEXINGARRAY_H
