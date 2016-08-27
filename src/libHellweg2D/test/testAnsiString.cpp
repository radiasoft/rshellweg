#include <cstring>
#include "AnsiString.hpp"
#include "catch.hpp"

#define CSTR_EQUAL(X, Y) REQUIRE( std::strcmp(X, Y) == 0 )

TEST_CASE("AnsiString: instantiation") {
    SECTION("default constructor") {
        AnsiString as;

        CSTR_EQUAL(as.c_str(), "");
    }

    SECTION("string constructor") {
        const char ts[] = "Test String";
        AnsiString as(ts);
        
        CSTR_EQUAL(as.c_str(), ts);
    }
}

TEST_CASE("AnsiString: test equality operators") {
    const char ts[] = "Test String";
    AnsiString as1(ts);
    AnsiString as2(ts);

    CSTR_EQUAL(as1.c_str(), ts);
    CSTR_EQUAL(as2.c_str(), ts);
    CSTR_EQUAL(as1.c_str(), as2.c_str());

    SECTION("operator==") {
        REQUIRE(as1 == as2);
        REQUIRE(as1 == ts);
        REQUIRE(as2 == ts);
        REQUIRE(ts == as1);
    }

    SECTION("operator!=") {
        AnsiString diff;

        REQUIRE(as1 != diff);
        REQUIRE(as2 != diff);
        REQUIRE_FALSE(as1 != as2);
        REQUIRE(as1 != "");
        REQUIRE("" != as1);
    }
}

TEST_CASE("AnsiString: operator+") {
    SECTION("AnsiString + AnsiString") {
        AnsiString as1("as1");
        AnsiString as2("as2");
        AnsiString as3("as3");

        REQUIRE( as1+as2+as3 == "as1as2as3" );
    }

    SECTION("AnsiString + c string") {
        AnsiString as1("as1");
        AnsiString as2("as2");
        const char c[] = ".";

        REQUIRE( as1+c == "as1." );
        REQUIRE( c+as2 == ".as2" );
        REQUIRE( as1+c+as2 == "as1.as2" );
    }
}
