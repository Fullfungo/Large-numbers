#include "num.hpp"
//#include "overlow_detect.hpp"
//#include <climits>
#include <cassert>
#include <string>

#define int_type_casts(start, end) \
start bool end \
start char end \
start signed char end \
start signed int end \
start signed short int end \
start signed long int end \
start signed long long int end \
start unsigned char end \
start unsigned int end \
start unsigned short int end \
start unsigned long int end \
start unsigned long long int end \
start wchar_t end \
start char8_t end \
start char16_t end \
start char32_t end

#define unsigned_int_type_casts(start, end) \
start unsigned char end \
start unsigned int end \
start unsigned short int end \
start unsigned long int end \
start unsigned long long int end \


#define signed_int_type_casts(start, end) \
start signed char end \
start signed int end \
start signed short int end \
start signed long int end \
start signed long long int end \


#define character_type_casts(start, end) \
start char end \
start wchar_t end \
start char8_t end \
start char16_t end \
start char32_t end

#define bool_type_cast(start, end) \
start bool end

#define isolated_cast(cast, start, end) cast({start, end})


template<std::invocable<> T>
bool tester(T f, std::string function_name){
    std::cout << "Testing " << function_name << ": ";
    try{
        f();
        std::cout << "Success!\n";
        return 1;
    }catch(std::exception &e){
        std::cout << "Fail!\n" << e.what() << "\n";
        return 0;
    }
}


// #define test(function) tester(function, #function)
// #define test_inplace(function_body) tester([](){function_body}, "\b")
#define test_named_inplace(name, function_body) tester([](){function_body return 0;}, name)
#define check(expression) if (!(expression)) throw std::runtime_error(#expression" is false on line " + std::to_string(__LINE__))

//#define isolated(body) {body}


int main(int, char **){
    test_named_inplace("default construction", large_num a;);
    test_named_inplace("copy construction",    large_num a; large_num b = a;);
    test_named_inplace("move construction",    large_num a; large_num b = std::move(a););
    test_named_inplace("copy assignment",      large_num a; large_num b; a = b;);
    test_named_inplace("move assignment",      large_num a; large_num b; a = std::move(b););

    test_named_inplace("numerical construction", 
        isolated_cast(unsigned_int_type_casts, large_num a = static_cast<,> (+37);  );
        isolated_cast(unsigned_int_type_casts, large_num a = static_cast<,> (-37);  );
        isolated_cast(signed_int_type_casts,   large_num a = static_cast<,> (+37);  );
        isolated_cast(signed_int_type_casts,   large_num a = static_cast<,> (-37);  );
        isolated_cast(character_type_casts,    large_num a = static_cast<,> (+37);  );
        isolated_cast(character_type_casts,    large_num a = static_cast<,> (-37);  );
        isolated_cast(bool_type_cast,          large_num a = static_cast<,> (true); );
        isolated_cast(bool_type_cast,          large_num a = static_cast<,> (false););
    );

    // doesn't invoke operator= (std::integral)
    test_named_inplace("numerical assignment",
        isolated_cast(unsigned_int_type_casts, large_num a; a = static_cast<,> (+37);  );
        isolated_cast(unsigned_int_type_casts, large_num a; a = static_cast<,> (-37);  );
        isolated_cast(signed_int_type_casts,   large_num a; a = static_cast<,> (+37);  );
        isolated_cast(signed_int_type_casts,   large_num a; a = static_cast<,> (-37);  );
        isolated_cast(character_type_casts,    large_num a; a = static_cast<,> (+37);  );
        isolated_cast(character_type_casts,    large_num a; a = static_cast<,> (-37);  );
        isolated_cast(bool_type_cast,          large_num a; a = static_cast<,> (true); );
        isolated_cast(bool_type_cast,          large_num a; a = static_cast<,> (false););
    );

    test_named_inplace("conversion to numerical types",
        isolated_cast(unsigned_int_type_casts, , a = large_num(+29); check(a == static_cast<decltype(a)>(+29)););
        isolated_cast(signed_int_type_casts, ,   a = large_num(+29); check(a == static_cast<decltype(a)>(+29)););
        isolated_cast(character_type_casts, ,    a = large_num(+29); check(a == static_cast<decltype(a)>(+29)););
        isolated_cast(bool_type_cast, ,          a = large_num(+29); check(a == static_cast<decltype(a)>(+29)););
        isolated_cast(unsigned_int_type_casts, , a = large_num(-29); check(a == static_cast<decltype(a)>(-29)););
        isolated_cast(signed_int_type_casts, ,   a = large_num(-29); check(a == static_cast<decltype(a)>(-29)););
        isolated_cast(character_type_casts, ,    a = large_num(-29); check(a == static_cast<decltype(a)>(-29)););
        isolated_cast(bool_type_cast, ,          a = large_num(-29); check(a == static_cast<decltype(a)>(-29)););
        isolated_cast(unsigned_int_type_casts, , a = large_num(0)  ; check(a == static_cast<decltype(a)>(0));  );
        isolated_cast(signed_int_type_casts, ,   a = large_num(0)  ; check(a == static_cast<decltype(a)>(0));  );
        isolated_cast(character_type_casts, ,    a = large_num(0)  ; check(a == static_cast<decltype(a)>(0));  );
        isolated_cast(bool_type_cast, ,          a = large_num(0)  ; check(a == static_cast<decltype(a)>(0));  );
    );

    #define test_single_comparison_operation(a, operation, b) \
    check((a operation b) == (static_cast<large_num>(a) operation static_cast<large_num>(b)))

    #define test_comparison(a, b) \
    test_single_comparison_operation(a, ==, b);\
    test_single_comparison_operation(a, !=, b);\
    test_single_comparison_operation(a, >, b);\
    test_single_comparison_operation(a, >=, b);\
    test_single_comparison_operation(a, <, b);\
    test_single_comparison_operation(a, <=, b)

    test_named_inplace("comparisons",
        test_comparison(+39, +42);
        test_comparison(+39, -42);
        test_comparison(-39, +42);
        test_comparison(-39, -42);
        test_comparison(+39, 0  );
        test_comparison(0  , +42);
        test_comparison(-39, 0  );
        test_comparison(0  , -42);
        test_comparison(0  , 0  );
    );

    // add arithmetic tests

    // add printing

    return 0;
}
