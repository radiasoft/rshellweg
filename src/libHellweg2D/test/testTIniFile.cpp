#include "IniFiles.hpp"
#include "catch.hpp"

std::string sample_ini = "";

TEST_CASE("TIniFile") {
    TIniFile ini;

    ini.fromString(sample_ini);

    SECTION("get default values") {
        REQUIRE( ini.ReadString("MAIN", "none", "default") == "default");
    }
}  

