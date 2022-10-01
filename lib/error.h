#pragma once

#include <cstring>
#include <optional>
#include <utility>
#include <string>

namespace utils {
#ifdef UTILS_USE_EXCEPTIONS
    #define OPTIONAL(__x__) __x__
    #define TRY(__x__) __x__
    #define VOID_RETURN(__x__) ({ __x__; return; })
    #define VOID_DONE VOID_RETURN(0)
    typedef void void_t;
    template<typename... TArgs>
    auto handle_call(int32_t fn(TArgs...)) {
        return [fn](TArgs... args) -> OPTIONAL(size_t) {
            auto err = fn(args...);
            if (err > 0) {
                throw std::runtime_error(std::strerror(err));
            }
            return -err;
        };
    }
#else

#define OPTIONAL(__T__) utils::option<__T__>
#define TRY(__x__) ({ utils::option __val = __x__; if (!__val.has_value()) { return __val.exception(); } __val.value(); })
#define VOID_RETURN(__x__) ({ TRY(__x__); return utils::unit(); })
#define VOID_DONE VOID_RETURN(0)
    struct unit {
    };

    struct error {
        explicit error(std::string message);
        error() = delete;
        const std::string& message() const;
    private:
        std::string m_message{};
    };

    template<typename T>
    struct option {
        option(error err) : m_error(err), m_has_value(false) {
        }
        option(T val) : m_value(val), m_has_value(true) {
        }
        option() = delete;

        [[nodiscard]] const T& value() const {
            return *m_value;
        }

        [[nodiscard]] bool has_value() const {
            return m_has_value;
        }

        [[nodiscard]] const error& exception() const {
            return *m_error;
        }

        template<typename TResult>
        option<TResult> then(option<TResult> fn(T)) {
            if (m_has_value) {
                return fn(*m_value);
            }
            return *m_error;
        }

        void on_error(void fn(error)) {
            if (m_error) {
                fn(*m_error);
            }
        }

    private:
        std::optional<T> m_value{};
        std::optional<utils::error> m_error{};
        bool m_has_value{false};
    };

    typedef option<unit> void_t;

    template<typename... TArgs>
    auto handle_call(int32_t fn(TArgs...)) {
        return [fn](TArgs... args) -> OPTIONAL(size_t) {
            auto err = fn(args...);
            if (err > 0) {
                return utils::error(strerror(err));
            }
            return -err;
        };
    }

#endif

}