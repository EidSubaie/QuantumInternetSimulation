#pragma once

#include "BaseAction.h"

namespace quisp::rules::actions {

// https://arxiv.org/abs/0811.2639
class DoubleSelectionDualAction : public Action {
 public:
  DoubleSelectionDualAction(unsigned long ruleset_id, unsigned long rule_id, int partner, QNIC_type qnic_type, int qnic_index, int resource, int trash_resource_x,
                            int trash_resource_z, int ds_trash_resource_x, int ds_trash_resource_z);
  cPacket* run(cModule* re) override;

 protected:
  int partner; /**< Identifies entanglement partner. */
  QNIC_type qnic_type;
  int qnic_id;
  int resource; /**< Identifies qubit */
  int trash_resource_Z;
  int trash_resource_X;
  int doubleselection_trash_resource_Z;
  int doubleselection_trash_resource_X;
  int mutable purification_count;  // Used for locked_id in StationaryQubit. You unlock the qubit when purification is successful.
  int num_purify;
  int action_index = 0;  // To track how many times this particular action has been invoked.
};

// https://arxiv.org/abs/0811.2639
class DoubleSelectionDualActionInv : public Action {
 public:
  DoubleSelectionDualActionInv(unsigned long ruleset_id, unsigned long rule_id, int partner, QNIC_type qnic_type, int qnic_index, int resource, int trash_resource_x,
                               int trash_resource_z, int ds_trash_resource_x, int ds_trash_resource_z);
  cPacket* run(cModule* re) override;

 protected:
  int partner; /**< Identifies entanglement partner. */
  QNIC_type qnic_type;
  int qnic_id;
  int resource; /**< Identifies qubit */
  int trash_resource_Z;
  int trash_resource_X;
  int doubleselection_trash_resource_Z;
  int doubleselection_trash_resource_X;
  int mutable purification_count;  // Used for locked_id in StationaryQubit. You unlock the qubit when purification is successful.
  int num_purify;
  int action_index = 0;  // To track how many times this particular action has been invoked.
};
}  // namespace quisp::rules::actions
