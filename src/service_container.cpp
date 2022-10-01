#include <service_container.h>

utils::service_container::~service_container()
{
    for (auto& finalizer : m_finalizers) {
        finalizer.second();
    }
}

std::vector<void*> utils::service_container::get_all(size_t id)
{
    if (!m_types.contains(id)) {
        return {};
    }
    return m_types[id];
}

void utils::service_container::register_type(size_t id, void* service)
{
    if (!m_types.contains(id)) {
        m_types.insert({ id, {} });
    }
    m_types[id].push_back(service);
}

void utils::service_container::register_finalizer(void* service, const std::function<void()>& finalizer)
{
    m_finalizers.insert({ service, finalizer });
}

void utils::service_container::remove(size_t id, void* service)
{
    auto types = get_all(id);
    if (types.empty() || std::erase(m_types[id], service) == 0) {
        return;
    }

    if (m_types[id].empty()) {
        m_types.erase(id);
    }

    if (m_finalizers.contains(service)) {
        m_finalizers[service]();
        m_finalizers.erase(service);
    }
}
