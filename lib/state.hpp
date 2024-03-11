#ifndef STATE_HPP
#define STATE_HPP

#include "Location.hpp" // Include the Location header file if needed

class State {
private:
    Location location;
    State* parent;
    int action;
    int cur_dir;

public:
    // Constructor
    State(Location loc, State* parent = nullptr, int action = -1, int cur_dir = 0)
        : location(loc), parent(parent), action(action), cur_dir(cur_dir) {}

    // Member functions
    void set_loc(Location loc) { location = loc; }
    void set_par(State* par) { parent = par; }
    void set_act(int act) { action = act; }
    void set_cur_dir(int cur_dir) { this->cur_dir = cur_dir; }
    bool is_goal() const {
        int x = location.get_position()[0];
        int y = location.get_position()[1];
        return (x == 7 && (y == 7 || y == 8)) || (x == 8 && (y == 7 || y == 8));
    }
};

#endif /* STATE_HPP */
