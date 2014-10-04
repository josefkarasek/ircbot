#include <netinet/in.h>
#include <string.h>

class INET_Addr {
public:
    INET_Addr (u_short port, long addr, bool enable_ipv6) {
        memset (reinterpret_cast <void *> (&addr_), 0, sizeof addr_);
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons (port);
        addr_.sin_addr.s_addr = htonl (addr);
    }

    u_short get_port (void) const {
        return addr_.sin_port;
    }

    long get_ip_addr (void) const {
        return addr_.sin_addr.s_addr;
    }

    sockaddr * addr (void) {
        return reinterpret_cast <sockaddr *>(&addr_);
    }

    size_t size (void) const {
        return sizeof (addr_);
    }

private:
    sockaddr_in addr_;
};
