#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

namespace { //=================================================================

namespace _1 {

double max(const std::vector<double>& numbers)
{
    assert(!numbers.empty());
    auto result = std::max_element(numbers.cbegin(), numbers.cend());
    return *result;
}

void test()
{
    auto sum = max({1}) + max({1, 2}) + max({1, 2, 3});
    std::cout << "sum: " << sum << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

enum direction_t {
    East,
    South,
    West,
    North
};

struct position_t {
    int x;
    int y;

    position_t(const position_t& pos, direction_t dir)
    {
        switch (dir) {
        case East:
            x = pos.x + 1;
            y = pos.y;
            break;
        case South:
            x = pos.x;
            y = pos.y + 1;
            break;
        case West:
            x = pos.x - 1;
            y = pos.y;
            break;
        case North:
            x = pos.x;
            y = pos.y - 1;
            break;
        }
    }
};


struct maze_t {
    bool wall[16][16];

    bool isWall(const position_t& pos) const {
        return isWall(pos.x, pos.y);
    }
    bool isWall(int x, int y) const {
        return wall[y][x];
    }
    bool isExit(const position_t& pos) const {
        return pos.x == 15 && pos.y == 15;
    }
};

void draw_maze(const maze_t& maze)
{
    //draw
}
void draw_player(const position_t& current, direction_t dir)
{
}

position_t next_position(direction_t dir, const position_t& prev, const maze_t& maze)
{
    const position_t desired = position_t(prev, dir);

    return maze.isWall(desired) ? prev : desired;
}

direction_t get_dir()
{
    return North;
}

void process_events(const maze_t& maze, const position_t& current)
{
    if (maze.isExit(current)) {
        //display message & exit
        return;
    }

    direction_t dir = get_dir();

    draw_maze(maze);
    draw_player(current, dir);

    const auto new_position = next_position(dir, current, maze);

    process_events(maze, new_position);
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_ch_05()
{
    _1::test();
}
