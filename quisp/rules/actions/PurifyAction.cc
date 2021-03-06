#include "PurifyAction.h"
#include <messages/classical_messages.h>
#include <modules/QRSA/RuleEngine/IRuleEngine.h>

namespace quisp::rules::actions {

PurifyAction::PurifyAction(unsigned long ruleset_id, unsigned long rule_id, bool x_purification, bool z_purification, int num_purification, int partner, QNIC_type qnic_type,
                           int qnic_id, int resource, int trash_resource)
    : Action(ruleset_id, rule_id),
      partner(partner),
      qnic_type(qnic_type),
      qnic_id(qnic_id),
      resource(resource),
      trash_resource(trash_resource),
      purification_count(num_purification),
      X(x_purification),
      Z(z_purification),
      num_purify(num_purification) {}

// Either Z or X purification.
cPacket *PurifyAction::run(cModule *re) {
  auto *qubit = getResource(resource, partner);
  auto *trash_qubit = getResource(trash_resource, partner);

  if (qubit == trash_qubit) {
    return generateError("Qubit and Trash_qubit must be different.");
  }
  if (qubit == nullptr || trash_qubit == nullptr) {
    return generateError("Not enough resource (Qubit and Trash_qubit) found. This should have been checked as a condition clause.");
  }

  bool meas = false;
  if (X && !Z) {
    meas = trash_qubit->Xpurify(qubit);
  } else if (!X && Z) {
    meas = trash_qubit->Zpurify(qubit);
  }

  qubit->Lock(ruleset_id, rule_id, action_index);

  if (trash_qubit->entangled_partner != nullptr) {
    trash_qubit->entangled_partner->no_density_matrix_nullptr_entangled_partner_ok = true;
    trash_qubit->entangled_partner->entangled_partner = nullptr;
  }

  removeResource_fromRule(trash_qubit);
  IRuleEngine *rule_engine = check_and_cast<IRuleEngine *>(re);
  rule_engine->freeConsumedResource(qnic_id, trash_qubit, qnic_type);

  PurificationResult *pk = new PurificationResult;
  pk->setDestAddr(partner);
  // This result is sent to partner address and my address.
  // To keep the information who is the purifcation partner, this variable is used.
  pk->setKind(7);
  pk->setAction_index(action_index);
  pk->setRule_id(rule_id);
  pk->setRuleset_id(ruleset_id);
  pk->setOutput_is_plus(meas);
  pk->setEntangled_with(qubit);
  action_index++;
  return pk;
}
}  // namespace quisp::rules::actions
