#pragma once

#include <map>

// everything we do in here is guarded by scope. this is used to put misc data
// about the game
namespace BattleShip {

using std::map;

// enum for different kinds of ships allowed
enum class eShipStatus {
    Carrier,
    Battleship,
    Cruiser,
    Submarine,
    Destroyer,
    Sea,
    HIT,
    MISS
};

// map to get ship size based on type
static map<eShipStatus, int> shipSize{ { eShipStatus::Carrier, 5 },
    { eShipStatus::Battleship, 4 },
    { eShipStatus::Cruiser, 3 },
    { eShipStatus::Submarine, 3 },
    { eShipStatus::Destroyer, 2 } };

// display character
static map<eShipStatus, char> shipDisplayIcon{ { eShipStatus::Carrier, 'C' },
    { eShipStatus::Battleship, 'B' },
    { eShipStatus::Cruiser, 'c' },
    { eShipStatus::Submarine, 'S' },
    { eShipStatus::Destroyer, 'D' },
    { eShipStatus::Sea, '~' },
    { eShipStatus::HIT, 'X' },
    { eShipStatus::MISS, '*' }

};

// could be bool but how to interpret true or false? call it is up?
enum class eOrientation { VERTICAL,
    HORIZONTAL };

// ideas for adding 'range' item for looping along column rows
struct Position {
    int x{ 0 };
    int y{ 0 };

    Position operator+(const Position& positionToAdd)
    {
        return Position{ x + positionToAdd.x,
            y + positionToAdd.y };
    }

    Position& operator+=(const Position& positionToAdd)
    {
        x += positionToAdd.x;
        y += positionToAdd.y;

        return *this;
    }

    bool operator!=(const Position& positionToCompare)
    {
        return (x != positionToCompare.x) || (y != positionToCompare.y);
    }
};

struct GridRange {
    GridRange() = default;
    GridRange(int initialOffset, int secondaryOffset, int shipLength,
        eOrientation orientation)
        : begin{ initialOffset, secondaryOffset }
        , end{ initialOffset, secondaryOffset + shipLength }
        , direction{ 0, 1 }
    {
        if (orientation == eOrientation::HORIZONTAL) {
            std::swap(begin.x, begin.y);
            std::swap(end.x, end.y);
            std::swap(direction.x, direction.y);
        }
    }

    // a 'typedef' for the direction the range should travel through VERT/HOR.
    // compatible with Position adding
    using Direction = Position;

    Position Dir() const
    {
        return direction;
    }

    Position Begin()
    {
        current = begin;
        return begin;
    }

    Position Start() const
    {
        return begin;
    }

    Position End() const
    {
        return end;
    }

    void Bump()
    {
        current += direction;
    }

    Position Current() const
    {
        return current;
    }

    Position begin{ 0, 0 };
    Position end{ 0, 0 };
    Position current{ begin };
    Direction direction{ 0, 0 };
};
} // namespace BattleShip