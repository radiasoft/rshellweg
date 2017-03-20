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

    SECTION("c string assignment") {
        const char ts[] = "Test String";
        AnsiString as;

        as = ts;

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

TEST_CASE("AnsiString: operator+=") {
    SECTION("AnsiString =+ AnsiString") {
        AnsiString as1("as1");
        AnsiString as2("as2");

        REQUIRE( as1 == "as1" );
        as1+=as2;
        REQUIRE( as1 == "as1as2" );
    }

    SECTION("AnsiString =+ char") {
        AnsiString as1("as1");

        REQUIRE( as1 == "as1" );
        as1+='c';
        REQUIRE( as1 == "as1c" );
    }
}

TEST_CASE("AnsiString: operator[]") {
    AnsiString as1("as1");

    REQUIRE( as1[1] == 'a' );
    REQUIRE( as1[2] == 's' );
    REQUIRE( as1[3] == '1' );
    REQUIRE( as1[4] == '\0' );
}

TEST_CASE("AnsiString: Length") {
    AnsiString as1("as1");

    REQUIRE( as1.Length() == 3);
}

TEST_CASE("AnsiString: FormatFloat") {
    AnsiString f1 = AnsiString::FormatFloat("#0", 0.1);
    AnsiString f2 = AnsiString::FormatFloat("#0.0", 0.1);
    AnsiString f3 = AnsiString::FormatFloat("#0.0000000", 0.1);
    AnsiString f4 = AnsiString::FormatFloat("", 0.1);

    REQUIRE( f1 == "0" );
    REQUIRE( f2 == "0.1" );
    REQUIRE( f3 == "0.1000000" );
    REQUIRE( f4 == "0.100000" );
}
