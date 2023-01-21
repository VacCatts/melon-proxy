#include "events.h"
#include "gt.hpp"
#include "proton/hash.hpp"
#include "proton/rtparam.hpp"
#include "proton/variant.hpp"
#include "server.h"
#include "utils.h"
#include <thread>
#include <limits.h>
#include "HTTPRequest.hpp"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "dialog.h"
using namespace std;

class PuzzleSolver {
public:
    bool Solved = false;
    string LatestAnswer = "";
    PuzzleSolver(string g) : SecretKey(g) {}
    void AddParams(string Key, string Value)
    {
        Params.append(Key + "=" + Value + "&");
    }

    string GetAnswer(string CaptchaUID)
    {
        AddParams("Action", "Solve");
        AddParams("Puzzle", CaptchaUID);
        AddParams("Secret", SecretKey);
        AddParams("Format", "txt");
        string FullUrl = API + Path + Params;
        http::Request request{ string{FullUrl}.c_str()};
        const auto response = request.send("GET");
        string captchaAnswer = string{ response.body.begin(), response.body.end() };
        Solved = captchaAnswer.find("Failed") == string::npos && captchaAnswer.length() > 6 && response.status==202;

        return LatestAnswer = (captchaAnswer.length() > 6) ? captchaAnswer.erase(0, 7) : "Failed";

    }
private:
    string SecretKey;
    string API = "http://api.surferwallet.net/";
    string Path = "Captcha?";
    string Params = "";
};

bool events::out::variantlist(gameupdatepacket_t* packet) {
    variantlist_t varlist{};
    varlist.serialize_from_mem(utils::get_extended(packet));
    PRINTS("varlist: %s\n", varlist.print().c_str());
    return false;
}
vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

bool events::out::pingreply(gameupdatepacket_t* packet) {
    //since this is a pointer we do not need to copy memory manually again
    packet->m_vec2_x = 1000.f;  //gravity
    packet->m_vec2_y = 250.f;   //move speed
    packet->m_vec_x = 64.f;     //punch range
    packet->m_vec_y = 64.f;     //build range
    packet->m_jump_amount = 0;  //for example unlim jumps set it to high which causes ban
    packet->m_player_flags = 0; //effect flags. good to have as 0 if using mod noclip, or etc.
    return false;
}

void events::in::writetofile(gameupdatepacket_t* packet) {
    /*std::ofstream sethost("D:\\Code\\C++\\kusiproxy\\x64\\Release\\text.txt");

    if (sethost.is_open()) {
        sethost << packet;
        sethost.close();
    }*/
    cout << packet;
}

void events::in::writetofile2(string str) {
    /*std::ofstream sethost("D:\\Code\\C++\\kusiproxy\\x64\\Release\\text.txt");

    if (sethost.is_open()) {
        sethost << str;
        sethost.close();
    }*/
    cout << str;
}

bool find_command(string chat, string name) {
    bool found = chat.find("/" + name) == 0;
    if (found)
        gt::send_log("`6" + chat);
    return found;
}

// TOGGLES
bool fastdrop2 = false;
bool fastdrop = false;
string fdcount = "0";
bool fasttrash = false;
bool daw = false;

// IDFK
bool takingunacc = false;
bool takingacc = false;
bool changingname = false;
string name2 = "NULL";
string originalname = "NULL";

// BLINK CONFIG
bool blink = false;
bool startedBlink = false;
int blinkstate = 0;

// MOD DETECTOR CONFIG
bool leaveworld = true;
bool takeunaccess = true;
bool banall = true;

// WRENCH MODE
bool wrench = true;
string mode = "pull";

// GAS PULL
bool gaspull = false;
bool automaticoff = false;

// TITLES
bool drtitle = false;
bool maxlevel = false;
bool legend = false;
bool g4gtitle = false;

// UTILS
void force() {
    Sleep(200);
    fastdrop2 = false;
}

void forceunacc() {
    Sleep(200);
    takingunacc = false;
}

void forceacc() {
    Sleep(500);
    takingacc = false;
}

void startBlink() {
    startedBlink = true;
    while (blink) {
        g_server->send(false, "action|setSkin\ncolor|3370516479");
        Sleep(300);
        g_server->send(false, "action|setSkin\ncolor|1348237567");
        Sleep(300);
        g_server->send(false, "action|setSkin\ncolor|2190853119");
        Sleep(300);
        g_server->send(false, "action|setSkin\ncolor|1685231359");
        Sleep(300);
        g_server->send(false, "action|setSkin\ncolor|2527912447");
        Sleep(300);
        g_server->send(false, "action|setSkin\ncolor|2864971775");
        Sleep(300);
        g_server->send(false, "action|setSkin\ncolor|3033464831");
        Sleep(300);
    }
}

string toString(int number) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
}

void forcechangingname() {
    Sleep(200);
    changingname = false;
}

/*void DropItem(int itemid, int count) {
    g_server->send(false, "action|drop\nitemID|" + toString(itemid));
    Sleep(500);
    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + toString(itemid) + "\ncount|" + toString(count) + "\n");
    Sleep(250);
}*/

void DropItem(int itemid, int count) {
    fastdrop2 = true;
    string packetC = "action|drop\nitemID|" + toString(itemid);
    g_server->send(false, packetC);
    Sleep(50);
    string packetD = "action|dialog_return\ndialog_name|drop_item\nitemID|" + toString(itemid) + "\ncount|" + toString(count) + "\n";
    g_server->send(false, packetD);
    thread(force).detach();
}

void DropItem(int itemid, string count) {
    fastdrop2 = true;
    string packetC = "action|drop\nitemID|" + toString(itemid);
    g_server->send(false, packetC);
    Sleep(50);
    string packetD = "action|dialog_return\ndialog_name|drop_item\nitemID|" + toString(itemid) + "\ncount|" + count + "\n";
    g_server->send(false, packetD);
    thread(force).detach();
}

void DropItem(int itemid) {
    string packetD = "action|dialog_return\ndialog_name|drop_item\nitemID|" + toString(itemid) + "\ncount|1\n";
    fastdrop2 = true;
    Sleep(50);
    for (server::Item& item : server::inventory) {
        if (item.id == itemid) {
            string packetC = "action|drop\nitemID|" + toString(itemid);
            g_server->send(false, packetC);
            Sleep(200);
            if (item.count == 1) {
                string packetD = "action|dialog_return\ndialog_name|drop_item\nitemID|" + toString(itemid) + "\ncount|1\n";
            }
            g_server->send(false, packetD);
            return;
        }
    }
    thread(force).detach();
}

void DoChat(string text) {
    g_server->send(false, "action|input\n|text|" + text);
    Sleep(5);
    return;
}

void Warp(string text) {
    g_server->send(false, "action|join_request\nname|" + text, 3);
    Sleep(5);
    return;
}

void PullUsername(string username) {
    for (auto& player : g_server->m_world.players) {
        auto name_2 = player.name.substr(2); //remove color
        if (name_2.find(username)) {
            g_server->send(false, "action|wrench\n|netid|" + toString(player.netid));
            g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + toString(player.netid) + "|\nbuttonClicked|pull");
        }
    }
    return;
}

void PullNetID(string netid) {
    g_server->send(false, "action|wrench\n|netid|" + netid);
    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + netid + "|\nbuttonClicked|pull");
    return;
}

void TakeUnaccess() {
    takingunacc = true;
    DoChat("/unaccess");
    Sleep(2000);
    g_server->send(false, "action|dialog_return\n|dialog_name|unaccess");
    thread(forceunacc).detach();
    return;
}

void TakeUnaccessFast() {
    takingunacc = true;
    DoChat("/unaccess");
    g_server->send(false, "action|dialog_return\n|dialog_name|unaccess");
    thread(forceunacc).detach();
    return;
}

void BanAll() {
    for (player p : g_server->m_world.players) {
        g_server->send(false, "action|wrench\n|netid|" + toString(p.netid));
        this_thread::sleep_for(chrono::milliseconds(5));
        g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + toString(p.netid) + "|\nbuttonClicked|worldban");
        this_thread::sleep_for(chrono::milliseconds(50));
    }
    return;
}

void ModJoinRoutine(string modgrowid) {
    variantlist_t Notification { "OnAddNotification" };
    Notification[1] = "interface/science_button.rttex";
    Notification[2] = "kebulan omistaja " + modgrowid + " tuli maahan";
    Notification[3] = "audio/hub_open.wav";
    g_server->send(true, Notification);

    gt::send_log("kebulan omistaja " + modgrowid + " tuli maahan");

    if (banall)
        BanAll();
    if (takeunaccess)
        TakeUnaccessFast();
    if (leaveworld)
        Warp("EXIT");
}

void Sleep(int milliseconds) {
    this_thread::sleep_for(chrono::microseconds(milliseconds));
}

void ChangeName(string name) {
    variantlist_t va{ "OnNameChanged" };
    va[1] = name;
    g_server->send(true, va, g_server->m_user, -1);
}

bool itsmod(int netid, string growid = "") {
    if (netid == g_server->m_world.local.netid)
        return false;
    else if (growid.find(g_server->m_world.local.name) != string::npos)
        return false;
    this_thread::sleep_for(chrono::milliseconds(20));
    for (auto g : g_server->m_world.players)
    {
        this_thread::sleep_for(chrono::microseconds(5));
        transform(g.name.begin(), g.name.end(), g.name.begin(), [](unsigned char c) { return std::tolower(c); });
        if (netid == g.netid)
            return false;
        else if  (growid.find(g.name) != string::npos) 
            return false;

    }
    ModJoinRoutine(growid);
    return true;
}

bool events::out::generictext(string packet) {
    if (!packet.find("tankIDPass|"))
        PRINTS("Generic text: %s\n", packet.c_str());

    auto& world = g_server->m_world;
    rtvar var = rtvar::parse(packet);
    if (!var.valid())
        return false;

    if (packet.find("buttonClicked|title") != -1) {
        Dialog title;
        title.addLabelWithIcon("Title", 11816, LABEL_BIG);

        title.addCheckbox("drtitle", "Dr.", drtitle);
        title.addCheckbox("maxlevel", "Level 125", maxlevel);
        title.addCheckbox("legend", "of legend", legend);
        title.addCheckbox("g4gtitle", "G4G Title", g4gtitle);

        title.addSpacer(SPACER_SMALL);
        title.endDialog("titledialog", "Okay", "Cancel");
        variantlist_t packet{ "OnDialogRequest" };
        packet[1] = title.finishDialog();
        g_server->send(true, packet);

    }

    if (packet.find("drtitle|") != -1) {
        string istoggled = packet.substr(packet.find("tle|") + 4, 1);
        cout << istoggled;
        if (istoggled == "1") {
            string name = "`4Dr." + g_server->m_world.local.name + "``";
            variantlist_t va{ "OnNameChanged" };
            va[1] = name;
            g_server->send(true, va, world.local.netid, -1);
            drtitle = true;
        }
        else {
            drtitle = false;
        }
    }

    if (packet.find("maxlevel|") != -1) {
        string istoggled = packet.substr(packet.find("vel|") + 4, 1);
        cout << istoggled;
        if (istoggled == "1") {
            maxlevel = true;
        }
        else {
            maxlevel = false;
        }
    }

    if (packet.find("legend|") != -1) {
        string istoggled = packet.substr(packet.find("end|") + 4, 1);
        cout << istoggled;
        if (istoggled == "1") {
            string name = "``" + g_server->m_world.local.name + " of Legend``";
            variantlist_t va{ "OnNameChanged" };
            va[1] = name;
            g_server->send(true, va, world.local.netid, -1);
            legend = true;
        }
        else {
            legend = false;
        }
    }

    if (packet.find("g4gtitle|") != -1) {
        string istoggled = packet.substr(packet.find("tle|") + 4, 1);
        if (istoggled == "1") {
            g4gtitle = true;
        }
        else {
            g4gtitle = false;
        }
        return true;
    }

    if (wrench == true) {
        if (packet.find("action|wrench") != -1) {
            g_server->send(false, packet);
            string sr = packet.substr(packet.find("netid|") + 6, packet.length() - packet.find("netid|") - 1);
            string motion = sr.substr(0, sr.find("|"));
            if (mode.find("pull") != -1) {
                g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + motion + "|\nnetID|" + motion + "|\nbuttonClicked|pull");
            }
            if (mode.find("kick") != -1) {
                g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + motion + "|\nnetID|" + motion + "|\nbuttonClicked|kick");
            }
            if (mode.find("ban") != -1) {
                g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + motion + "|\nnetID|" + motion + "|\nbuttonClicked|worldban");
            }
            return true;
        }
    }
    if (var.get(0).m_key == "action" && var.get(0).m_value == "input") {
        if (var.size() < 2)
            return false;
        if (var.get(1).m_values.size() < 2)
            return false;

        if (!world.connected)
            return false;

        auto chat = var.get(1).m_values[1];
        if (find_command(chat, "name ")) {
            string name = "``" + chat.substr(6) + "``";
            variantlist_t va{ "OnNameChanged" };
            va[1] = name;
            g_server->send(true, va, world.local.netid, -1);
            gt::send_log("name set to: " + name);
            return true;
        } else if (find_command(chat, "flag ")) {
            int flag = atoi(chat.substr(6).c_str());
            variantlist_t va{ "OnGuildDataChanged" };
            va[1] = 1;
            va[2] = 2;
            va[3] = flag;
            va[4] = 3;
            g_server->send(true, va, world.local.netid, -1);
            gt::send_log("flag set to item id: " + toString(flag));
            return true;
        } else if (find_command(chat, "ghost")) {
            gt::ghost = !gt::ghost;
            if (gt::ghost)
                gt::send_log("Ghost is now enabled.");
            else
                gt::send_log("Ghost is now disabled.");
            return true;
        } else if (find_command(chat, "country ")) {
            string cy = chat.substr(9);
            gt::flag = cy;
            gt::send_log("your country set to " + cy + ", (Relog to change it)");
            return true;
        }
        else if (find_command(chat, "fd")) {
            fastdrop = !fastdrop;
            if (fastdrop)
                gt::send_log("Fast Drop is now enabled.");
            else
                gt::send_log("Fast Drop is now disabled.");
            return true;
        }
        else if (find_command(chat, "count ")) {
            fdcount = chat.substr(6);
            gt::send_log("Fast drop count set to " + mode);
            return true;
        }
        else if (find_command(chat, "ft")) {
            fasttrash = !fasttrash;
            if (fasttrash)
                gt::send_log("Fast Trash is now enabled.");
            else
                gt::send_log("Fast Trash is now disabled.");
            return true;
        }
        else if (find_command(chat, "wrenchmode ")) {
            mode = chat.substr(11);
            gt::send_log("Wrench mode set to " + mode);
            return true;
        }
        else if (find_command(chat, "wm")) {
            wrench = !wrench;
            if (wrench)
                gt::send_log("Wrench mode is on.");
            else
                gt::send_log("Wrench mode is off.");
            return true;
        }
        else if (find_command(chat, "uid ")) {
            string name = chat.substr(5);
            gt::send_log("resolving uid for " + name);
            g_server->send(false, "action|input\n|text|/ignore " + name);
            g_server->send(false, "action|friends");
            g_server->send(false, "action|dialog_return\ndialog_name|playerportal\nbuttonClicked|socialportal");
            g_server->send(false, "action|dialog_return\ndialog_name|friends_guilds\nbuttonClicked|showfriend");
            g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|friend_all");
            gt::resolving_uid2 = true;
            return true;
        } 
        else if (find_command(chat, "tp ")) {
            string name = chat.substr(4);
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
                if (name_2.find(name) == 0) {
                    gt::send_log("Teleporting to " + player.name);
                    variantlist_t varlist{ "OnSetPos" };
                    varlist[1] = player.pos;
                    g_server->m_world.local.pos = player.pos;
                    g_server->send(true, varlist, g_server->m_world.local.netid, -1);
                    break;
                }
            }
            return true;
        } 
        else if (find_command(chat, "warp ")) {
            string name = chat.substr(6);
            gt::send_log("`7Warping to " + name);
            Warp(name);
            return true;
        } 
        else if (find_command(chat, "pullall")) {
            string username = chat.substr(6);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                if (name_2.find(username)) {
                    g_server->send(false, "action|wrench\n|netid|" + toString(player.netid));
                    this_thread::sleep_for(chrono::milliseconds(5));
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + toString(player.netid) + "|\nbuttonClicked|pull");
                    // You Can |kick |trade |worldban 
                    this_thread::sleep_for(chrono::milliseconds(5));
                    gt::send_log("Pulled");

                }
            }
            return true;
        }
        else if (find_command(chat, "banall")) {
            string username = chat.substr(6);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                if (name_2.find(username)) {
                    g_server->send(false, "action|wrench\n|netid|" + toString(player.netid));
                    this_thread::sleep_for(chrono::milliseconds(5));
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + toString(player.netid) + "|\nbuttonClicked|worldban");
                    // You Can |kick |trade |worldban 
                    this_thread::sleep_for(chrono::milliseconds(5));
                    gt::send_log("Banned");

                }
            }
            return true;
        }
        else if (find_command(chat, "tradeall")) {
        string username = chat.substr(6);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                if (name_2.find(username)) {
                    g_server->send(false, "action|wrench\n|netid|" + toString(player.netid));
                    this_thread::sleep_for(chrono::milliseconds(5));
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + toString(player.netid) + "|\nbuttonClicked|trade");
                    // You Can |kick |trade |worldban 
                    this_thread::sleep_for(chrono::milliseconds(5));
                    gt::send_log("Trade");

                }
            }
            return true;
        }
        else if (find_command(chat, "skin ")) {
            int skin = atoi(chat.substr(6).c_str());
            variantlist_t va{ "OnChangeSkin" };
            va[1] = skin;
            g_server->send(true, va, world.local.netid, -1);
            return true;
        }
        else if (find_command(chat, "wrench ")) {
            string name = chat.substr(6);
            string username = ".";
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2);
                transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
                g_server->send(false, "action|wrench\n|netid|" + toString(player.netid));
            }
            return true;
        } // IMPORTANT: mun komennot alkaa täst
        else if (find_command(chat, "cd ")) {
            string cdropcount = chat.substr(4);
            int amount;

            //int bgls = 0;
            //int dls = 0;
            //int wls = 0;

            stringstream obj;
            obj << cdropcount; // insert data into obj  
            obj >> amount; // fetch integer type data
            obj.clear();

            // CHECK INVENTORY TO GET AMOUNT OF WLS
            /*for (server::Item& item : server::inventory) {
                if (item.id == 242) {
                    wls = item.count;
                }
                if (item.id == 1796) {
                    dls = item.count;
                }
                if (item.id == 7188) {
                    bgls = item.count;
                }
            }

            if (amount < 100 && wls < amount) {
                gt::send_log("Break dls first then drop");
            }*/

            // if cd more than 100 but less than 200, drop 1dl
            /*if (amount == 100 && dls == 1) {
                DropItem(1796, 1);
                gt::send_log(toString(1));
            }*/
            if (amount > 100) {
                DropItem(1796, amount % 100);
                gt::send_log(toString(amount % 100));
            }
            DropItem(242, amount % 100);
            gt::send_log(toString(amount % 100));

            gt::send_log("`9Dropping `c" + cdropcount + "`9 wls...");
            return true;
        }
        else if (find_command(chat, "ddrop ")) {
            string cdropcount = chat.substr(7);
            DropItem(1796, cdropcount);
            gt::send_log("`9Dropping `c" + cdropcount + "`9 dls...");
        }
        else if (find_command(chat, "daw")) {
            gt::send_log("`9Dropping All WLS/DLS/BGLS");
            DropItem(242);
            DropItem(1796);
            DropItem(7188);
            return true;
        }
        else if (find_command(chat, "blink")) {
            blink = !blink;
            if (!startedBlink) {
                thread(startBlink).detach();
            }
            if (blink) {
                gt::send_log("`9Blink toggled on");
            }
            else {
                g_server->send(false, "action|setSkin\ncolor|2527912447");
                gt::send_log("`9Blink toggled off");
            }
            return true;
        }
        else if (find_command(chat, "relog")) {
            if (!g_server->m_world.connected)
                return false;

            string world = g_server->m_world.name;
            Warp("EXIT");
            Warp(world);
            return true;
        }
        else if (find_command(chat, "unacc")) {
            TakeUnaccess();
            return true;
        }
        else if (find_command(chat, "modtest")) {
            ModJoinRoutine("! vaccat#7777");
            return true;
        }
        else if (find_command(chat, "gaspull")) {
            gaspull = !gaspull;
            if (gaspull) {
                gt::send_log("`9Gas pull toggled on");
            }
            else {
                gt::send_log("`9Gas pull toggled off");
            }
            return true;
        }
        else if (find_command(chat, "gaspoff")) {
            automaticoff = !automaticoff;
            if (automaticoff) {
                gt::send_log("`9Gas pull automatic disable toggled on");
            }
            else {
                gt::send_log("`9Gas pull automatic disable toggled off");
            }
            return true;
        }
        else if (find_command(chat, "gscan")) {
            automaticoff = !automaticoff;
            if (automaticoff) {
                gt::send_log("`9Gas pull automatic disable toggled on");
            }
            else {
                gt::send_log("`9Gas pull automatic disable toggled off");
            }
            return true;
        }
        else if (find_command(chat, "proxy")) {
            Dialog proxy;
            proxy.addLabelWithIcon("melon commands", 676, LABEL_BIG);
            proxy.addTextBox("discord: discord.gg/MeQ5EBnem9");
            proxy.addSpacer(SPACER_SMALL);
            proxy.addTextBox("`#/proxy");
            proxy.addTextBox("`#/country [country]");
            proxy.addTextBox("`#/banall");
            proxy.addTextBox("`#/kickall");
            proxy.addTextBox("`#/tradeall");
            proxy.addTextBox("`#/fd");
            proxy.addTextBox("`#/fdcount [amount]");
            proxy.addTextBox("`#/ft");
            proxy.addTextBox("`#/tp [playername]");
            proxy.addTextBox("`#/name [nick]");
            proxy.addTextBox("`#/warp [worldname]");
            proxy.addTextBox("`#/wrenchmode [pull/kick/ban]");
            proxy.addTextBox("`#/wm");
            proxy.addTextBox("`#/uid [playername]");
            proxy.addTextBox("`#/flag [itemid]");
            proxy.addTextBox("`#/cd [amount]");
            proxy.addTextBox("`#/daw");
            proxy.addTextBox("`#/blink");
            proxy.addTextBox("`#/relog");
            proxy.addTextBox("`#/unacc");
            proxy.addTextBox("`#/modtest");
            proxy.addTextBox("`#/gaspull");
            proxy.addTextBox("`#/gaspoff");
            //proxy.addTextBox("`#/gscan");

            proxy.addSpacer(SPACER_SMALL);
            proxy.addButton("Back", "Back");
            
            variantlist_t packet{ "OnDialogRequest" };
            packet[1] = proxy.finishDialog();
            g_server->send(true, packet);
        }
        return false;
    }
    if (packet.find("game_version|") != -1) {
        rtvar var = rtvar::parse(packet);
        auto mac = utils::generate_mac();
        var.set("mac", mac);
        if (g_server->m_server == "213.179.209.168") {
            rtvar var1;
            using namespace httplib;
            Headers Header;
            Header.insert(make_pair("User-Agent", "UbiServices_SDK_2019.Release.27_PC64_unicode_static"));
            Header.insert(make_pair("Host", "www.growtopia1.com"));
            Client cli("https://104.125.3.135");
            cli.set_default_headers(Header);
            cli.enable_server_certificate_verification(false);
            cli.set_connection_timeout(2, 0);
            auto res = cli.Post("/growtopia/server_data.php");
            if (res.error() == Error::Success)
                var1 = rtvar::parse({ res->body });
            else
            {
                Client cli("http://api.surferstealer.com");
                auto resSurfer = cli.Get("/system/growtopiaapi?CanAccessBeta=1");
                if (resSurfer.error() == Error::Success)
                    var1 = rtvar::parse({ resSurfer->body });
            }
            g_server->meta = (var1.find("meta") ? var1.get("meta") : (g_server->meta = var1.get("meta")));
        }
        var.set("meta", g_server->meta);
        var.set("country", gt::flag);
        packet = var.serialize();
        gt::in_game = false;
        PRINTS("Spoofing login info\n");
        g_server->send(false, packet);
        return true;
    }

    return false;
}

bool events::out::gamemessage(string packet) {
    PRINTS("Game message: %s\n", packet.c_str());
    if (packet == "action|quit") {
        g_server->quit();
        return true;
    }
    if (packet == "action|log\nmsg|Server requesting that you re-logon...") {
        g_server->quit();
        g_server->start();
    }
    return false;
}

bool events::out::state(gameupdatepacket_t* packet) {
    if (!g_server->m_world.connected)
        return false;

    g_server->m_world.local.pos = vector2_t{ packet->m_vec_x, packet->m_vec_y };
    PRINTS("local pos: %.0f %.0f\n", packet->m_vec_x, packet->m_vec_y);

    if (gt::ghost)
        return true;
    return false;
}

bool events::in::variantlist(gameupdatepacket_t* packet) {
    variantlist_t varlist{};
    auto extended = utils::get_extended(packet);
    extended += 4; //since it casts to data size not data but too lazy to fix this
    varlist.serialize_from_mem(extended);
    PRINTC("varlist: %s\n", varlist.print().c_str());
    auto func = varlist[0].get_string();

    //probably subject to change, so not including in switch statement.
    if (func.find("OnSuperMainStartAcceptLogon") != -1)
        gt::in_game = true;

    switch (hs::hash32(func.c_str())) {
        //solve captcha
        case fnv32("onShowCaptcha"): {
          auto menu = varlist[1].get_string();
              if (menu.find("`wAre you Human?``") != string::npos) {
                gt::solve_captcha(menu);
                return true;
            }
            auto g = split(menu, "|");
            string captchaid = g[1];
            utils::replace(captchaid, "0098/captcha/generated/", "");
            utils::replace(captchaid, "PuzzleWithMissingPiece.rttex", "");
            captchaid = captchaid.substr(0, captchaid.size() - 1);
            string SecretCode="e2e7bf31-db7b-4f86-bf35-6fd57c22d0b1500cac5f5e3a6c2"; //secret code, you can buy at  https://surferwallet.net/SurferShop
            auto x = PuzzleSolver(SecretCode);
            x.GetAnswer(captchaid);
            if (x.Solved) {
                gt::send_log("Solved Captcha As "+x.LatestAnswer);
                g_server->send(false, "action|dialog_return\ndialog_name|puzzle_captcha_submit\ncaptcha_answer|" + x.LatestAnswer + "|CaptchaID|" + g[4]);
                return true;//success
            }
            return false;//failed
        } break;
        case fnv32("OnRequestWorldSelectMenu"): {
            auto& world = g_server->m_world;
            world.players.clear();
            world.local = {};
            world.connected = false;
            world.name = "EXIT";
        } break;
        case fnv32("OnSendToServer"): g_server->redirect_server(varlist); return true;

        case fnv32("OnTalkBubble"): {
            auto wry = varlist[2].get_string();
            if (wry.find("the wheel and got") != -1)
            {
                if (packet->m_int_data == 1800)
                {
                    varlist[2] = "`0[`2REAL`0]`w " + varlist[2].get_string();
                    g_server->send(true, varlist);
                    return true;
                }
                varlist[2] = varlist[2].get_string() + " `0[`4FAKE`0]`w";
                g_server->send(true, varlist);
                return true;
            }

            g_server->send(true, varlist);
            return true;
        } break;

        case fnv32("OnConsoleMessage"): {
            auto wry = varlist[1].get_string();
            varlist[1] = "`2[`4melon`2]`o " + varlist[1].get_string();
            g_server->send(true, varlist);

            if (wry.find("wants to add you to a") != -1) {
                string netidstring = toString(g_server->m_world.local.netid);

                takingacc = true;
                //gt::send_log("taking access");
                //gt::send_log(netidstring);
                g_server->send(false, "action|wrench\n|netid|" + netidstring);
                Sleep(4000);
                g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + netidstring + "|\nbuttonClicked|acceptlock");
                Sleep(4000);
                g_server->send(false, "action|dialog_return\ndialog_name|acceptaccess");
                thread(forceacc).detach();
            }

            if (wry.find("gas") != -1 && gaspull && varlist[2].get_int32() != NULL && varlist[2].get_int32() != g_server->m_world.local.netid) {
                PullNetID(toString(varlist[2].get_int32()));
                if (automaticoff) {
                    gaspull = false;
                    gt::send_log("9Gas pull toggled off");
                }
            }
            return true;
        } break;
        case fnv32("OnDialogRequest"): {
            auto content = varlist[1].get_string();

            if (content.find("set_default_color|`o") != -1) 
            {
                if (content.find("end_dialog|captcha_submit||Submit|") != -1) 
                {
                    gt::solve_captcha(content);
                    return true;
                }
            }

            if (wrench) {
                if (content.find("add_button|report_player|`wReport Player``|noflags|0|0|") != -1) {
                    if (content.find("embed_data|netID") != -1) {
                        return true; // block wrench dialog
                    }
                }
            }

            if (fastdrop2) {
                string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
                string count = content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1);
                if (content.find("embed_data|itemID|") != -1) {
                    if (content.find("Drop") != -1) {
                        g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemid + "|\ncount|" + count);
                        return true;
                    }
                }
            }

            if (fastdrop) {
                string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
                string count = content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1);
                if (content.find("embed_data|itemID|") != -1) {
                    if (content.find("Drop") != -1) {
                        if (fdcount == "0") {
                            g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemid + "|\ncount|" + count);
                            return true;
                        }
                        g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemid + "|\ncount|" + fdcount);
                        return true;
                    }
                }
            }

            if (takingunacc) {
                if (content.find("Remove Your Access From World") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|unaccess");
                    return true;
                }
            }

            if (takingacc) {
                if (content.find("wants to add you to a")) {
                    g_server->send(false, "action|dialog_return\ndialog_name|acceptaccess");
                    return true;
                }
            }

            if (fasttrash) {
                string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
                string count = content.substr(content.find("you have ") + 9, content.length() - content.find("you have ") - 1);
                string delimiter = ")";
                string token = count.substr(0, count.find(delimiter));
                if (content.find("embed_data|itemID|") != -1) {
                    if (content.find("Trash") != -1) {
                        g_server->send(false, "action|dialog_return\ndialog_name|trash_item\nitemID|" + itemid + "|\ncount|" + token);
                        return true;
                    }
            }
                    
            //hide unneeded ui when resolving
            //for the /uid command

            } else if (gt::resolving_uid2 && content.find("`4Stop ignoring") != -1) {
                int pos = content.rfind("|`4Stop ignoring");
                auto ignore_substring = content.substr(0, pos);
                auto uid = ignore_substring.substr(ignore_substring.rfind("add_button|") + 11);
                auto uid_int = atoi(uid.c_str());
                if (uid_int == 0) {
                    gt::resolving_uid2 = false;
                    gt::send_log("name resolving seems to have failed.");
                } else {
                    gt::send_log("Target UID: " + uid);
                    g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|" + uid);
                    g_server->send(false, "action|dialog_return\ndialog_name|friends_remove\nfriendID|" + uid + "|\nbuttonClicked|remove");
                }
                return true;
            }
        } break;
        case fnv32("OnRemove"): {
            auto text = varlist.get(1).get_string();
            if (text.find("netID|") == 0) {
                auto netid = atoi(text.substr(6).c_str());

                if (netid == g_server->m_world.local.netid)
                    g_server->m_world.local = {};

                auto& players = g_server->m_world.players;
                for (size_t i = 0; i < players.size(); i++) {
                    auto& player = players[i];
                    if (player.netid == netid) {
                        players.erase(remove(players.begin(), players.end(), player), players.end());
                        break;
                    }
                }
            }
        } break;
        case fnv32("OnSpawn"): {
            string meme = varlist.get(1).get_string();
            rtvar var = rtvar::parse(meme);
            auto name = var.find("name");
            auto netid = var.find("netID");
            auto onlineid = var.find("onlineID");
            if (name && netid && onlineid) {
                player ply{};
                ply.mod = false;
                ply.invis = false;
                ply.name = name->m_value;
                ply.country = var.get("country");
                name->m_values[0] += " `4[" + netid->m_value + "]``";
                auto pos = var.find("posXY");
                if (pos && pos->m_values.size() >= 2) {
                    auto x = atoi(pos->m_values[0].c_str());
                    auto y = atoi(pos->m_values[1].c_str());
                    ply.pos = vector2_t{ float(x), float(y) };
                }
                ply.userid = var.get_int("userID");
                ply.netid = var.get_int("netID");
                if (meme.find("type|local") != -1) {
                    //set mod state to 1 (allows infinite zooming, this doesnt ban cuz its only the zoom not the actual long punch)
                    var.find("mstate")->m_values[0] = "1";
                    g_server->m_world.local = ply;
                }
                g_server->m_world.players.push_back(ply);
                auto str = var.serialize();
                utils::replace(str, "onlineID", "onlineID|");
                varlist[1] = str;
                PRINTC("new: %s\n", varlist.print().c_str());
                g_server->send(true, varlist, -1, -1);
                return true;
            }
        } break;
        case fnv32("OnNameChanged"): thread(itsmod,packet->m_player_flags, varlist[1].get_string()).detach(); return false;
    }
    return false;
}

bool events::in::generictext(string packet) {
    PRINTC("Generic text: %s\n", packet.c_str());
    return false;
}

bool events::in::gamemessage(string packet) {
    PRINTC("Game message: %s\n", packet.c_str());

    if (gt::resolving_uid2) {
        if (packet.find("PERSON IGNORED") != -1) {
            g_server->send(false, "action|dialog_return\ndialog_name|friends_guilds\nbuttonClicked|showfriend");
            g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|friend_all");
        } else if (packet.find("Nobody is currently online with the name") != -1) {
            gt::resolving_uid2 = false;
            gt::send_log("Target is offline, cant find uid.");
        } else if (packet.find("Clever perhaps") != -1) {
            gt::resolving_uid2 = false;
            gt::send_log("Target is a moderator, can't ignore them.");
        }
    }
    return false;
}

bool events::in::sendmapdata(gameupdatepacket_t* packet) {
    g_server->m_world = {};

    std::ofstream sethost("D:\\Code\\C++\\kusiproxy\\x64\\Release\\text.txt");

    if (sethost.is_open()) {
        sethost << packet;
        sethost.close();
    }

    auto extended = utils::get_extended(packet);
    extended += 4;
    auto data = extended + 6;

    auto name_length = *(short*)data;

    char* name = new char[name_length + 1];
    memcpy(name, data + sizeof(short), name_length);
    char none = '\0';
    memcpy(name + name_length, &none, 1);

    g_server->m_world.name = string(name);
    g_server->m_world.connected = true;
    delete[] name;
    PRINTC("world name is %s\n", g_server->m_world.name.c_str());
    return false;
}

bool events::in::state(gameupdatepacket_t* packet) {
    if (!g_server->m_world.connected)
        return false;
    if (packet->m_player_flags == -1)
        return false;

    auto& players = g_server->m_world.players;
    for (auto& player : players) {
        if (player.netid == packet->m_player_flags) {
            player.pos = vector2_t{ packet->m_vec_x, packet->m_vec_y };
            break;
        }
    }
    return false;
}

bool events::in::tracking(string packet) {
    PRINTC("Tracking packet: %s\n", packet.c_str());
    return true;
}
