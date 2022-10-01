#include <service_container.h>

struct greeter {
    virtual void greet() = 0;
};

struct greeter_config {
    greeter_config(std::string val) : m_value(std::move(val)) { }
    const std::string& get() {
        return m_value;
    }
private:
    std::string m_value;
};

struct greeter_service : public greeter {
    static greeter_service* make(greeter_config *config) {
        return new greeter_service(config->get());
    }

    void greet() final {
        std::cout << "Hello, " << m_who << "!" << std::endl;
    }
private:
    greeter_service(std::string who) : m_who(std::move(who)) { }
    std::string m_who;
};

int greet_function(greeter *test) {
    test->greet();
    return 0;
}

int main() {
    utils::service_container container{};
    container.add(new greeter_config("World"));
    container.add<greeter, greeter_service>();

    auto test = container.get<greeter>();
    test->greet();

    return container.inject(greet_function);
}