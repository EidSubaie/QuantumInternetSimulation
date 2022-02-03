#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <test_utils/TestUtils.h>
#include <memory>
#include "Clause.h"

namespace {
using namespace quisp_test;
using namespace quisp::rules;
using quisp::modules::QNIC_type;

TEST(ClauseTest, EnoughResourceConditionClause_serialize_json) {
  prepareSimulation();
  // num_resource, required_fidelity, partner_addr, qnic_type, qnic_id
  auto enough_resource = std::make_unique<EnoughResourceConditionClause>(3, 0.85, 1, QNIC_R, 13);
  json enough_resource_json = enough_resource->serialize_json();
  EXPECT_EQ(enough_resource_json["type"], "enough_resource");
  EXPECT_EQ(enough_resource_json["options"]["num_resources"], 3);
  EXPECT_EQ(enough_resource_json["options"]["required_fidelity"], 0.85);
  EXPECT_EQ(enough_resource_json["options"]["partner_address"], 1);
  EXPECT_EQ(enough_resource_json["options"]["qnic_type"], 1);
  EXPECT_EQ(enough_resource_json["options"]["qnic_id"], 13);
}

TEST(ClauseTest, MeasureCountConditionClause_serialize_json) {
  prepareSimulation();
  // num measure
  auto measure_count = std::make_unique<MeasureCountConditionClause>(1000, 1, QNIC_E, 15);
  json measure_count_json = measure_count->serialize_json();
  EXPECT_EQ(measure_count_json["type"], "measure_count");
  EXPECT_EQ(measure_count_json["options"]["num_measure"], 1000);
  EXPECT_EQ(measure_count_json["options"]["partner_address"], 1);
  EXPECT_EQ(measure_count_json["options"]["qnic_type"], 0);
  EXPECT_EQ(measure_count_json["options"]["qnic_id"], 15);
}

TEST(ClauseTest, FidelityConditionClause_serialize_json) {
  prepareSimulation();
  // num measure
  auto fidelity = std::make_unique<FidelityConditionClause>(0.8, 3, QNIC_RP, 103);
  json fidelity_json = fidelity->serialize_json();
  EXPECT_EQ(fidelity_json["type"], "fidelity");
  EXPECT_EQ(fidelity_json["options"]["required_fidelity"], 0.8);
  EXPECT_EQ(fidelity_json["options"]["partner_address"], 3);
  EXPECT_EQ(fidelity_json["options"]["qnic_type"], 2);
  EXPECT_EQ(fidelity_json["options"]["qnic_id"], 103);
}

}  // namespace