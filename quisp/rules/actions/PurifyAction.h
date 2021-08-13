#pragma once

#include "BaseAction.h"

namespace quisp {
namespace rules {
namespace actions {

class PurifyAction : public Action {
 protected:
  unsigned long ruleset_id;
  unsigned long rule_id;
  int partner; /**< Identifies entanglement partner. */
  QNIC_type qnic_type;
  int qnic_id;
  int resource; /**< Identifies qubit */
  int trash_resource;
  int mutable purification_count;  // Used for locked_id in StationaryQubit. You unlock the qubit when purification is successful.
  bool X;
  bool Z;
  int num_purify;
  int action_index = 0;  // To track how many times this particular action has been invoked.
  // To track the number of invocation with partner address. (partner_address, action_index)
  std::map<int, int> action_indices;

 public:
  PurifyAction();
  PurifyAction(int part, QNIC_type qt, int qi, int res, int tres, unsigned long rs_id, unsigned long r_id);
  PurifyAction(unsigned long RuleSet_id, unsigned long rule_index, bool X_purification, bool Z_purification, int num_purification, int part, QNIC_type qt, int qi, int res, int tres);
  // cPacket* run(qnicResources *resources) override;
  // cPacket* run(cModule *re, qnicResources *resources) override;
  cPacket* run(cModule* re) override;
};

}  // namespace actions
}  // namespace rules
}  // namespace quisp
