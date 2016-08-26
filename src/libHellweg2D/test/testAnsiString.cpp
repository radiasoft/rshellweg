#include <cstring>
#include "AnsiString.hpp"
#include "catch.hpp"

TEST_CASE("instantiate AnsiString") {
    SECTION("default constructor") {
        AnsiString as;

        REQUIRE( std::strcmp(as.c_str(), "") == 0 );
    }

    SECTION("string constructor") {
        const char ts[] = "Test String";
        AnsiString as(ts);
        
        REQUIRE( std::strcmp(as.c_str(), ts) == 0 );
    }
}
