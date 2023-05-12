#include <vector>
#include <cstdint>

class Publisher {
public:
    void publish(const std::vector<uint8_t>& message) {
    }
};

class Subscriber {
public:
    std::vector<uint8_t> receive() {
        std::vector<uint8_t> receivedMessage = {0x01, 0x02, 0x03, 0x04, 0x05};
        return receivedMessage;
    }
};
