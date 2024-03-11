#include <Arduino.h>
#include <Queue.h> 
#include "API.h"   
#include "Location.h" 
#include "state.h"  

const int MAZE_WIDTH = 16;
const int MAZE_HEIGHT = 16;

int cur_direction = 0;
int cur_position[2] = {0, 0};

Location maze[MAZE_HEIGHT][MAZE_WIDTH];

Queue<Location> loc_stack;
Queue<int> dir_stack;
Queue<int> act_stack;
Queue<State> frontier;

void update_position(int move_direction = 1) {
    if (cur_direction == 0)
        cur_position[1] += move_direction;
    else if (cur_direction == 1)
        cur_position[0] += move_direction;
    else if (cur_direction == 2)
        cur_position[1] -= move_direction;
    else if (cur_direction == 3)
        cur_position[0] -= move_direction;
}

void update_direction(int turn_direction) {
    cur_direction = (cur_direction + turn_direction) % 4;
}

std::vector<bool> get_walls() {
    std::vector<bool> walls(4, false);
    walls[cur_direction] = wallFront();
    walls[(cur_direction + 1) % 4] = wallRight();
    walls[(cur_direction + 2) % 4] = false;
    walls[(cur_direction + 3) % 4] = wallLeft();
    if (cur_position[0] == 0 && cur_position[1] == 0)
        walls[2] = true;
    return walls;
}

void mark_visited_api(int* pos = n ullptr) {
    if (pos == nullptr)
        pos = cur_position;
    setColor(pos[0], pos[1], "G");
    setText(pos[0], pos[1], "hit");
}

void mark_solution_api(int* pos = nullptr) {
    if (pos == nullptr)
        pos = cur_position;
    setColor(pos[0], pos[1], "B");
    setText(pos[0], pos[1], "Sol");
}

void mark_bfs_api(int* pos = nullptr) {
    if (pos == nullptr)
        pos = cur_position;
    setColor(pos[0], pos[1], "c");
    setText(pos[0], pos[1], "dfs");
}

void mark_bktrk_api(int* pos = nullptr) {
    if (pos == nullptr)
        pos = cur_position;
    setColor(pos[0], pos[1], "o");
    setText(pos[0], pos[1], "bktrk");
}

void log(const char* string) {
    Serial.println(string);
}

void move_forward() {
    API.moveForward();
    update_position(+1);
}

void turn_left() {
    API.turnLeft();
    update_direction(-1);
}

void turn_right() {
    API.turnRight();
    update_direction(+1);
}

void dfs_map_maze() {
    Location cur_loc = maze[cur_position[0]][cur_position[1]];

    if (!cur_loc.visited) {
        cur_loc.set_visited(true);
        cur_loc.set_walls(get_walls());
        mark_visited_api(cur_position);

        if (!cur_loc.walls[0] && !maze[cur_position[0]][cur_position[1] + 1].visited)
            loc_stack.enqueue(maze[cur_position[0]][cur_position[1] + 1]);

        if (!cur_loc.walls[1] && !maze[cur_position[0] + 1][cur_position[1]].visited)
            loc_stack.enqueue(maze[cur_position[0] + 1][cur_position[1]]);

        if (!cur_loc.walls[2] && !maze[cur_position[0]][cur_position[1] - 1].visited)
            loc_stack.enqueue(maze[cur_position[0]][cur_position[1] - 1]);

        if (!cur_loc.walls[3] && !maze[cur_position[0] - 1][cur_position[1]].visited)
            loc_stack.enqueue(maze[cur_position[0] - 1][cur_position[1]]);
    }

    while (true) {
        if (loc_stack.empty()) {
            if (!(cur_position[0] == 0 && cur_position[1] == 0)) {
                turn_right();
                move_forward();
                dfs_map_maze();
            }
            return;
        }
        Location next_loc = loc_stack.dequeue();
        if (!next_loc.visited)
            break;
    }

    if (cur_loc.can_move_to(next_loc)) {
        turn_toward(next_loc);
        dir_stack.enqueue(cur_direction);
        move_forward();
    } else {
        loc_stack.enqueue(next_loc);
        turn_right();
        move_forward();
    }
    dfs_map_maze();
}

void find_bfs_shortest_path() {
    for (int i = 0; i < MAZE_HEIGHT; ++i) {
        for (int j = 0; j < MAZE_WIDTH; ++j) {
            maze[i][j].visited = false;
        }
    }
    State first_state = State(&maze[0][0]);
    frontier.enqueue(first_state);
    while (!frontier.empty()) {
        State next_state = frontier.dequeue();
        maze[next_state.location->position[0]][next_state.location->position[1]].set_visited(true);
        mark_bfs_api(next_state.location->position);
        if (next_state.is_goal())
            return;
        Location* my_loc = next_state.location;
        Location* north_loc = nullptr;
        Location* east_loc = nullptr;
        Location* south_loc = nullptr;
        Location* west_loc = nullptr;
        if (!my_loc->walls[0])
            north_loc = &maze[my_loc->position[0]][my_loc->position[1] + 1];
        if (!my_loc->walls[1])
            east_loc = &maze[my_loc->position[0] + 1][my_loc->position[1]];
        if (!my_loc->walls[2])
            south_loc = &maze[my_loc->position[0]][my_loc->position[1] - 1];
        if (!my_loc->walls[3])
            west_loc = &maze[my_loc->position[0] - 1][my_loc->position[1]];
        if (north_loc && my_loc->can_move_to(north_loc) && !north_loc->visited)
            frontier.enqueue(State(north_loc, &next_state, (0 - next_state.cur_dir) % 4, 0));
        if (east_loc && my_loc->can_move_to(east_loc) && !east_loc->visited)
            frontier.enqueue(State(east_loc, &next_state, (1 - next_state.cur_dir) % 4, 1));
        if (south_loc && my_loc->can_move_to(south_loc) && !south_loc->visited)
            frontier.enqueue(State(south_loc, &next_state, (2 - next_state.cur_dir) % 4, 2));
        if (west_loc && my_loc->can_move_to(west_loc) && !west_loc->visited)
            frontier.enqueue(State(west_loc, &next_state, (3 - next_state.cur_dir) % 4, 3));
    }
}

void execute_shortest_path(State sol) {
    while (sol.parent != &sol) {
        act_stack.enqueue(sol.action);
        mark_bktrk_api(sol.location->position);
        sol = *sol.parent;
    }
    while (!act_stack.empty()) {
        int act = act_stack.dequeue();
        mark_solution_api();
        if (act == 1)
            turn_right();
        else if (act == 3)
            turn_left();
        move_forward();
    }
}

void setup() {
    Serial.begin(9600); // Initialize serial communication
    API.begin();        // Initialize API
    log("Running...");
    dfs_map_maze();
    cur_direction = 0;
    find_bfs_shortest_path();
    execute_shortest_path(frontier.peek());
    log("Done!");
}

void loop() {}
