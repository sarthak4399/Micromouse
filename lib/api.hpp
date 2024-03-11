#include <iostream>
#include <string>

class MouseCrashedError : public std::exception {
public:
    const char* what() const noexcept override {
        return "Mouse crashed";
    }
};

std::string command(std::initializer_list<std::string> args, const std::type_info* return_type = nullptr) {
    std::string line;
    for (const auto& arg : args) {
        line += arg + " ";
    }
    line += "\n";
    std::cout << line;
    std::cout.flush();

    if (return_type) {
        std::string response;
        std::getline(std::cin, response);
        if (*return_type == typeid(bool)) {
            return (response == "true") ? "true" : "false";
        }
        return response;
    }

    return "";
}

int mazeWidth() {
    return std::stoi(command({"mazeWidth"}, &typeid(int)));
}

int mazeHeight() {
    return std::stoi(command({"mazeHeight"}, &typeid(int)));
}

bool wallFront() {
    return command({"wallFront"}, &typeid(bool)) == "true";
}

bool wallRight() {
    return command({"wallRight"}, &typeid(bool)) == "true";
}

bool wallLeft() {
    return command({"wallLeft"}, &typeid(bool)) == "true";
}

void moveForward() {
    std::string response = command({"moveForward"});
    if (response == "crash") {
        throw MouseCrashedError();
    }
}

void turnRight() {
    command({"turnRight"});
}

void turnLeft() {
    command({"turnLeft"});
}

void setWall(int x, int y, const std::string& direction) {
    command({"setWall", std::to_string(x), std::to_string(y), direction});
}

void clearWall(int x, int y, const std::string& direction) {
    command({"clearWall", std::to_string(x), std::to_string(y), direction});
}

void setColor(int x, int y, const std::string& color) {
    command({"setColor", std::to_string(x), std::to_string(y), color});
}

void clearColor(int x, int y) {
    command({"clearColor", std::to_string(x), std::to_string(y)});
}

void clearAllColor() {
    command({"clearAllColor"});
}

void setText(int x, int y, const std::string& text) {
    command({"setText", std::to_string(x), std::to_string(y), text});
}

void clearText(int x, int y) {
    command({"clearText", std::to_string(x), std::to_string(y)});
}

void clearAllText() {
    command({"clearAllText"});
}

bool wasReset() {
    return command({"wasReset"}, &typeid(bool)) == "true";
}

void ackReset() {
    command({"ackReset"});
}
