#include <iostream>
#include <list>
#include <map>
#include <tuple>

template<typename T>
struct logging_allocator {
    using value_type = T;
    T *allocate(std::size_t n) const {
        auto p = std::malloc(2*n * sizeof(T));
        if (!p)
            throw std::bad_alloc();
        return reinterpret_cast<T *>(p);
    }

    void deallocate(T *p, std::size_t n) const {
        std::free(p);
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) const {
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) const {
        p->~T();
    }
};


int main(int, char *[]) {
    auto v = std::list<int, logging_allocator<int>>{};

    for (size_t i = 0; i < 5; ++i) {
        v.emplace_back(i);
    }

    for (auto i = v.begin(); i != v.end(); i++) {
        std::cout << i << std::endl;
    }

    for (auto i: v) {
        std::cout << &i << std::endl;
    }

    return 0;
}

