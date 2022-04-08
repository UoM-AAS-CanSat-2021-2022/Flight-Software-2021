#include <Arduino.h>
#include <SoftwareSerial.h>
#include <XBee.h>
#include <cstdint>

#include "xbee/setup.hpp"
#include "xbee/panid_changer.hpp"

namespace xbee {
    std::uint8_t id_cmd[2] = { 'I', 'D' };
    std::uint8_t wr_cmd[2] = { 'W', 'R' };
    std::uint8_t ac_cmd[2] = { 'A', 'C' };

	PanidChanger::PanidChanger() : cmds_sent(3), oks_received(3), panid{0, 0} {}

	void PanidChanger::change_to(std::uint16_t panid) {
		this->panid[0] = (panid >> 8) & 0xFF;
		this->panid[1] = panid & 0xFF;
	}

	void PanidChanger::ok_received() {
		oks_received += 1;
	}

	void PanidChanger::loop_logic() {
		// This ordering of packets is what the XCTU utility does when it a write to the ID parameter.
		switch (cmds_sent) {
			case 0:
				// send the ID command to set the value
				at_req.setCommand(xbee::id_cmd);
				at_req.setCommandValue(panid);
				at_req.setCommandValueLength(2);
				xb.send(at_req);
				cmds_sent++;
				break;
			case 1:
				// send the WR command to write it? (guessing)
				if (oks_received != 1) break;
				at_req.clearCommandValue();
				at_req.setCommand(xbee::wr_cmd);
				xb.send(at_req);
				cmds_sent++;
				break;
			case 2:
				// I have no idea 
				if (oks_received != 2) break;
				at_req.setCommand(xbee::ac_cmd);
				xb.send(at_req);
				cmds_sent++;
				break;
			default:
				break;
		}
	}

	bool PanidChanger::changed() {
		return cmds_sent == 3 &&  oks_received == 3;
	}
}
