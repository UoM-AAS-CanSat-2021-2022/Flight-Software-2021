#pragma once

#include <cstdint>
#include <XBee.h>

namespace xbee {
    class PanidChanger {
        std::uint8_t cmds_sent;
        std::uint8_t oks_received;
        std::uint8_t panid[2];
        AtCommandRequest at_req;

    public:
        PanidChanger();

        void change_to(std::uint16_t);
        void ok_received();
        void loop_logic();
        bool changed();
    };
}
