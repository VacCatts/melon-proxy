#include <Windows.h>
#include "auth.hpp"
#include <string>
#include "skStr.h"
#include "authentication.hpp"

//using namespace KeyAuth;

std::string name = ""; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = ""; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = ""; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.0"; // leave alone unless you've changed version on website
std::string url = "https://keyauth.win/api/1.2/"; // change if you're self-hosting

KeyAuth::api KeyAuthApp(name, ownerid, secret, version, url);

bool authentication::login(std::string username) {
    KeyAuthApp.init();
    KeyAuthApp.login(username, "defaultpassword");
    if (KeyAuthApp.data.success)
        return true;

    return false;
}

bool authentication::createaccount(std::string username, std::string license) {
    KeyAuthApp.init();
    KeyAuthApp.regstr(username, "defaultpassword", license);
    if (KeyAuthApp.data.success)
        return true;

    return false;
}