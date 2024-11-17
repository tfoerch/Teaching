#include "ControlChannelFSM.hpp"
#include <cassert>

auto main() -> int
{
  using namespace cc;
  ControlChannelFSM controlChannelFsm{true};
  sml::sm<ControlChannelFSM> ccfsm{controlChannelFsm};
  assert(ccfsm.is(ControlChannelFSM::c_down));
  ccfsm.process_event(EvBringUp{});
  assert(ccfsm.is(ControlChannelFSM::c_confSnd));
  ccfsm.process_event(EvConfDone{});
  assert(ccfsm.is(ControlChannelFSM::c_active));
  ccfsm.process_event(EvHelloRcvd{});
  assert(ccfsm.is(ControlChannelFSM::c_up));
}