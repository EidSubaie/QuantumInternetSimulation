#pragma once

#include "BaseAction.h"

namespace quisp::rules::actions {

class DoublePurifyAction : public Action {
 public:
  DoublePurifyAction(unsigned long ruleset_id, unsigned long rule_id, int partner, QNIC_type qnic_type, int qnic_id, int resource, int trash_resource_x, int trash_resource_z);
  cPacket* run(cModule* re) override;

 protected:
  int partner; /**< Identifies entanglement partner. */
  QNIC_type qnic_type;
  int qnic_id;
  int resource; /**< Identifies qubit */
  int trash_resource_Z;
  int trash_resource_X;
  int mutable purification_count;  // Used for locked_id in StationaryQubit. You unlock the qubit when purification is successful.
  int num_purify;
  int action_index = 0;  // To track how many times this particular action has been invoked.
};

class DoublePurifyActionInv : public Action {
 public:
  DoublePurifyActionInv(unsigned long ruleset_id, unsigned long rule_id, int partner, QNIC_type qnic_type, int qnic_id, int resource, int trash_resource_x, int trash_resource_z);
  cPacket* run(cModule* re) override;

 protected:
  int partner; /**< Identifies entanglement partner. */
  QNIC_type qnic_type;
  int qnic_id;
  int resource; /**< Identifies qubit */
  int trash_resource_Z;
  int trash_resource_X;
  int mutable purification_count;  // Used for locked_id in StationaryQubit. You unlock the qubit when purification is successful.
  int num_purify;
  int action_index = 0;  // To track how many times this particular action has been invoked.
};
}  // namespace quisp::rules::actions
