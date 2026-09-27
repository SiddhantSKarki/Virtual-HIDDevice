#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <thread>

static constexpr const char* SOCKET_PATH = "/tmp/hid_mouse_lab.sock";

struct MouseReport {
    uint8_t buttons;
    int8_t dx;
    int8_t dy;
    int8_t wheel;
};

static int connectToHost() {
    std::cout << "[device] pid=" << getpid() << "\n";
    std::cout << "[device] socket path=" << SOCKET_PATH << "\n";

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        std::cerr << "[device] socket failed: " << std::strerror(errno) << "\n";
        return -1;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    std::cout << "[device] connecting...\n";

    if (connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        std::cerr << "[device] connect failed: " << std::strerror(errno) << "\n";
        close(fd);
        return -1;
    }

    std::cout << "[device] connected. fd=" << fd << "\n";
    return fd;
}

static bool sendReport(int fd, const MouseReport& report) {
    const auto* bytes = reinterpret_cast<const uint8_t*>(&report);
    size_t totalWritten = 0;

    std::cout << "[device] sending report: "
              << "buttons=0x" << std::hex << static_cast<int>(report.buttons) << std::dec
              << ", dx=" << static_cast<int>(report.dx)
              << ", dy=" << static_cast<int>(report.dy)
              << ", wheel=" << static_cast<int>(report.wheel)
              << "\n";

    while (totalWritten < sizeof(report)) {
        ssize_t n = write(fd, bytes + totalWritten, sizeof(report) - totalWritten);

        if (n < 0) {
            std::cerr << "[device] write failed: " << std::strerror(errno) << "\n";
            return false;
        }

        std::cout << "[device] write returned " << n << "\n";

        totalWritten += static_cast<size_t>(n);
    }

    return true;
}

int main() {
    std::cout.setf(std::ios::unitbuf);

    int fd = connectToHost();
    if (fd == -1) {
        return 1;
    }

    MouseReport moveRight{
        .buttons = 0x00,
        .dx = 20,
        .dy = 0,
        .wheel = 0
    };

    MouseReport leftDown{
        .buttons = 0x01,
        .dx = 0,
        .dy = 0,
        .wheel = 0
    };

    MouseReport dragRight{
        .buttons = 0x01,
        .dx = 15,
        .dy = 0,
        .wheel = 0
    };

    MouseReport leftUp{
        .buttons = 0x00,
        .dx = 0,
        .dy = 0,
        .wheel = 0
    };

    sendReport(fd, moveRight);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    sendReport(fd, leftDown);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    sendReport(fd, dragRight);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    sendReport(fd, leftUp);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    close(fd);

    std::cout << "[device] done\n";
    return 0;
}