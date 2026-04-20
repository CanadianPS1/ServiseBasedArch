#include "nlohmann/json.hpp"
class Orders{
    public:
    Orders();
    static void KafkaConnect();
    static void Email(nlohmann::json messageJson);
    static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);
    static void UpdateOrders();
    static void Produce(const std::string& message, const char* topic);
    static void orderLoop();
};