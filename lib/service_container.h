#pragma once

#include <interface/service_collection.h>

namespace utils {
    class service_container final : public service_collection {
    public:
        ~service_container();

    protected:
        std::vector<void*> get_all(size_t id) final;
        void register_type(size_t id, void* service) final;
        void register_finalizer(void* service, const std::function<void()>& finalizer) final;
        void remove(size_t id, void* service) final;

    private:
        std::unordered_map<size_t, std::vector<void*>> m_types {};
        std::unordered_map<void*, std::function<void()>> m_finalizers {};
    };
}