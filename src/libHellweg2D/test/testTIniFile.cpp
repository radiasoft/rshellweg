#include "IniFiles.hpp"
#include "catch.hpp"

std::string sample_ini = \
"[MAIN]                   \n" \
"string = this is a string\n" \
"integer = 123            \n" \
"float = 456.789          \n";   

TEST_CASE("TIniFile") {
    TIniFile ini;

    ini.fromString(sample_ini);

    SECTION("get default values") {
        REQUIRE( ini.ReadString("none", "none", "default") == "default");
        REQUIRE( ini.ReadFloat("none", "none", 1.0) == 1.0);
        REQUIRE( ini.ReadInteger("none", "none", 1) == 1);
    }
    
    SECTION("do not get default values") {
        REQUIRE( ini.ReadString("MAIN", "string", "default") == "this is a string");
        REQUIRE( ini.ReadFloat("MAIN", "float", 1.0) == 456.789);
        REQUIRE( ini.ReadInteger("MAIN", "integer", 1) == 123);
    }

    SECTION("case insensitivity") {
        REQUIRE( ini.ReadString("mAiN", "sTrInG", "default") == "this is a string");
    }
}  

