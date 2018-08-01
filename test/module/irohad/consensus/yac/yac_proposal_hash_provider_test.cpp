/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "consensus/yac/impl/yac_proposal_hash_provider_impl.hpp"

#include <string>

#include <gtest/gtest.h>
#include "module/shared_model/interface_mocks.hpp"

using namespace iroha::network;
using namespace iroha::consensus::yac;

using ::testing::ReturnRef;

struct YacProposalHashProviderTest : public ::testing::Test {
  void SetUp() override {
    proposal = std::make_unique<MockProposal>();
    EXPECT_CALL(*proposal, blob()).WillRepeatedly(ReturnRef(blob));
    hash = proposal->hash();
  }

  YacProposalHashProviderImpl hash_provider;
  std::unique_ptr<MockProposal> proposal;
  shared_model::interface::types::BlobType blob;
  shared_model::interface::types::HashType hash;
  iroha::ordering::transport::RoundType round{1, 1};
};

/**
 * @given non-empty proposal optional and round number
 * @when proposal, round -> yac hash -> proposal info
 * @then hash and round from info matches given data
 */
TEST_F(YacProposalHashProviderTest, YacHashFromProposal) {
  std::unique_ptr<shared_model::interface::Proposal> iproposal(
      std::move(proposal));
  auto yac_hash =
      hash_provider.makeHash(ProposalVote{std::move(iproposal), round});

  auto model_hash = hash_provider.toModelHash(yac_hash);

  ASSERT_TRUE(model_hash.hash);
  ASSERT_EQ(hash, model_hash.hash.value());
  ASSERT_EQ(round, model_hash.round);
}

/**
 * @given empty proposal optional and round number
 * @when empty proposal, round -> yac hash -> proposal info
 * @then empty hash and round from info matches given data
 */
TEST_F(YacProposalHashProviderTest, YacHashFromNone) {
  auto yac_hash = hash_provider.makeHash(ProposalVote{boost::none, round});

  auto model_hash = hash_provider.toModelHash(yac_hash);

  ASSERT_FALSE(model_hash.hash);
  ASSERT_EQ(round, model_hash.round);
}
