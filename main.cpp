#include "fraction.hpp"
//#include "overlow_detect.hpp"
//#include <climits>
#include <cassert>
#include <string>
#include <ctime>

#define RUN_TESTS 1

#if RUN_TESTS
    #define int_type_casts(start, end) \
    start bool end \
    start char end \
    start signed char end \
    start signed short int end \
    start signed int end \
    start signed long int end \
    start signed long long int end \
    start unsigned char end \
    start unsigned short int end \
    start unsigned int end \
    start unsigned long int end \
    start unsigned long long int end \
    start wchar_t end \
    start char8_t end \
    start char16_t end \
    start char32_t end

    #define unsigned_int_type_casts(start, end) \
    start unsigned char end \
    start unsigned short int end \
    start unsigned int end \
    start unsigned long int end \
    start unsigned long long int end \


    #define signed_int_type_casts(start, end) \
    start signed char end \
    start signed short int end \
    start signed int end \
    start signed long int end \
    start signed long long int end \


    #define character_type_casts(start, end) \
    start char end \
    start char8_t end \
    start char16_t end \
    start char32_t end \
    start wchar_t end

    #define bool_type_cast(start, end) \
    start bool end

    #define isolated_cast(cast, start, end) cast({start, end})


    template<std::invocable<> T>
    bool tester(T f, std::string function_name){
        std::cout << "Testing " << function_name << ": ";
        std::flush(std::cout);
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

    #define test_arithmetic_unary_prefix(operator, a, result) \
    check((operator static_cast<large_num>(a)) == static_cast<large_num>(result))

    #define test_arithmetic_unary_postfix(operator, a, result1, result2) \
    check_with_prerequisite(large_num a_large = a,\
    a_large operator == static_cast<large_num>(result1) && a_large == static_cast<large_num>(result2))

    #define test_arithmetic_binary(a, operator, b, result) \
    check((static_cast<large_num>(a) operator static_cast<large_num>(b)) == static_cast<large_num>(result))

    class expected_exception: public std::exception{};

    #define expect_exception(expression) \
    try{\
        expression; throw expected_exception{};\
    }catch(expected_exception &e[[maybe_unused]]){\
        throw std::runtime_error(#expression" did not throw an exception on line " + std::to_string(__LINE__));\
    }catch(...){}

    #define undefined_value (-0)
#else
    #define int_type_casts(start, end)
    #define unsigned_int_type_casts(start, end)
    #define signed_int_type_casts(start, end)
    #define character_type_casts(start, end)
    #define bool_type_cast(start, end)
    #define isolated_cast(cast, start, end)
    template<std::invocable<> T>
    bool tester(T f, std::string function_name);
    // #define test(function) tester(function, #function)
    // #define test_inplace(function_body) tester([](){function_body}, "\b")
    #define test_named_inplace(name, function_body)
    #define check(expression)
    #define check_with_prerequisite(prerequisite, expression)
    //#define isolated(body) {body}
    #define test_single_comparison_operation(a, operator, b)
    #define test_comparison(a, b)
    #define test_arithmetic_unary_prefix(operator, a, result)
    #define test_arithmetic_unary_postfix(operator, a, result1, result2)
    #define test_arithmetic_binary(a, operator, b, result)
    #define expect_exception(expression)
#endif

#if PRIVATE_ACCESS
    void print_large_num_content(const large_num &num){
        std::cout << "Contents: ";
        for (auto &chunk : num.storage){
            std::cout << "[" << (uintmax_t)chunk << "] ";
        }
        std::cout << "\b.\n";
    }
#else
    void print_large_num_content(const large_num &num){}
#endif


int main(int, char **){
    // tests for small (-128 < . < 128) numbers


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
        isolated_cast(unsigned_int_type_casts, , a = large_num(static_cast<decltype(a)>(0))  ; check(a == static_cast<decltype(a)>(0));  );
        isolated_cast(unsigned_int_type_casts, , a = large_num(static_cast<decltype(a)>(+29)); check(a == static_cast<decltype(a)>(+29)););
        isolated_cast(unsigned_int_type_casts, , a = large_num(static_cast<decltype(a)>(-29)); check(a == static_cast<decltype(a)>(-29)););
        isolated_cast(signed_int_type_casts, ,   a = large_num(static_cast<decltype(a)>(0))  ; check(a == static_cast<decltype(a)>(0));  );
        isolated_cast(signed_int_type_casts, ,   a = large_num(static_cast<decltype(a)>(+29)); check(a == static_cast<decltype(a)>(+29)););
        isolated_cast(signed_int_type_casts, ,   a = large_num(static_cast<decltype(a)>(-29)); check(a == static_cast<decltype(a)>(-29)););
        isolated_cast(character_type_casts, ,    a = large_num(static_cast<decltype(a)>(0))  ; check(a == static_cast<decltype(a)>(0));  );
        isolated_cast(character_type_casts, ,    a = large_num(static_cast<decltype(a)>(+29)); check(a == static_cast<decltype(a)>(+29)););
        isolated_cast(character_type_casts, ,    a = large_num(static_cast<decltype(a)>(-29)); check(a == static_cast<decltype(a)>(-29)););
        isolated_cast(bool_type_cast, ,          a = large_num(static_cast<decltype(a)>(0))  ; check(a == static_cast<decltype(a)>(0));  );
        isolated_cast(bool_type_cast, ,          a = large_num(static_cast<decltype(a)>(+29)); check(a == static_cast<decltype(a)>(+29)););
        isolated_cast(bool_type_cast, ,          a = large_num(static_cast<decltype(a)>(-29)); check(a == static_cast<decltype(a)>(-29)););
    );


    // comparison
    test_named_inplace("comparison",
        test_comparison(0  , 0  );
        test_comparison(+39, 0  );
        test_comparison(0  , +42);
        test_comparison(-39, 0  );
        test_comparison(0  , -42);
        test_comparison(+39, +39);
        test_comparison(-42, -42);
        test_comparison(+39, +42);
        test_comparison(+39, -42);
        test_comparison(-39, +42);
        test_comparison(-39, -42);
    );


    // arithmetic evaluation
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
        test_arithmetic_binary( 33, -,  12,  21);
        test_arithmetic_binary( 33, -, -12,  45);
        test_arithmetic_binary(-33, -,  12, -45);
        test_arithmetic_binary(-33, -, -12, -21);
    );

    test_named_inplace("multiplication",
        test_arithmetic_binary(  0, *,   0,   0);
        test_arithmetic_binary( 12, *,   0,   0);
        test_arithmetic_binary(  0, *,  47,   0);
        test_arithmetic_binary(-12, *,   0,   0);
        test_arithmetic_binary(  0, *, -47,   0);
        test_arithmetic_binary( 13, *,   6,  78);
        test_arithmetic_binary(-13, *,   6, -78);
        test_arithmetic_binary( 13, *,  -6, -78);
        test_arithmetic_binary(-13, *,  -6,  78);
    );

    test_named_inplace("division",
        expect_exception(test_arithmetic_binary(  0, /, 0, undefined_value));
        expect_exception(test_arithmetic_binary( 22, /, 0, undefined_value));
        expect_exception(test_arithmetic_binary(-22, /, 0, undefined_value));
        test_arithmetic_binary(  0, /,  22,  0);
        test_arithmetic_binary(  0, /, -22,  0);
        test_arithmetic_binary( 66, /,  22,  3);
        test_arithmetic_binary( 66, /, -22, -3);
        test_arithmetic_binary(-66, /,  22, -3);
        test_arithmetic_binary(-66, /, -22,  3);
        test_arithmetic_binary( 97, /,  22,  4);
        test_arithmetic_binary( 97, /, -22, -4);
        test_arithmetic_binary(-97, /,  22, -4);
        test_arithmetic_binary(-97, /, -22,  4);
    );

    test_named_inplace("modulo",
        expect_exception(test_arithmetic_binary(  0, %, 0, undefined_value));
        expect_exception(test_arithmetic_binary( 22, %, 0, undefined_value));
        expect_exception(test_arithmetic_binary(-22, %, 0, undefined_value));
        test_arithmetic_binary(  0, %,  22,  0);
        test_arithmetic_binary(  0, %, -22,  0);
        test_arithmetic_binary( 66, %,  22,  0);
        test_arithmetic_binary( 66, %, -22,  0);
        test_arithmetic_binary(-66, %,  22,  0);
        test_arithmetic_binary(-66, %, -22,  0);
        test_arithmetic_binary( 97, %,  22,  9);
        test_arithmetic_binary( 97, %, -22,  9);
        test_arithmetic_binary(-97, %,  22, -9);
        test_arithmetic_binary(-97, %, -22, -9);
    );

    test_named_inplace("bitwise AND",
        test_arithmetic_binary(  0, &,   0,   0);
        test_arithmetic_binary( 22, &,   0,   0);
        test_arithmetic_binary(  0, &,  22,   0);
        test_arithmetic_binary(-22, &,   0,   0);
        test_arithmetic_binary(  0, &, -22,   0);
        test_arithmetic_binary( 25, &,  19,  17);
        test_arithmetic_binary( 25, &, -19,   9);
        test_arithmetic_binary(-25, &,  19,   3);
        test_arithmetic_binary(-25, &, -19, -27);
    );

    test_named_inplace("bitwise OR",
        test_arithmetic_binary(  0, |,   0,   0);
        test_arithmetic_binary( 22, |,   0,  22);
        test_arithmetic_binary(  0, |,  22,  22);
        test_arithmetic_binary(-22, |,   0, -22);
        test_arithmetic_binary(  0, |, -22, -22);
        test_arithmetic_binary( 25, |,  19,  27);
        test_arithmetic_binary( 25, |, -19,  -3);
        test_arithmetic_binary(-25, |,  19,  -9);
        test_arithmetic_binary(-25, |, -19, -17);
    );

    test_named_inplace("bitwise XOR",
        test_arithmetic_binary(  0, ^,   0,   0);
        test_arithmetic_binary( 22, ^,   0,  22);
        test_arithmetic_binary(  0, ^,  22,  22);
        test_arithmetic_binary(-22, ^,   0, -22);
        test_arithmetic_binary(  0, ^, -22, -22);
        test_arithmetic_binary( 25, ^,  19,  10);
        test_arithmetic_binary( 25, ^, -19, -12);
        test_arithmetic_binary(-25, ^,  19, -12);
        test_arithmetic_binary(-25, ^, -19,  10);
    );

    test_named_inplace("left shift",
        expect_exception(test_arithmetic_binary(  0, <<, -3, undefined_value));
        expect_exception(test_arithmetic_binary( 12, <<, -3, undefined_value));
        expect_exception(test_arithmetic_binary(-12, <<, -3, undefined_value));
        test_arithmetic_binary(  0, <<,   0,    0);
        test_arithmetic_binary(  0, <<,   3,    0);
        test_arithmetic_binary( 15, <<,   0,   15);
        test_arithmetic_binary(-15, <<,   0,  -15);
        test_arithmetic_binary( 15, <<,   3,  120);
        test_arithmetic_binary(-15, <<,   3, -120);
    );

    test_named_inplace("right shift",
        expect_exception(test_arithmetic_binary(  0, >>, -3, undefined_value));
        expect_exception(test_arithmetic_binary( 12, >>, -3, undefined_value));
        expect_exception(test_arithmetic_binary(-12, >>, -3, undefined_value));
        test_arithmetic_binary(  0, >>,   0,   0);
        test_arithmetic_binary(  0, >>,   3,   0);
        test_arithmetic_binary( 15, >>,   0,  15);
        test_arithmetic_binary(-15, >>,   0, -15);
        test_arithmetic_binary( 15, >>,   3,  1);
        test_arithmetic_binary(-15, >>,   3, -2);
    );

/*
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
*/

    test_named_inplace("string conversions",
        check(static_cast<std::string>(static_cast<large_num>(""))   == "0");
        check(static_cast<std::string>(static_cast<large_num>("0"))  == "0");
        check(static_cast<std::string>(static_cast<large_num>("+0")) == "0");
        check(static_cast<std::string>(static_cast<large_num>("-0")) == "0");
        {
            std::string s;
            std::cout << '\n';
            for (size_t i = 1; i < 100; ++i){
                s.push_back('0' + std::rand() % 10);
                std::cout << i << ": " << s << '\n';
                check(static_cast<std::string>(static_cast<large_num>(s)) == s);
            }
        }
        {
            std::string s;
            std::cout << '\n';
            for (size_t i = 1; i < 100; ++i){
                s.push_back('0' + std::rand() % 10);
                std::cout << i << ": " << '-' + s << '\n';
                check(static_cast<std::string>(static_cast<large_num>('-' + s)) == '-' + s);
            }
        }
        {
            std::string s;
            std::cout << '\n';
            for (size_t i = 1; i < 100; ++i){
                s.push_back('0' + std::rand() % 10);
                std::cout << i << ": " << '+' + s << '\n';
                check(static_cast<std::string>(static_cast<large_num>('+' + s)) == s);
            }
        }
    );

    test_named_inplace("alternating fibonacci",
            auto time1 = std::time(nullptr);
        std::cout << '\n';
        large_num factorial = 1;
        for (intmax_t i = 1; i <= 100; ++i){
            factorial *= -i;
            //std::cout << i << "! = " << factorial << '\n';
        }
        
        for (intmax_t i = 101; i --> 1;){
            factorial /= -i;
            //std::cout << i-1 << "! = " << factorial << '\n';
        }
            auto time2 = std::time(nullptr);
            std::cout << time2 - time1 << '\n';
    );

    return 0;
}
