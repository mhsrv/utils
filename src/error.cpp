#include <error.h>

const std::string &utils::error::message() const {
    return m_message;
}

utils::error::error(std::string message) : m_message(std::move(message)) {
}
