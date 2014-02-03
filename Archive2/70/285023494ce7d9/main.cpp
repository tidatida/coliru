#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <memory>

typedef std::size_t ulword;

template <ulword... I> 
struct indices { typedef indices type; };

template <ulword N, ulword... I>
struct make_indices : public make_indices<N - 1, N - 1, I...> { };

template <ulword... I>
struct make_indices<0, I...> : public indices<I...>{ };

namespace Furrovine {

    namespace detail {
        template <typename T, ulword skew>
		struct forward_stack {
            operator T& () {
                return *( static_cast<T*>( static_cast<void*>( this ) ) + skew );
            }
            
            operator const T& () const {
                return *( static_cast<const T*>( static_cast<const void*>( this ) ) + skew );
            }
            
            T& operator= ( const T& value ) {
                T& item = *this;
                item = value;
                return *this;
            }
        };
        
        template <typename T, ulword i>
		struct vector_member;

		template <typename T>
		struct vector_member<T, 0> : public forward_stack<T, 0> {
		
        };

		template <typename T>
		struct vector_member<T, 1> : public forward_stack<T, 1> {
		
        };

		template <typename T>
		struct vector_member<T, 2> : public forward_stack<T, 2> {
		
        };

		template <typename T>
		struct vector_member<T, 3> : public forward_stack<T, 3> {
		
        };

		template <typename T, typename TEat>
		struct vector_members;

		template <typename T, ulword... I>
		struct vector_members<T, indices<I...>> : public vector_member<T, I>... {};
	}

	template <typename T, ulword n>
	struct RVector : detail::vector_members<T, typename make_indices<n>::type> {
		typedef typename make_indices<n>::type indices_t;
        
    };
    
    template <typename T>
    struct RVector2 : RVector<T, 2> {
        union {
            std::array<T, 2> arf;
            struct {
                float x;
                float y;
            };
        };
    };
    
    struct test {
        union {
            RVector2<float> aggregate;
        };
    };
    
}

int main() {
    using namespace Furrovine;
    static_assert( std::is_pod<RVector<float, 3>>::value, "Fuck this shit." );
    static_assert( std::is_standard_layout<RVector<float, 3>>::value, "Fuck this shit." );
    static_assert( std::is_pod<detail::vector_member<float, 0>>::value, "Well, fuck!" );
    static_assert( std::is_pod<detail::vector_member<float, 1>>::value, "Well, fuck!" );
    static_assert( std::is_pod<detail::vector_member<float, 2>>::value, "Well, fuck!" );
    static_assert( std::is_pod<detail::vector_members<float, make_indices<3>::type>>::value, "Well, fuck!" );

    static_assert( std::is_pod<RVector2<float>>::value, "Damnit ._." );
    static_assert( std::is_standard_layout<RVector2<float>>::value, "Damnit ._." );
    static_assert( sizeof( RVector2<float> ) == 8, "Even more damnit!" );
    RVector2<float> meow;
    std::cout << meow.x << " " << meow.y << std::endl;
    meow.x = 2.56f;
    meow.y = 3.78f;
    std::cout << meow.x << " " << meow.y << std::endl;
}
