/** \file RuleEngine.h
 *  \authors cldurand,takaakimatsuo
 *  \date 2018/04/04
 *
 *  \brief RuleEngine
 */
#ifndef QUISP_MODULES_RULEENGINE_H_
#define QUISP_MODULES_RULEENGINE_H_

#include <omnetpp.h>
#include <rules/RuleSet.h>
#include <vector>

#include <messages/classical_messages.h>
#include "../../PhysicalConnection/BSA/HoMController.h"
#include "BellPairStore/BellPairStore.h"
#include "IRuleEngine.h"
#include "QNicStore/IQNicStore.h"
#include "RuleSetStore/RuleSetStore.h"
#include "modules/QNIC/StationaryQubit/IStationaryQubit.h"
#include "modules/QRSA/HardwareMonitor/HardwareMonitor.h"
#include "modules/QRSA/RealTimeController/IRealTimeController.h"
#include "modules/QRSA/RoutingDaemon/RoutingDaemon.h"
#include "modules/QRSA/RuleEngine/QubitRecord/IQubitRecord.h"
#include "modules/QUBIT.h"
#include "utils/ComponentProvider.h"

using namespace omnetpp;

namespace quisp {
namespace modules {
using namespace rules;
using qnic_store::IQNicStore;
using qubit_record::IQubitRecord;

/** \class RuleEngine RuleEngine.h
 *  \note The Connection Manager responds to connection requests received from other nodes.
 *        Connection setup, so a regular operation but not high bandwidth, relatively low constraints.
 *        Connections from nearest neighbors only.
 *        Connection manager needs to know which qnic is connected to where, which QNode not HoM/EPPS.
 *
 *  \brief RuleEngine
 */

class RuleEngine : public IRuleEngine {
 private:
  // simsignal_t recog_resSignal;
  simsignal_t actual_resSignal;
  // friend class Action;
 public:
  RuleEngine();
  int parentAddress;  // Parent QNode's address
  EmitPhotonRequest *emt;
  NeighborTable ntable;
  int number_of_qnics_all;  // qnic,qnic_r,_qnic_rp
  int number_of_qnics;
  int number_of_qnics_r;
  int number_of_qnics_rp;
  PurificationTable Purification_table;
  DoublePurificationTable DoublePurification_table;
  TriplePurificationTable TriplePurification_table;
  QuatroPurificationTable QuatroPurification_table;

  bool *terminated_qnic;  // When you need to intentionally stop the link to make the simulation lighter.
  sentQubitIndexTracker *tracker;
  IHardwareMonitor *hardware_monitor;
  IRoutingDaemon *routingdaemon;
  IRealTimeController *realtime_controller;
  int *qnic_burst_trial_counter;
  BellPairStore bell_pair_store;
  // typedef rules::RuleSet* RuleSetPtr;
  ruleset_store::RuleSetStore rp;
  // Vector for store package for simultaneous entanglement swapping
  std::map<int, std::map<int, int>> simultaneous_es_results;
  // tracker accessible table has as many number of boolean value as the number of qnics in the qnode.
  // when the tracker for the qnic is clered by previous BSM trial it goes true
  // when the RuleEngine try to start new Photon emittion, it goes false and other BSM trial can't access to it.
  std::vector<bool> tracker_accessible;

  void freeResource(int qnic_index, int qubit_index, QNIC_type qnic_type) override;
  void freeConsumedResource(int qnic_index, IStationaryQubit *qubit, QNIC_type qnic_type) override;
  void ResourceAllocation(int qnic_type, int qnic_index) override;

 protected:
  void initialize() override;
  void finish() override;
  void handleMessage(cMessage *msg) override;
  void scheduleFirstPhotonEmission(BSMtimingNotifier *pk, QNIC_type qnic_type);
  void sendPhotonTransmissionSchedule(PhotonTransmissionConfig transmission_config);
  void shootPhoton(SchedulePhotonTransmissionsOnebyOne *pk);
  // virtual int getQNICjob_index_for_this_qnic(int qnic_index, QNIC_type qnic_type);
  void incrementBurstTrial(int destAddr, int internal_qnic_address, int internal_qnic_index);
  void shootPhoton_internal(SchedulePhotonTransmissionsOnebyOne *pk);
  bool burstTrial_outdated(int this_trial, int qnic_address);
  // virtual int getQnicIndex_toNeighbor(int destAddr);
  InterfaceInfo getInterface_toNeighbor(int destAddr);
  InterfaceInfo getInterface_toNeighbor_Internal(int local_qnic_index);
  void scheduleNextEmissionEvent(int qnic_index, int qnic_address, double interval, simtime_t timing, int num_sent, bool internal, int trial);
  void freeFailedQubits_and_AddAsResource(int destAddr, int internal_qnic_address, int internal_qnic_index, CombinedBSAresults *pk_result);
  void clearTrackerTable(int destAddr, int internal_qnic_address);
  // virtual void traverseThroughAllProcesses(RuleEngine *re, int qnic_type, int qnic_index);
  void traverseThroughAllProcesses2();
  double predictResourceFidelity(QNIC_type qnic_type, int qnic_index, int entangled_node_address, int resource_index);
  // virtual void check_Purification_Agreement(purification_result pr);
  void storeCheck_Purification_Agreement(purification_result pur_result);
  void storeCheck_DoublePurification_Agreement(Doublepurification_result pr);
  void storeCheck_TriplePurification_Agreement(Triplepurification_result pr);
  void storeCheck_QuatroPurification_Agreement(Quatropurification_result pr);
  void Unlock_resource_and_upgrade_stage(unsigned long ruleset_id, unsigned long rule_id, int index);
  void Unlock_resource_and_discard(unsigned long ruleset_id, unsigned long rule_id, int index);

  void updateResources_EntanglementSwapping(swapping_result swapr);
  virtual void updateResources_SimultaneousEntanglementSwapping(swapping_result swapr);

  utils::ComponentProvider provider;
  std::unique_ptr<IQNicStore> qnic_store = nullptr;
};

}  // namespace modules
}  // namespace quisp

#endif /* MODULES_RULEENGINE_H_ */
