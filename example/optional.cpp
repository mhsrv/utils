#include <iostream>
#include "error.h"

utils::option<bool> create_error() {
    return utils::error("Intentional error");
}

utils::option<bool> create_true() {
    return true;
}

utils::option<bool> create_false() {
    return false;
}

void print_bool(utils::option<bool> value) {
    value.then<utils::unit>([](bool value) -> utils::void_t {
        if (value) {
            std::cout << "Everything is fine" << std::endl;
            VOID_DONE;
        } else {
            return utils::error("Not fine");
        }
    }).on_error([](utils::error error) {
        std::cout << error.message() << std::endl;
    });
}

int main() {
    print_bool(create_true());
    print_bool(create_false());
    print_bool(create_error());
    return 0;
}
