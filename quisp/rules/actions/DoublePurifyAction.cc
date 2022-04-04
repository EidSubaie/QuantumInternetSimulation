#include "DoublePurifyAction.h"
#include <messages/classical_messages.h>
#include <modules/QRSA/RuleEngine/IRuleEngine.h>

namespace quisp::rules::actions {

DoublePurifyAction::DoublePurifyAction(unsigned long ruleset_id, unsigned long rule_id, int partner, QNIC_type qnic_type, int qnic_id, int resource, int trash_resource_x,
                                       int trash_resource_z)
    : Action(ruleset_id, rule_id),
      partner(partner),
      qnic_type(qnic_type),
      qnic_id(qnic_id),
      resource(resource),
      trash_resource_Z(trash_resource_z),
      trash_resource_X(trash_resource_x) {}

// Double error purification
cPacket *DoublePurifyAction::run(cModule *re) {
  auto *qubit = getResource(resource, partner);
  auto *trash_qubit_X = getResource(trash_resource_X, partner);
  auto *trash_qubit_Z = getResource(trash_resource_Z, partner);

  if (qubit == trash_qubit_X || qubit == trash_qubit_Z || trash_qubit_Z == trash_qubit_X) {
    return generateError("Qubit and Trash_qubit must be different.");
  }
  if (qubit == nullptr || trash_qubit_Z == nullptr || trash_qubit_X == nullptr) {
    return generateError("Not enough resource (Qubit and Trash_qubit) found. This should have been checked as a condition clause.");
  }

  bool meas_X = trash_qubit_X->Xpurify(qubit);
  bool meas_Z = trash_qubit_Z->Zpurify(qubit);

  qubit->Lock(ruleset_id, rule_id, action_index);

  // Delete measured resource from the tracked list of resources.
  removeResource_fromRule(trash_qubit_X);
  removeResource_fromRule(trash_qubit_Z);
  IRuleEngine *rule_engine = check_and_cast<IRuleEngine *>(re);
  rule_engine->freeConsumedResource(qnic_id, trash_qubit_X, qnic_type);
  rule_engine->freeConsumedResource(qnic_id, trash_qubit_Z, qnic_type);
  // Deleting done

  DoublePurificationResult *pk = new DoublePurificationResult;
  pk->setDestAddr(partner);
  pk->setKind(7);
  pk->setAction_index(action_index);
  pk->setRule_id(rule_id);
  pk->setRuleset_id(ruleset_id);
  pk->setXOutput_is_plus(meas_X);
  pk->setZOutput_is_plus(meas_Z);
  pk->setEntangled_with(qubit);
  action_index++;
  return pk;
}

DoublePurifyActionInv::DoublePurifyActionInv(unsigned long ruleset_id, unsigned long rule_id, int partner, QNIC_type qnic_type, int qnic_id, int resource, int trash_resource_x,
                                             int trash_resource_z)
    : Action(ruleset_id, rule_id),
      partner(partner),
      qnic_type(qnic_type),
      qnic_id(qnic_id),
      resource(resource),
      trash_resource_Z(trash_resource_z),
      trash_resource_X(trash_resource_x) {}

// Inveerted double error purification.
cPacket *DoublePurifyActionInv::run(cModule *re) {
  auto *qubit = getResource(resource, partner);
  auto *trash_qubit_X = getResource(trash_resource_X, partner);
  auto *trash_qubit_Z = getResource(trash_resource_Z, partner);

  if (qubit == trash_qubit_X || qubit == trash_qubit_Z || trash_qubit_Z == trash_qubit_X) {
    return generateError("Qubit and Trash_qubit must be different.");
  }
  if (qubit == nullptr || trash_qubit_Z == nullptr || trash_qubit_X == nullptr) {
    return generateError("Not enough resource (Qubit and Trash_qubit) found. This should have been checked as a condition clause.");
  }

  bool meas_Z = trash_qubit_Z->Zpurify(qubit);
  bool meas_X = trash_qubit_X->Xpurify(qubit);

  qubit->Lock(ruleset_id, rule_id, action_index);

  // Delete measured resource from the tracked list of resources.
  removeResource_fromRule(trash_qubit_X);
  removeResource_fromRule(trash_qubit_Z);
  IRuleEngine *rule_engine = check_and_cast<IRuleEngine *>(re);
  rule_engine->freeConsumedResource(qnic_id, trash_qubit_X, qnic_type);
  rule_engine->freeConsumedResource(qnic_id, trash_qubit_Z, qnic_type);

  DoublePurificationResult *pk = new DoublePurificationResult;
  pk->setDestAddr(partner);
  pk->setKind(7);
  pk->setAction_index(action_index);
  pk->setRule_id(rule_id);
  pk->setRuleset_id(ruleset_id);
  pk->setXOutput_is_plus(meas_X);
  pk->setZOutput_is_plus(meas_Z);
  pk->setEntangled_with(qubit);
  action_index++;
  return pk;
}
}  // namespace quisp::rules::actions
