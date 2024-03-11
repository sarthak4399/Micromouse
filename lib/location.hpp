#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <vector>

class Location {
private:
    std::vector<bool> walls;
    std::vector<int> position;
    bool visited;

public:
    Location(std::vector<int> pos = {-1, -1})
        : position(pos), visited(false) {
        walls = {false, false, false, false};
    }
    std::vector<int> get_position() const { return position; }
    void set_position(std::vector<int> pos) {
        position = pos;
    }

    void set_walls(std::vector<bool> _walls) {
        walls = _walls;
    }

    void set_visited(bool vis) {
        visited = vis;
    }

    bool can_move_to(Location loc) {
        return ((loc.position[0] == position[0] && loc.position[1] - position[1] == 1 && !walls[0]) ||
                (loc.position[1] == position[1] && loc.position[0] - position[0] == 1 && !walls[1]) ||
                (loc.position[0] == position[0] && loc.position[1] - position[1] == -1 && !walls[2]) ||
                (loc.position[1] == position[1] && loc.position[0] - position[0] == -1 && !walls[3]));
    }

};

#endif // LOCATION_HPP
