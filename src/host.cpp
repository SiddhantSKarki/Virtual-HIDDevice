#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

static constexpr const char* SOCKET_PATH = "/tmp/hid_mouse_lab.sock";

struct MouseReport {
    uint8_t buttons;
    int8_t dx;
    int8_t dy;
    int8_t wheel;
};

static int createServerSocket() {
    int serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverFd == -1) {
        std::cerr << "socket failed: " << std::strerror(errno) << "\n";
        return -1;
    }

    unlink(SOCKET_PATH);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(serverFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        std::cerr << "bind failed: " << std::strerror(errno) << "\n";
        close(serverFd);
        return -1;
    }

    if (listen(serverFd, 1) == -1) {
        std::cerr << "listen failed: " << std::strerror(errno) << "\n";
        close(serverFd);
        return -1;
    }

    return serverFd;
}

static bool readExact(int fd, void* buffer, size_t size) {
    auto* bytes = static_cast<uint8_t*>(buffer);
    size_t totalRead = 0;

    while (totalRead < size) {
        ssize_t n = read(fd, bytes + totalRead, size - totalRead);

        if (n == 0) {
            return false;
        }

        if (n < 0) {
            std::cerr << "read failed: " << std::strerror(errno) << "\n";
            return false;
        }

        totalRead += static_cast<size_t>(n);
    }

    return true;
}

static void printMouseReport(const MouseReport& report) {
    bool left = report.buttons & 0x01;
    bool right = report.buttons & 0x02;
    bool middle = report.buttons & 0x04;

    std::cout << "MouseReport { "
              << "buttons=0x" << std::hex << static_cast<int>(report.buttons) << std::dec
              << ", left=" << left
              << ", right=" << right
              << ", middle=" << middle
              << ", dx=" << static_cast<int>(report.dx)
              << ", dy=" << static_cast<int>(report.dy)
              << ", wheel=" << static_cast<int>(report.wheel)
              << " }\n";
}

int main() {
    int serverFd = createServerSocket();
    if (serverFd == -1) {
        return 1;
    }

    std::cout << "hid_host listening on " << SOCKET_PATH << "\n";
    std::cout << "Run ./build/fake_mouse in another terminal.\n";

    int clientFd = accept(serverFd, nullptr, nullptr);
    if (clientFd == -1) {
        std::cerr << "accept failed: " << std::strerror(errno) << "\n";
        close(serverFd);
        return 1;
    }

    std::cout << "fake mouse connected.\n";

    while (true) {
        MouseReport report{};

        if (!readExact(clientFd, &report, sizeof(report))) {
            std::cout << "fake mouse disconnected.\n";
            break;
        }

        printMouseReport(report);
    }

    close(clientFd);
    close(serverFd);
    unlink(SOCKET_PATH);

    return 0;
}