#pragma once
#include <meta/concepts.h>
#include <concepts>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

namespace utils {
    class service_provider {
    public:
        template <typename TService>
        void execute_on(const std::function<void(TService*)>& method)
        {
            const auto& typeId = typeid(TService).hash_code();
            const auto& types = get_all(typeId);
            for (const auto& type : types) {
                method(reinterpret_cast<TService*>(type));
            }
        }
        template <typename TService>
        requires is_pointer<TService>
        TService get()
        {
            static TService ref;
            const auto& types = get_all(correct_type_id(ref).hash_code());
            if (types.empty()) {
                return nullptr;
            }
            return reinterpret_cast<TService>(types[0]);
        }

        template <typename TService>
        requires is_normal<TService>
        TService* get()
        {
            return get<TService*>();
        }

        template <typename TService>
        requires is_container<TService>
        TService get()
        {
            auto test = nullptr;
            static TService ref;
            auto services = allocate(ref);
            const auto& types = get_all(correct_type_id(services).hash_code());
            for (auto& type : types) {
                services->push_back(correct_cast(services, type));
            }
            return services;
        }
        template <typename T>
        void execute_on(T func)
        {
            execute_on(std::function(func));
        }

        template <typename T>
        auto inject(T func)
        {
            return preform_inject(std::function(func));
        }

        template <typename T>
        auto construct()
        {
            return preform_inject(std::function(T::make));
        }

    protected:
        virtual std::vector<void*> get_all(size_t id) = 0;

    private:
        template <typename T>
        T* correct_cast(std::shared_ptr<std::vector<T*>> ref, void* obj)
        {
            return reinterpret_cast<T*>(obj);
        }

        template <typename T>
        std::shared_ptr<std::vector<T*>> allocate(std::shared_ptr<std::vector<T*>> ref)
        {
            return std::make_shared<std::vector<T*>>();
        }

        template <typename TReturn, typename... TArgs>
        TReturn preform_inject(const std::function<TReturn(TArgs...)>& func)
        {
            return func(get<TArgs>()...);
        }

        template <typename T>
        const std::type_info& correct_type_id(std::shared_ptr<std::vector<T*>> ref)
        {
            return typeid(T);
        }

        template <typename T>
        const std::type_info& correct_type_id(T* ref)
        {
            return typeid(T);
        }
    };

#define MULTIPLE_SERVICES(__T__) std::shared_ptr<std::vector<__T__>>

}