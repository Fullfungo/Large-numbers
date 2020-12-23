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
#define check_with_prerequisite(prerequisite, expression) if (prerequisite; !(expression)) throw std::runtime_error(#expression" is false on line " + std::to_string(__LINE__))

//#define isolated(body) {body}


int main(int, char **){

    // simple construction
    test_named_inplace("default construction", large_num a;);
    test_named_inplace("copy construction",    large_num a; large_num b = a;);
    test_named_inplace("move construction",    large_num a; large_num b = std::move(a););
    

    // simple assignment
    test_named_inplace("copy assignment",      large_num a; large_num b; a = b;);
    test_named_inplace("move assignment",      large_num a; large_num b; a = std::move(b););


    // numerical construction
        // add huge numbers
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


    // numerical conversion
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


    // comparison
    #define test_single_comparison_operation(a, operator, b) \
    check((a operator b) == (static_cast<large_num>(a) operator static_cast<large_num>(b)))

    #define test_comparison(a, b) \
    test_single_comparison_operation(a, ==,  b);\
    test_single_comparison_operation(a, !=,  b);\
    test_single_comparison_operation(a, >,   b);\
    test_single_comparison_operation(a, >=,  b);\
    test_single_comparison_operation(a, <,   b);\
    test_single_comparison_operation(a, <=,  b);\
    test_single_comparison_operation(a, <=>, b)

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


    // arithmetic evaluation
    #define test_arithmetic_unary_prefix(operator, a, result) \
    check(operator static_cast<large_num>(a) == static_cast<large_num>(result))

    #define test_arithmetic_unary_postfix(operator, a, result1, result2) \
    check_with_prerequisite(large_num a_large = a,\
    a_large operator == static_cast<large_num>(result1) && a_large == static_cast<large_num>(result2))

    #define test_arithmetic_binary(a, operator, b, result) \
    check(static_cast<large_num>(a) operator static_cast<large_num>(b) == static_cast<large_num>(result))

        // add huge numbers

    test_named_inplace("affirmation (unary plus)",
        test_arithmetic_unary_prefix(+,   0,   0);
        test_arithmetic_unary_prefix(+,  33,  33);
        test_arithmetic_unary_prefix(+,  64,  64);
        test_arithmetic_unary_prefix(+, -45, -45);
        test_arithmetic_unary_prefix(+,  16,  16);
    );

    test_named_inplace("negation (unary minus)",
        test_arithmetic_unary_prefix(-,   0,   0);
        test_arithmetic_unary_prefix(-,   2,  -2);
        test_arithmetic_unary_prefix(-,  35, -35);
        test_arithmetic_unary_prefix(-, -55,  55);
        test_arithmetic_unary_prefix(-, -64,  64);
    );

    test_named_inplace("complement (bitwise NOT)",
        test_arithmetic_unary_prefix(~,  0,  -1);
        test_arithmetic_unary_prefix(~,  -1,  0);
        test_arithmetic_unary_prefix(~,  34, -35);
        test_arithmetic_unary_prefix(~,  63, -64);
        test_arithmetic_unary_prefix(~, -22,  21);
        test_arithmetic_unary_prefix(~, -64,  63);
    );

    test_named_inplace("pre-increment",
        test_arithmetic_unary_prefix(++,   0,   1);
        test_arithmetic_unary_prefix(++,  -1,   0);
        test_arithmetic_unary_prefix(++,   5,   6);
        test_arithmetic_unary_prefix(++,  63,  64);
        test_arithmetic_unary_prefix(++,  -5,  -4);
        test_arithmetic_unary_prefix(++, -65, -64);
    );

    test_named_inplace("pre-decrement",
        test_arithmetic_unary_prefix(--,   0,  -1);
        test_arithmetic_unary_prefix(--,   1,   0);
        test_arithmetic_unary_prefix(--,  23,  22);
        test_arithmetic_unary_prefix(--,  64,  63);
        test_arithmetic_unary_prefix(--, -33, -34);
        test_arithmetic_unary_prefix(--, -64, -65);
    );

    test_named_inplace("post-increment",
        test_arithmetic_unary_postfix(++,   0,   0,   1);
        test_arithmetic_unary_postfix(++,  -1,  -1,   0);
        test_arithmetic_unary_postfix(++,   5,   5,   6);
        test_arithmetic_unary_postfix(++,  63,  63,  64);
        test_arithmetic_unary_postfix(++,  -5,  -5,  -4);
        test_arithmetic_unary_postfix(++, -65, -65, -64);
    );

    test_named_inplace("post-decrement",
        test_arithmetic_unary_postfix(--,   0,   0,  -1);
        test_arithmetic_unary_postfix(--,   1,   1,   0);
        test_arithmetic_unary_postfix(--,  23,  23,  22);
        test_arithmetic_unary_postfix(--,  64,  64,  63);
        test_arithmetic_unary_postfix(--, -33, -33, -34);
        test_arithmetic_unary_postfix(--, -64, -64, -65);
    );

    test_named_inplace("summation",
        test_arithmetic_binary(  0, +,   0,   0);
        test_arithmetic_binary(  0, +,  47,  47);
        test_arithmetic_binary( 22, +,   0,  22);
        test_arithmetic_binary(  0, +, -47, -47);
        test_arithmetic_binary(-22, +,   0, -22);
        test_arithmetic_binary( 22, +,  47,  69);
        test_arithmetic_binary( 22, +, -47, -25);
        test_arithmetic_binary(-22, +,  47,  25);
        test_arithmetic_binary(-22, +, -47, -69);
    );

    test_named_inplace("subtraction",
        test_arithmetic_binary(  0, -,   0,   0);
        test_arithmetic_binary( 33, -,   0,  33);
        test_arithmetic_binary(  0, -,  12, -12);
        test_arithmetic_binary(-33, -,   0, -33);
        test_arithmetic_binary(  0, -, -12,  12);
        test_arithmetic_binary( 33, -,  12,  11);
        test_arithmetic_binary( 33, -, -12,  45);
        test_arithmetic_binary(-33, -,  12, -45);
        test_arithmetic_binary(-33, -, -12, -11);
    );

    test_named_inplace("multiplication",
        ;
    );

    test_named_inplace("division",
        ;
    );

    test_named_inplace("modulo",
        ;
    );

    test_named_inplace("bitwise AND",
        ;
    );

    test_named_inplace("bitwise OR",
        ;
    );

    test_named_inplace("bitwise XOR",
        ;
    );

    test_named_inplace("left shift",
        ;
    );

    test_named_inplace("right shift",
        ;
    );


    // arithmetic + assignment
    test_named_inplace("summation assignment",
        ;
    );

    test_named_inplace("subtraction assignment",
        ;
    );

    test_named_inplace("multiplication assignment",
        ;
    );

    test_named_inplace("division assignment",
        ;
    );

    test_named_inplace("modulo assignment",
        ;
    );

    test_named_inplace("bitwise AND assignment",
        ;
    );

    test_named_inplace("bitwise OR assignment",
        ;
    );

    test_named_inplace("bitwise XOR assignment",
        ;
    );

    test_named_inplace("left shift assignment",
        ;
    );

    test_named_inplace("right shift assignment",
        ;
    );


    // add pointer arithmetic
    test_named_inplace("pointer-number addition",
        ;
    );

    test_named_inplace("pointer-number subtraction",
        ;
    );


    // add printing/reading
    test_named_inplace("printing to an output",
        ;
    );

    test_named_inplace("reading from an input",
        ;
    );

    return 0;
}
