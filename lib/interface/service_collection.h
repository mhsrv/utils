#pragma once
#include "service_provider.h"
namespace utils {
    class service_collection : public service_provider {
    public:
        template <typename TRegister, typename TService>
        TRegister* add(TService* service)
        {
            auto obj = dynamic_cast<TRegister*>(service);
            if (obj == nullptr) {
                return nullptr;
            }
            register_type(typeid(TRegister).hash_code(), obj);
            return obj;
        }

        template <typename TService>
        TService* add()
        {
            return add<TService, TService>();
        }

        template <typename TRegister, typename TService>
        TRegister* add()
        {
            auto* cs = construct<TService>();
            register_finalizer((void*)cs, [cs]() {
                delete cs;
            });
            auto obj = dynamic_cast<TRegister*>(cs);
            if (obj == nullptr) {
                return nullptr;
            }
            register_type(typeid(TRegister).hash_code(), obj);
            return obj;
        }

        template <typename TService>
        TService* add(TService* service)
        {
            return add<TService, TService>(service);
        }

        template <typename TService>
        void remove(TService* service)
        {
            auto& typeId = typeid(TService);
            auto hashCode = typeId.hash_code();
            remove(hashCode, service);
        }

    protected:
        virtual void register_type(size_t id, void* service) = 0;
        virtual void register_finalizer(void* service, const std::function<void()>& finalizer) = 0;
        virtual void remove(size_t id, void* service) = 0;
    };
}