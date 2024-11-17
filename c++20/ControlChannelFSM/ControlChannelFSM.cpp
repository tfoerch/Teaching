#include "ControlChannelFSM.hpp"
#include <iostream>

namespace cc {

void ControlChannelFSM::sendConfigMsg() { std::cout << "sendConfig" << '\n'; }
void ControlChannelFSM::stopSendConfigMsg() { std::cout << "stopSendConfigMsg" << '\n'; }
void ControlChannelFSM::resendConfigMsg() { std::cout << "resendConfigMsg" << '\n'; }
void ControlChannelFSM::sendConfigNackMsg() { std::cout << "sendConfigNackMsg" << '\n'; }
void ControlChannelFSM::sendConfigAckMsg() { std::cout << "sendConfigAckMsg" << '\n'; }
void ControlChannelFSM::sendHelloMsg() { std::cout << "sendHelloMsg" << '\n'; }
void ControlChannelFSM::stopSendHelloMsg() { std::cout << "stopSendHelloMsg" << '\n'; }
void ControlChannelFSM::setCCDownFlagOnMsg() { std::cout << "setCCDownFlagOnMsg" << '\n'; }
 
} // namespace cc
