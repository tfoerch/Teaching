#include "sml.hpp"

namespace sml = boost::sml;

//                          +--------+
//       +----------------->|        |<--------------+
//       |       +--------->|  Down  |<----------+   |
//       |       |+---------|        |<-------+  |   |
//       |       ||         +--------+        |  |   |
//       |       ||           |    ^       2,9| 2|  2|
//       |       ||1b       1a|    |          |  |   |
//       |       ||           v    |2,9       |  |   |
//       |       ||         +--------+        |  |   |
//       |       ||      +->|        |<------+|  |   |
//       |       ||  4,7,|  |ConfSnd |       ||  |   |
//       |       || 14,15+--|        |<----+ ||  |   |
//       |       ||         +--------+     | ||  |   |
//       |       ||       3,8a| |          | ||  |   |
//       |       || +---------+ |8b  14,12a| ||  |   |
//       |       || |           v          | ||  |   |
//       |       |+-|------>+--------+     | ||  |   |
//       |       |  |    +->|        |-----|-|+  |   |
//       |       |  |6,14|  |ConfRcv |     | |   |   |
//       |       |  |    +--|        |<--+ | |   |   |
//       |       |  |       +--------+   | | |   |   |
//       |       |  |          5| ^      | | |   |   |
//       |       |  +---------+ | |      | | |   |   |
//       |       |            | | |      | | |   |   |
//       |       |            v v |6,12b | | |   |   |
//       |       |10        +--------+   | | |   |   |
//       |       +----------|        |   | | |   |   |
//       |       |       +--| Active |---|-+ |   |   |
//  10,17|       |   5,16|  |        |-------|---+   |
//   +-------+ 9 |   13  +->|        |   |   |       |
//   | Going |<--|----------+--------+   |   |       |
//   | Down  |   |           11| ^       |   |       |
//   +-------+   |             | |5      |   |       |
//       ^       |             v |  6,12b|   |       |
//       |9      |10        +--------+   |   |12a,14 |
//       |       +----------|        |---+   |       |
//       |                  |   Up   |-------+       |
//       +------------------|        |---------------+
//                          +--------+
//                            |   ^
//                            |   |
//                            +---+
//                           11,13,16

namespace cc {

struct ConfigMsg
{
  bool m_validConfig = true;
};
struct ConfigNackMsg {};

struct HelloMsg {};

// events
struct EvBringUp {};
struct EvCCDn {};
struct EvConfDone {};
struct EvConfErr {
  EvConfErr(const ConfigNackMsg& configNackMsg) : m_configNackMsg(configNackMsg) {}
  const ConfigNackMsg& m_configNackMsg;
};
struct EvNewConfOK {};
struct EvNewConfErr {};
struct EvContenWin {};
struct EvContenLost {
  EvContenLost(const ConfigMsg& configMsg) : m_configMsg(configMsg) {}
  const ConfigMsg& m_configMsg;
};
struct EvAdminDown {};
struct EvNbrGoesDn {};
struct EvHelloRcvd {};
struct EvHoldTimer {};
struct EvSeqNumErr {};
struct EvReconfig {};
struct EvConfRet {};
struct EvHelloRet {};
struct EvDownTimer {};

struct ActionSendConfig {};
struct ActionStopSendConfig {};
struct ActionResendConfig {};
struct ActionSendConfigAck {};
struct ActionSendConfigNack {};
struct ActionSendHello {};
struct ActionStopSendHello {};
struct ActionSetCCDownFlag {};
struct ActionClearCCDownFlag {};
struct ActionNoAction {};

// states
struct Down {};
struct ConfSnd {};
struct ConfRcv {};
struct Active {};
struct Up {};
struct GoingDown {};

struct ControlChannelFSM;

struct ActiveConfig {
  template <class TEvent>
  auto operator()(ControlChannelFSM&, const TEvent&) const -> bool;
};

struct PassiveConfig {
  template <class TEvent>
  auto operator()(const ControlChannelFSM&, const TEvent&) const -> bool;
};

struct NotAcceptableConfig {
  template <class TEvent>
  auto operator()(const ControlChannelFSM&, const TEvent&) const -> bool;
};

struct AcceptableConfig {
  template <class TEvent>
  auto operator()(const ControlChannelFSM&, const TEvent&) const -> bool;
};

struct SendConfig {
  template <class TEvent>
  void operator()(ControlChannelFSM&, const TEvent&);
};

struct StopSendConfig {
  template <class TEvent>
  void operator()(ControlChannelFSM&, const TEvent&);
};

struct ResendConfig {
  template <class TEvent>
  void operator()(ControlChannelFSM&, const TEvent&);
};

struct SendConfigNack {
  template <class TEvent>
  void operator()(ControlChannelFSM&, const TEvent&);
};

struct SendConfigAck {
  template <class TEvent>
  void operator()(ControlChannelFSM&, const TEvent&);
};

struct SendHello {
  template <class TEvent>
  void operator()(ControlChannelFSM&, const TEvent&);
};

struct StopSendHello {
  template <class TEvent>
  void operator()(ControlChannelFSM&, const TEvent&);
};

struct SetCCDownFlag {
  template <class TEvent>
  void operator()(ControlChannelFSM&, const TEvent&);
};

struct ControlChannelFSM
{
public:
  auto operator()() const
  {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      *c_down     + event<EvBringUp> [ c_activeConfig ]           / c_sendConfig     = c_confSnd,
      c_down      + event<EvBringUp> [ c_passiveConfig ]                           = c_confRcv,
      c_confSnd   + sml::on_entry<_>                            / c_sendConfig,
      c_confSnd   + sml::on_exit<_>                             / c_stopSendConfig,
      c_confSnd   + event<EvCCDn>                               / c_stopSendConfig = c_down,
      c_confSnd   + event<EvAdminDown>                          / c_stopSendConfig = c_down,
      c_confSnd   + event<EvConfErr>                            / c_sendConfig     = c_confSnd,
      c_confSnd   + event<EvContenWin>                                           = c_confSnd,
      c_confSnd   + event<EvReconfig>                           / c_sendConfig     = c_confSnd,
      c_confSnd   + event<EvConfRet>                            / c_resendConfig   = c_confSnd,
      c_confSnd   + event<EvContenLost> [ c_notAcceptableConfig ] / c_stopSendConfig = c_confRcv,
      c_confSnd   + event<EvConfDone>                           / c_stopSendConfig = c_active,
      c_confSnd   + event<EvContenLost> [ c_acceptableConfig ]    / c_stopSendConfig = c_active,
      c_confRcv   + event<EvCCDn>                               / c_stopSendConfig = c_down,
      c_confRcv   + event<EvAdminDown>                          / c_stopSendConfig = c_down,
      c_confRcv   + event<EvNewConfErr>                         / c_sendConfigNack = c_confRcv,
      c_confRcv   + event<EvReconfig>                                            = c_confRcv,
      c_confRcv   + event<EvNewConfOK>                          / c_sendConfigAck  = c_active,
      c_active    + event<EvCCDn>                               / c_stopSendHello  = c_down,
      c_active    + event<EvAdminDown>                          / c_setCCDownFlag  = c_goingDown,
      c_active    + event<EvNbrGoesDn>                          / c_stopSendHello  = c_down,
      c_active    + event<EvReconfig>                           / c_sendConfig     = c_confSnd,
      c_active    + event<EvHoldTimer> [ c_activeConfig ]         / c_sendConfig     = c_confSnd,
      c_active    + event<EvHoldTimer> [ c_passiveConfig ]        / c_stopSendHello  = c_confRcv,
      c_active    + event<EvNewConfErr>                         / c_sendConfigNack = c_confRcv,
      c_active    + event<EvNewConfOK>                                           = c_active,
      c_active    + event<EvSeqNumErr>                                           = c_active,
      c_active    + event<EvHelloRet>                           / c_sendHello      = c_active,
      c_active    + event<EvHelloRcvd>                                           = c_up,
      c_up        + event<EvCCDn>                               / c_stopSendHello  = c_down,
      c_up        + event<EvAdminDown>                          / c_setCCDownFlag  = c_goingDown,
      c_up        + event<EvNbrGoesDn>                          / c_stopSendHello  = c_down,
      c_up        + event<EvReconfig>                           / c_sendConfig     = c_confSnd,
      c_up        + event<EvHoldTimer> [ c_activeConfig ]         / c_sendConfig     = c_confSnd,
      c_up        + event<EvHoldTimer> [ c_passiveConfig ]        / c_stopSendHello  = c_confRcv,
      c_up        + event<EvNewConfErr>                         / c_sendConfigNack = c_confRcv,
      c_up        + event<EvNewConfOK>                                           = c_active,
      c_up        + event<EvSeqNumErr>                                           = c_up,
      c_up        + event<EvHelloRet>                           / c_sendHello      = c_up,
      c_up        + event<EvHelloRcvd>                                           = c_up,
      c_goingDown + event<EvNbrGoesDn>                          / c_stopSendHello  = c_down,
      c_goingDown + event<EvDownTimer>                          / c_stopSendHello  = c_down
    );
  }

  [[nodiscard]] constexpr auto isActiveConfig() const -> bool { return m_activeConfig; }

  [[nodiscard]] static auto isConfigAcceptable(const EvContenLost& eventContenLost)  -> bool
  { return eventContenLost.m_configMsg.m_validConfig; }
  static void sendConfigMsg();
  static void stopSendConfigMsg();
  static void resendConfigMsg();
  static void sendConfigNackMsg();
  static void sendConfigAckMsg();
  static void sendHelloMsg();
  static void stopSendHelloMsg();
  static void setCCDownFlagOnMsg();
  bool  m_activeConfig;
  static constexpr sml::front::state_sm<Down>::type  c_down = {};
  static constexpr sml::front::state_sm<ConfSnd>::type  c_confSnd = {};
  static constexpr sml::front::state_sm<ConfRcv>::type  c_confRcv = {};
  static constexpr sml::front::state_sm<Active>::type  c_active = {};
  static constexpr sml::front::state_sm<Up>::type  c_up = {};
  static constexpr sml::front::state_sm<GoingDown>::type  c_goingDown = {};
  static constexpr ActiveConfig c_activeConfig = {};
  static constexpr PassiveConfig c_passiveConfig = {};
  static constexpr AcceptableConfig c_acceptableConfig = {};
  static constexpr NotAcceptableConfig c_notAcceptableConfig = {};
  static constexpr SendConfig c_sendConfig = {};
  static constexpr StopSendConfig c_stopSendConfig = {};
  static constexpr ResendConfig c_resendConfig = {};
  static constexpr SendConfigNack c_sendConfigNack = {};
  static constexpr SendConfigAck c_sendConfigAck = {};
  static constexpr SendHello c_sendHello = {};
  static constexpr StopSendHello c_stopSendHello = {};
  static constexpr SetCCDownFlag c_setCCDownFlag = {};
};

template <class TEvent> auto ActiveConfig::operator()(ControlChannelFSM& ccFsm, const TEvent&) const -> bool
{ return ccFsm.isActiveConfig(); }
template <class TEvent> auto PassiveConfig::operator()(const ControlChannelFSM& ccFsm, const TEvent&) const -> bool
{ return !ccFsm.isActiveConfig(); }

template <class TEvent> auto AcceptableConfig::operator()(const ControlChannelFSM&  /*ccFsm*/, const TEvent& event) const -> bool
{ return ControlChannelFSM::isConfigAcceptable(event); }
template <class TEvent> auto NotAcceptableConfig::operator()(const ControlChannelFSM&  /*ccFsm*/, const TEvent& event) const -> bool
{ return !ControlChannelFSM::isConfigAcceptable(event); }

template <class TEvent> void SendConfig::operator()(ControlChannelFSM&  /*ccFsm*/, const TEvent&)
{ ControlChannelFSM::sendConfigMsg(); }
template <class TEvent> void StopSendConfig::operator()(ControlChannelFSM&  /*ccFsm*/, const TEvent&)
{ ControlChannelFSM::stopSendConfigMsg(); }
template <class TEvent> void ResendConfig::operator()(ControlChannelFSM&  /*ccFsm*/, const TEvent&)
{ ControlChannelFSM::resendConfigMsg(); }
template <class TEvent> void SendConfigNack::operator()(ControlChannelFSM&  /*ccFsm*/, const TEvent&)
{ ControlChannelFSM::sendConfigNackMsg(); }
template <class TEvent> void SendConfigAck::operator()(ControlChannelFSM&  /*ccFsm*/, const TEvent&)
{ ControlChannelFSM::sendConfigAckMsg(); }
template <class TEvent> void SendHello::operator()(ControlChannelFSM&  /*ccFsm*/, const TEvent&)
{ ControlChannelFSM::sendHelloMsg(); }
template <class TEvent> void StopSendHello::operator()(ControlChannelFSM&  /*ccFsm*/, const TEvent&)
{ ControlChannelFSM::stopSendHelloMsg(); }
template <class TEvent> void SetCCDownFlag::operator()(ControlChannelFSM&  /*ccFsm*/, const TEvent&)
{ ControlChannelFSM::setCCDownFlagOnMsg(); }

} // namespace cc
