#pragma once
#include<boost/json.hpp>
#include<string>
#include<optional>
#include<map>
namespace statetypes {
    namespace json = boost::json;
    struct GameState {
        int64_t id;
        int64_t day;
        int64_t playerTurn;
        std::vector<int64_t> playerOrder;
        std::string variant;
        bool active;

        void readFrom(json::object const& obj) {
            if (auto ptr = obj.if_contains("id")) {
                if (auto tPtr = ptr->if_int64()) {
                    id = *tPtr;
                }
                else {
                    throw std::exception("Expected 'id' as int64_t in GameState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'id' was missing in GameState");
            }
            if (auto ptr = obj.if_contains("day")) {
                if (auto tPtr = ptr->if_int64()) {
                    day = *tPtr;
                }
                else {
                    throw std::exception("Expected 'day' as int64_t in GameState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'day' was missing in GameState");
            }
            if (auto ptr = obj.if_contains("playerTurn")) {
                if (auto tPtr = ptr->if_int64()) {
                    playerTurn = *tPtr;
                }
                else {
                    throw std::exception("Expected 'playerTurn' as int64_t in GameState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'playerTurn' was missing in GameState");
            }
            if (auto ptr = obj.if_contains("playerOrder")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<int64_t> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_int64()) {
                            arr.push_back(int64_t{ *vPtr });
                        }
                        else {
                            throw std::exception("Item in array 'playerOrder' in GameState was expected to be int64, but was of wrong type.");
                        }
                    }
                    playerOrder = std::move(arr);
                }
                else {
                    throw std::exception("Expected 'playerOrder' as array in GameState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'playerOrder' was missing in GameState");
            }
            if (auto ptr = obj.if_contains("variant")) {
                if (auto tPtr = ptr->if_string()) {
                    variant = *tPtr;
                }
                else {
                    throw std::exception("Expected 'variant' as std::string in GameState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'variant' was missing in GameState");
            }
            if (auto ptr = obj.if_contains("active")) {
                if (auto tPtr = ptr->if_bool()) {
                    active = *tPtr;
                }
                else {
                    throw std::exception("Expected 'active' as bool in GameState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'active' was missing in GameState");
            }
        }

        void writeTo(json::object& obj) const {
            obj["id"] = id;
            obj["day"] = day;
            obj["playerTurn"] = playerTurn;
            {
                json::array arr;
                for (auto const& val : playerOrder) {
                    arr.push_back(int64_t{ val });
                }
                obj["playerOrder"] = std::move(arr);
            }
            obj["variant"] = variant;
            obj["active"] = active;
        }
    };
    struct PlayerState {
        int64_t id;
        int64_t owner;
        std::string commanderName;
        int64_t funds;
        int64_t powerCharge;
        std::string armyColor;
        bool alive;
        int64_t totalPowerUses;
        std::optional<std::string> team;
        std::optional<std::string> powerActive;
        std::optional<int64_t> powerActiveDay;
        std::optional<std::vector<std::string>> bannedUnits;
        std::optional<int64_t> incomeMultiplier;
        std::optional<int64_t> coMeterMultiplier;

        void readFrom(json::object const& obj) {
            if (auto ptr = obj.if_contains("id")) {
                if (auto tPtr = ptr->if_int64()) {
                    id = *tPtr;
                }
                else {
                    throw std::exception("Expected 'id' as int64_t in PlayerState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'id' was missing in PlayerState");
            }
            if (auto ptr = obj.if_contains("owner")) {
                if (auto tPtr = ptr->if_int64()) {
                    owner = *tPtr;
                }
                else {
                    throw std::exception("Expected 'owner' as int64_t in PlayerState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'owner' was missing in PlayerState");
            }
            if (auto ptr = obj.if_contains("commanderName")) {
                if (auto tPtr = ptr->if_string()) {
                    commanderName = *tPtr;
                }
                else {
                    throw std::exception("Expected 'commanderName' as std::string in PlayerState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'commanderName' was missing in PlayerState");
            }
            if (auto ptr = obj.if_contains("funds")) {
                if (auto tPtr = ptr->if_int64()) {
                    funds = *tPtr;
                }
                else {
                    throw std::exception("Expected 'funds' as int64_t in PlayerState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'funds' was missing in PlayerState");
            }
            if (auto ptr = obj.if_contains("powerCharge")) {
                if (auto tPtr = ptr->if_int64()) {
                    powerCharge = *tPtr;
                }
                else {
                    throw std::exception("Expected 'powerCharge' as int64_t in PlayerState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'powerCharge' was missing in PlayerState");
            }
            if (auto ptr = obj.if_contains("armyColor")) {
                if (auto tPtr = ptr->if_string()) {
                    armyColor = *tPtr;
                }
                else {
                    throw std::exception("Expected 'armyColor' as std::string in PlayerState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'armyColor' was missing in PlayerState");
            }
            if (auto ptr = obj.if_contains("alive")) {
                if (auto tPtr = ptr->if_bool()) {
                    alive = *tPtr;
                }
                else {
                    throw std::exception("Expected 'alive' as bool in PlayerState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'alive' was missing in PlayerState");
            }
            if (auto ptr = obj.if_contains("totalPowerUses")) {
                if (auto tPtr = ptr->if_int64()) {
                    totalPowerUses = *tPtr;
                }
                else {
                    throw std::exception("Expected 'totalPowerUses' as int64_t in PlayerState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'totalPowerUses' was missing in PlayerState");
            }
            if (auto ptr = obj.if_contains("team")) {
                if (auto tPtr = ptr->if_string()) {
                    team = *tPtr;
                }
                else {
                    throw std::exception("Expected 'team' as std::string in PlayerState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("powerActive")) {
                if (auto tPtr = ptr->if_string()) {
                    powerActive = *tPtr;
                }
                else {
                    throw std::exception("Expected 'powerActive' as std::string in PlayerState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("powerActiveDay")) {
                if (auto tPtr = ptr->if_int64()) {
                    powerActiveDay = *tPtr;
                }
                else {
                    throw std::exception("Expected 'powerActiveDay' as int64_t in PlayerState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("bannedUnits")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<std::string> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_string()) {
                            arr.push_back(std::string{ *vPtr });
                        }
                        else {
                            throw std::exception("Item in array 'bannedUnits' in PlayerState was expected to be string, but was of wrong type.");
                        }
                    }
                    bannedUnits = std::move(arr);
                }
                else {
                    throw std::exception("Expected 'bannedUnits' as array in PlayerState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("incomeMultiplier")) {
                if (auto tPtr = ptr->if_int64()) {
                    incomeMultiplier = *tPtr;
                }
                else {
                    throw std::exception("Expected 'incomeMultiplier' as int64_t in PlayerState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("coMeterMultiplier")) {
                if (auto tPtr = ptr->if_int64()) {
                    coMeterMultiplier = *tPtr;
                }
                else {
                    throw std::exception("Expected 'coMeterMultiplier' as int64_t in PlayerState, but was of wrong type.");
                }
            }
        }

        void writeTo(json::object& obj) const {
            obj["id"] = id;
            obj["owner"] = owner;
            obj["commanderName"] = commanderName;
            obj["funds"] = funds;
            obj["powerCharge"] = powerCharge;
            obj["armyColor"] = armyColor;
            obj["alive"] = alive;
            obj["totalPowerUses"] = totalPowerUses;
            if (team) {
                obj["team"] = *team;
            }
            if (powerActive) {
                obj["powerActive"] = *powerActive;
            }
            if (powerActiveDay) {
                obj["powerActiveDay"] = *powerActiveDay;
            }
            if (bannedUnits) {
                json::array arr;
                for (auto const& val : *bannedUnits) {
                    arr.push_back(json::string{ val });
                }
                obj["bannedUnits"] = std::move(arr);
            }
            if (incomeMultiplier) {
                obj["incomeMultiplier"] = *incomeMultiplier;
            }
            if (coMeterMultiplier) {
                obj["coMeterMultiplier"] = *coMeterMultiplier;
            }
        }
    };
    struct SettingsState {
        int64_t id;
        int64_t startingFunds;
        int64_t incomeMultiplier;
        bool fogOfWar;
        std::map<std::string, int64_t> variant;
        bool coPowers;
        bool teams;
        int64_t modId;
        int64_t coMeterSize;
        int64_t coMeterMultiplier;
        std::optional<int64_t> unitLimit;
        std::optional<int64_t> captureLimit;
        std::optional<int64_t> dayLimit;

        void readFrom(json::object const& obj) {
            if (auto ptr = obj.if_contains("id")) {
                if (auto tPtr = ptr->if_int64()) {
                    id = *tPtr;
                }
                else {
                    throw std::exception("Expected 'id' as int64_t in SettingsState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'id' was missing in SettingsState");
            }
            if (auto ptr = obj.if_contains("startingFunds")) {
                if (auto tPtr = ptr->if_int64()) {
                    startingFunds = *tPtr;
                }
                else {
                    throw std::exception("Expected 'startingFunds' as int64_t in SettingsState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'startingFunds' was missing in SettingsState");
            }
            if (auto ptr = obj.if_contains("incomeMultiplier")) {
                if (auto tPtr = ptr->if_int64()) {
                    incomeMultiplier = *tPtr;
                }
                else {
                    throw std::exception("Expected 'incomeMultiplier' as int64_t in SettingsState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'incomeMultiplier' was missing in SettingsState");
            }
            if (auto ptr = obj.if_contains("fogOfWar")) {
                if (auto tPtr = ptr->if_bool()) {
                    fogOfWar = *tPtr;
                }
                else {
                    throw std::exception("Expected 'fogOfWar' as bool in SettingsState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'fogOfWar' was missing in SettingsState");
            }
            if (auto ptr = obj.if_contains("variant")) {
                if (auto tPtr = ptr->if_object()) {
                    std::map<std::string, int64_t> map;
                    for (auto const& entry : *tPtr) {
                        if (auto mPtr = entry.value().if_int64()) {
                            map[entry.key_c_str()] = int64_t{ *mPtr };
                        }
                        else {
                            throw std::exception("Entry in map 'variant' in SettingsState was expected to be int64, but was of wrong type.");
                        }
                    }
                    variant = std::move(map);
                }
                else {
                    throw std::exception("Expected 'variant' as object in SettingsState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'variant' was missing in SettingsState");
            }
            if (auto ptr = obj.if_contains("coPowers")) {
                if (auto tPtr = ptr->if_bool()) {
                    coPowers = *tPtr;
                }
                else {
                    throw std::exception("Expected 'coPowers' as bool in SettingsState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'coPowers' was missing in SettingsState");
            }
            if (auto ptr = obj.if_contains("teams")) {
                if (auto tPtr = ptr->if_bool()) {
                    teams = *tPtr;
                }
                else {
                    throw std::exception("Expected 'teams' as bool in SettingsState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'teams' was missing in SettingsState");
            }
            if (auto ptr = obj.if_contains("modId")) {
                if (auto tPtr = ptr->if_int64()) {
                    modId = *tPtr;
                }
                else {
                    throw std::exception("Expected 'modId' as int64_t in SettingsState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'modId' was missing in SettingsState");
            }
            if (auto ptr = obj.if_contains("coMeterSize")) {
                if (auto tPtr = ptr->if_int64()) {
                    coMeterSize = *tPtr;
                }
                else {
                    throw std::exception("Expected 'coMeterSize' as int64_t in SettingsState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'coMeterSize' was missing in SettingsState");
            }
            if (auto ptr = obj.if_contains("coMeterMultiplier")) {
                if (auto tPtr = ptr->if_int64()) {
                    coMeterMultiplier = *tPtr;
                }
                else {
                    throw std::exception("Expected 'coMeterMultiplier' as int64_t in SettingsState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'coMeterMultiplier' was missing in SettingsState");
            }
            if (auto ptr = obj.if_contains("unitLimit")) {
                if (auto tPtr = ptr->if_int64()) {
                    unitLimit = *tPtr;
                }
                else {
                    throw std::exception("Expected 'unitLimit' as int64_t in SettingsState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("captureLimit")) {
                if (auto tPtr = ptr->if_int64()) {
                    captureLimit = *tPtr;
                }
                else {
                    throw std::exception("Expected 'captureLimit' as int64_t in SettingsState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("dayLimit")) {
                if (auto tPtr = ptr->if_int64()) {
                    dayLimit = *tPtr;
                }
                else {
                    throw std::exception("Expected 'dayLimit' as int64_t in SettingsState, but was of wrong type.");
                }
            }
        }

        void writeTo(json::object& obj) const {
            obj["id"] = id;
            obj["startingFunds"] = startingFunds;
            obj["incomeMultiplier"] = incomeMultiplier;
            obj["fogOfWar"] = fogOfWar;
            {
                json::object m;
                for (auto const& entry : variant) {
                    m[entry.first] = entry.second;
                }
                obj["variant"] = std::move(m);
            }
            obj["coPowers"] = coPowers;
            obj["teams"] = teams;
            obj["modId"] = modId;
            obj["coMeterSize"] = coMeterSize;
            obj["coMeterMultiplier"] = coMeterMultiplier;
            if (unitLimit) {
                obj["unitLimit"] = *unitLimit;
            }
            if (captureLimit) {
                obj["captureLimit"] = *captureLimit;
            }
            if (dayLimit) {
                obj["dayLimit"] = *dayLimit;
            }
        }
    };
    struct TerrainState {
        int64_t id;
        int64_t x;
        int64_t y;
        std::string name;
        std::optional<int64_t> orientation;
        std::optional<int64_t> capturePoints;
        std::optional<std::string> owner;
        std::optional<int64_t> activationCount;
        std::optional<int64_t> hitPoints;

        void readFrom(json::object const& obj) {
            if (auto ptr = obj.if_contains("id")) {
                if (auto tPtr = ptr->if_int64()) {
                    id = *tPtr;
                }
                else {
                    throw std::exception("Expected 'id' as int64_t in TerrainState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'id' was missing in TerrainState");
            }
            if (auto ptr = obj.if_contains("x")) {
                if (auto tPtr = ptr->if_int64()) {
                    x = *tPtr;
                }
                else {
                    throw std::exception("Expected 'x' as int64_t in TerrainState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'x' was missing in TerrainState");
            }
            if (auto ptr = obj.if_contains("y")) {
                if (auto tPtr = ptr->if_int64()) {
                    y = *tPtr;
                }
                else {
                    throw std::exception("Expected 'y' as int64_t in TerrainState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'y' was missing in TerrainState");
            }
            if (auto ptr = obj.if_contains("name")) {
                if (auto tPtr = ptr->if_string()) {
                    name = *tPtr;
                }
                else {
                    throw std::exception("Expected 'name' as std::string in TerrainState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'name' was missing in TerrainState");
            }
            if (auto ptr = obj.if_contains("orientation")) {
                if (auto tPtr = ptr->if_int64()) {
                    orientation = *tPtr;
                }
                else {
                    throw std::exception("Expected 'orientation' as int64_t in TerrainState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("capturePoints")) {
                if (auto tPtr = ptr->if_int64()) {
                    capturePoints = *tPtr;
                }
                else {
                    throw std::exception("Expected 'capturePoints' as int64_t in TerrainState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("owner")) {
                if (auto tPtr = ptr->if_string()) {
                    owner = *tPtr;
                }
                else {
                    throw std::exception("Expected 'owner' as std::string in TerrainState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("activationCount")) {
                if (auto tPtr = ptr->if_int64()) {
                    activationCount = *tPtr;
                }
                else {
                    throw std::exception("Expected 'activationCount' as int64_t in TerrainState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("hitPoints")) {
                if (auto tPtr = ptr->if_int64()) {
                    hitPoints = *tPtr;
                }
                else {
                    throw std::exception("Expected 'hitPoints' as int64_t in TerrainState, but was of wrong type.");
                }
            }
        }

        void writeTo(json::object& obj) const {
            obj["id"] = id;
            obj["x"] = x;
            obj["y"] = y;
            obj["name"] = name;
            if (orientation) {
                obj["orientation"] = *orientation;
            }
            if (capturePoints) {
                obj["capturePoints"] = *capturePoints;
            }
            if (owner) {
                obj["owner"] = *owner;
            }
            if (activationCount) {
                obj["activationCount"] = *activationCount;
            }
            if (hitPoints) {
                obj["hitPoints"] = *hitPoints;
            }
        }
    };
    struct UnitState {
        int64_t id;
        int64_t x;
        int64_t y;
        std::string name;
        int64_t ammo;
        int64_t fuel;
        bool active;
        std::optional<int64_t> stunned;
        std::optional<std::vector<int64_t>> transporting;
        std::optional<int64_t> owner;
        std::optional<int64_t> hitPoints;
        std::optional<int64_t> realHitPoints;
        std::optional<bool> stealthed;

        void readFrom(json::object const& obj) {
            if (auto ptr = obj.if_contains("id")) {
                if (auto tPtr = ptr->if_int64()) {
                    id = *tPtr;
                }
                else {
                    throw std::exception("Expected 'id' as int64_t in UnitState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'id' was missing in UnitState");
            }
            if (auto ptr = obj.if_contains("x")) {
                if (auto tPtr = ptr->if_int64()) {
                    x = *tPtr;
                }
                else {
                    throw std::exception("Expected 'x' as int64_t in UnitState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'x' was missing in UnitState");
            }
            if (auto ptr = obj.if_contains("y")) {
                if (auto tPtr = ptr->if_int64()) {
                    y = *tPtr;
                }
                else {
                    throw std::exception("Expected 'y' as int64_t in UnitState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'y' was missing in UnitState");
            }
            if (auto ptr = obj.if_contains("name")) {
                if (auto tPtr = ptr->if_string()) {
                    name = *tPtr;
                }
                else {
                    throw std::exception("Expected 'name' as std::string in UnitState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'name' was missing in UnitState");
            }
            if (auto ptr = obj.if_contains("ammo")) {
                if (auto tPtr = ptr->if_int64()) {
                    ammo = *tPtr;
                }
                else {
                    throw std::exception("Expected 'ammo' as int64_t in UnitState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'ammo' was missing in UnitState");
            }
            if (auto ptr = obj.if_contains("fuel")) {
                if (auto tPtr = ptr->if_int64()) {
                    fuel = *tPtr;
                }
                else {
                    throw std::exception("Expected 'fuel' as int64_t in UnitState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'fuel' was missing in UnitState");
            }
            if (auto ptr = obj.if_contains("active")) {
                if (auto tPtr = ptr->if_bool()) {
                    active = *tPtr;
                }
                else {
                    throw std::exception("Expected 'active' as bool in UnitState, but was of wrong type.");
                }
            }
            else {
                throw std::exception("'active' was missing in UnitState");
            }
            if (auto ptr = obj.if_contains("stunned")) {
                if (auto tPtr = ptr->if_int64()) {
                    stunned = *tPtr;
                }
                else {
                    throw std::exception("Expected 'stunned' as int64_t in UnitState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("transporting")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<int64_t> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_int64()) {
                            arr.push_back(int64_t{ *vPtr });
                        }
                        else {
                            throw std::exception("Item in array 'transporting' in UnitState was expected to be int64, but was of wrong type.");
                        }
                    }
                    transporting = std::move(arr);
                }
                else {
                    throw std::exception("Expected 'transporting' as array in UnitState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("owner")) {
                if (auto tPtr = ptr->if_int64()) {
                    owner = *tPtr;
                }
                else {
                    throw std::exception("Expected 'owner' as int64_t in UnitState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("hitPoints")) {
                if (auto tPtr = ptr->if_int64()) {
                    hitPoints = *tPtr;
                }
                else {
                    throw std::exception("Expected 'hitPoints' as int64_t in UnitState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("realHitPoints")) {
                if (auto tPtr = ptr->if_int64()) {
                    realHitPoints = *tPtr;
                }
                else {
                    throw std::exception("Expected 'realHitPoints' as int64_t in UnitState, but was of wrong type.");
                }
            }
            if (auto ptr = obj.if_contains("stealthed")) {
                if (auto tPtr = ptr->if_bool()) {
                    stealthed = *tPtr;
                }
                else {
                    throw std::exception("Expected 'stealthed' as bool in UnitState, but was of wrong type.");
                }
            }
        }

        void writeTo(json::object& obj) const {
            obj["id"] = id;
            obj["x"] = x;
            obj["y"] = y;
            obj["name"] = name;
            obj["ammo"] = ammo;
            obj["fuel"] = fuel;
            obj["active"] = active;
            if (stunned) {
                obj["stunned"] = *stunned;
            }
            if (transporting) {
                json::array arr;
                for (auto const& val : *transporting) {
                    arr.push_back(int64_t{ val });
                }
                obj["transporting"] = std::move(arr);
            }
            if (owner) {
                obj["owner"] = *owner;
            }
            if (hitPoints) {
                obj["hitPoints"] = *hitPoints;
            }
            if (realHitPoints) {
                obj["realHitPoints"] = *realHitPoints;
            }
            if (stealthed) {
                obj["stealthed"] = *stealthed;
            }
        }
    };

}