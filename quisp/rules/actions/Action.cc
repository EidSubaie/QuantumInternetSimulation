/** \file Action.cc
 *
 *  \authors cldurand,takaakimatsuo
 *  \date 2018/06/25
 *
 *  \brief Action
 */
#include <messages/base_messages_m.h>
#include <omnetpp.h>
#include "BaseAction.h"

namespace quisp::rules::actions {

Action::Action(unsigned long ruleset_id, unsigned long rule_id) : ruleset_id(ruleset_id), rule_id(rule_id){};

IStationaryQubit *Action::getResource(int required_index, int partner) {
  int i = 0;
  for (auto it = rule_resources->begin(); it != rule_resources->end(); ++it) {
    if (it->first == partner && !it->second->isLocked()) {
      if (i == required_index) return it->second;
      i++;
    }
  }
  return nullptr;
}

void Action::removeResource_fromRule(IStationaryQubit *qubit) {
  for (auto it = rule_resources->begin(); it != rule_resources->end(); ++it) {
    if (it->second == qubit) {
      rule_resources->erase(it);
      return;
    }
  }
}
cPacket *Action::generateError(const char *msg) {
  auto *error = new quisp::messages::Error();
  error->setError_text(msg);
  return error;
}

}  // namespace quisp::rules::actions
