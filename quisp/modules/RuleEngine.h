/** \file RuleEngine.h
 *  \todo clean Clean code when it is simple.
 *  \todo doc Write doxygen documentation.
 *  \authors cldurand,takaakimatsuo
 *  \date 2018/04/04
 *
 *  \brief RuleEngine
 */
#ifndef QUISP_MODULES_RULEENGINE_H_
#define QUISP_MODULES_RULEENGINE_H_

//#include <vector>
//#include <omnetpp.h>
#include <classical_messages_m.h>
//#include <modules/stationaryQubit.h>
//#include <modules/RealTimeController.h>
//#include <modules/HardwareMonitor.h>
//#include <modules/HoM_Controller.h>
//#include <modules/QUBIT.h>
#include <rules/RuleSet.h>

using namespace omnetpp;

namespace quisp {
namespace modules {
using namespace rules;

/** \class RuleEngine RuleEngine.h
 *  \todo Documentation of the class header.
 *  \note The Connection Manager responds to connection requests received from other nodes.
 *        Connection setup, so a regular operation but not high bandwidth, relatively low constraints.
 *        Connections from nearest neighbors only.
 *        Connection manager needs to know which qnic is connected to where, which QNode not HoM/EPPS.
 *
 *  \brief RuleEngine
 */

//Process = RuleSet
typedef struct _process{
    int ownner_addr;
    int working_partner_addr;
    //int process_ID;
    RuleSet* RuleSet;
} process;

class RuleEngine : public cSimpleModule
{
    //friend class Action;
    public:
        int parentAddress;//Parent QNode's address
        EmitPhotonRequest *emt;
        HardwareMonitor::NeighborTable ntable;
        int number_of_qnics_all;//qnic,qnic_r,_qnic_rp
        int number_of_qnics;
        int number_of_qnics_r;
        int number_of_qnics_rp;
        typedef std::map<int, QubitState> QubitStateTable;
        typedef std::map<int, QubitAddr_cons> sentQubitIndexTracker;//nth shot -> node/qnic/qubit index (node addr not needed actually)
        //Although qnic index is in QubitAddr, lest make int qnic_index -> QubisState to lessen the search
        //QubitStateTable stable, stable_r, stable_rp;
        QubitStateTable* Busy_OR_Free_QubitState_table;
        sentQubitIndexTracker* tracker;
        HardwareMonitor *hardware_monitor;
        RealTimeController *realtime_controller;
        int* qnic_burst_trial_counter;
        typedef std::map<int,bool> trial_tracker;//trial index, false or true (that trial is over or not)
        qnicResources* allResources; //Size will be defined in initialization. If 3 qnic types, then size is 3. Type defined in QUBIT.h
        /*
         * DEFINED in QNIC.h
         * typedef std::multimap<int, stationaryQubit*> EntangledPairs;//entangled Node address -> pointer to that local qubit
         * typedef EntangledPairs* qnicResources;//For each qnic. If the number of "qnic" is 3, then the size is 3.
         * For resource management over.
         * */

        //typedef rules::RuleSet* RuleSetPtr;
        typedef std::map<int, process> running_processes;
        running_processes rp;
        //typedef std::map<std::string, quisp::rules::RuleSet> processes;//process_id -> Rule set

    protected:
        virtual void initialize() override;
        virtual void finish() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual cModule* getQNode();
        virtual int countFreeQubits_inQnic(QubitStateTable table, int qnic_index);
        virtual int getOneFreeQubit_inQnic(QubitStateTable table, int qnic_index);
        virtual QubitStateTable setQubitBusy_inQnic(QubitStateTable table, int qnic_index, int qubit_index);
        virtual QubitStateTable setQubitFree_inQnic(QubitStateTable table, int qnic_index, int qubit_index);
        virtual QubitStateTable initializeQubitStateTable(QubitStateTable temp, QNIC_type qnic_type);
        virtual void scheduleFirstPhotonEmission(BSMtimingNotifier *pk, QNIC_type qnic_type);
        virtual void shootPhoton(SchedulePhotonTransmissionsOnebyOne * pk);
        //virtual int getQNICjob_index_for_this_qnic(int qnic_index, QNIC_type qnic_type);
        virtual void incrementBurstTrial(int destAddr, int internal_qnic_address, int internal_qnic_index);
        virtual void shootPhoton_internal(SchedulePhotonTransmissionsOnebyOne * pk);
        virtual bool burstTrial_outdated(int this_trial, int qnic_address);
        //virtual int getQnicIndex_toNeighbor(int destAddr);
        virtual Interface_inf getInterface_toNeighbor(int destAddr);
        virtual Interface_inf getInterface_toNeighbor_Internal(int local_qnic_index);
        virtual void scheduleNextEmissionEvent(int qnic_index, int qnic_address, double interval, simtime_t timing, int num_sent, bool internal, int trial);
        virtual void freeFailedQubits_and_AddAsResource(int destAddr, int internal_qnic_address, int internal_qnic_index, CombinedBSAresults *pk_result);
        virtual void clearTrackerTable(int destAddr, int internal_qnic_address);
        virtual void traverseThroughAllProcesses(int qnic_type, int qnic_index);
        virtual double predictResourceFidelity(QNIC_type qnic_type, int qnic_index, int entangled_node_address, int resource_index);
        virtual void freeResource(int qnic_index, int qubit_index, QNIC_type qnic_type);
};

} // namespace modules
} // namespace quisp

#endif /* MODULES_RULEENGINE_H_ */
