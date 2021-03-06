#include <cstdint>
#include <cstddef>
#include <deque>
#include <iostream>
#include <set>
#include <vector>
#include <boost/container/flat_map.hpp>


template <typename Storage>
struct Adaptor
{
    template<typename T>
    struct Allocator
    {
        typedef T * pointer;
        typedef const T * const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T value_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        template<typename U>
        struct rebind
        { typedef Allocator<U> other; };

        Allocator(Storage& inStorage) : mStorage(&inStorage) { }
        Allocator(const Allocator& rhs) : mStorage(rhs.mStorage) { }

        template <typename U>
        Allocator(const Allocator<U>& rhs) : mStorage(rhs.mStorage) { }

        Allocator& operator=(const Allocator& rhs)
        { mStorage = rhs.mStorage; return *this; }

        ~Allocator() { }

        const T * address(const T& s) const { return &s; }
        T * address(T& r) const { return &r; }

        size_t max_size() const { return std::size_t(-1); }

        bool operator==(const Allocator&) const { return true; }
        bool operator!=(const Allocator& other) const { return !(*this == other); }

        void construct(T * const p, const T& t) const { new (static_cast<void *>(p)) T(t); }
        void destroy(T * const p) const { p->~T(); }

        T * allocate(size_t n) const { return n ? static_cast<T*>(mStorage->allocate(n * sizeof(T))) : nullptr; }

        void deallocate(T * const p, const size_t n) const { mStorage->deallocate(p, n * sizeof(T)); }

        template <typename U>
        T * allocate(const size_t n, const U*) const { return this->allocate(n); }

        Storage* mStorage;
    };
};


struct Arena
{
    Arena(std::size_t inSize) : mStorage(align(inSize)), mData(mStorage.data()) {}
    
    Arena(Arena&&) = default;
    Arena& operator=(Arena&&) = default;
    
    ~Arena() = default;

    void* allocate(std::size_t n)
    {
        auto result = mData;
        mData += align(n);
        std::cout << " +" << n;
        return result;
    }

    void deallocate(void* data, std::size_t n)
    {
        std::cout << " -" << n;
        // deallocate is a no-op
        (void)data;
        (void)n;
    }
    
private:
    uint32_t align(uint32_t n)
    {
        enum { alignment = sizeof(void*) };
        auto remainder = n % alignment;
        return n + bool(remainder) * (alignment - remainder);
    }

    std::vector<char> mStorage;
    char* mData;
};


struct Malloc
{
    void* allocate(std::size_t n)
    {
        std::cout << " +" << n;
        return malloc(n);
    }

    void deallocate(void* data, std::size_t n)
    {
        std::cout << " -" << n;
        free(data);
    }
};


struct Pool
{
    void* allocate(std::size_t n)
    {
        std::cout << " +" << n;
        
        auto& storage = mStorage[n];
        if (storage.empty())
        {
            return malloc(n);
        }
        
        void* result = storage.back();
        storage.pop_back();
        return result;
    }

    void deallocate(void* data, std::size_t n)
    {
        std::cout << " -" << n;
        mStorage[n].push_back(data);
    }
    
    using Storage = std::vector<void*>;
    boost::container::flat_map<std::size_t, Storage> mStorage;
};


enum
{
    arena_size = 500000,
    num_iterations = 200
};


void test_vector()
{
    std::cout << __FUNCTION__ << std::endl;

    Pool storage;
    using Alloc = Adaptor<Pool>::Allocator<int>;
    
    std::vector<int, Alloc> container{Alloc{storage}};
    for (int i = 0; i != num_iterations; ++i)
    {
        container.push_back(i);
    }   
    std::cout << "\n";
}


void test_vector_with_reserve()
{
    std::cout << __FUNCTION__ << std::endl;
    
    Arena arena{arena_size};
    
    using Alloc = Adaptor<Arena>::Allocator<int>;
    std::vector<int, Alloc> container(Alloc{arena});
    container.reserve(num_iterations);
    
    for (int i = 0; i != num_iterations; ++i)
    {
        container.push_back(i);
    }   
    std::cout << "\n";
}




void test_set()
{
    std::cout << __FUNCTION__ << std::endl;
    
    Arena arena{arena_size};    
    using Alloc = Adaptor<Arena>::Allocator<int>;
    std::set<int, std::less<int>, Alloc> container(Alloc{arena});
    
    
    for (int i = 0; i != num_iterations; ++i)
    {
        container.insert(i);
    }   
    std::cout << "\n";
}


void test_deque()
{
    std::cout << __FUNCTION__ << std::endl;
    
    Arena arena{arena_size};
    
    using Alloc = Adaptor<Arena>::Allocator<int>;
    std::deque<int, Alloc> container(Alloc{arena});
    
    
    for (int i = 0; i != num_iterations; ++i)
    {
        container.push_back(i);
    }   
    std::cout << "\n";
}


void test_deque_with_reserve()
{
    std::cout << __FUNCTION__ << std::endl;
    
    Arena arena{arena_size};
    
    using Alloc = Adaptor<Arena>::Allocator<int>;
    std::deque<int, Alloc> container(Alloc{arena});
    
    
    for (int i = 0; i != num_iterations; ++i)
    {
        container.push_back(i);
    }   
    std::cout << "\n";
}


int main()
{
    test_vector();
    std::cout << "\n\n";
    
    test_vector_with_reserve();
    std::cout << "\n\n";
    
    test_set();
    std::cout << "\n\n";
    
    test_deque();
    std::cout << "\n\n";
}

