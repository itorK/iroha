/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_PROPOSAL_GATE_HPP
#define IROHA_PROPOSAL_GATE_HPP

#include <boost/variant.hpp>
#include <rxcpp/rx.hpp>
#include "ordering/on_demand_os_transport.hpp"

namespace iroha {
  namespace network {

    struct ProposalVote {
      boost::optional<ordering::transport::OdOsNotification::ProposalType>
          proposal;
      ordering::transport::RoundType round;
    };

    struct ProposalCommit {
      boost::optional<std::shared_ptr<shared_model::interface::Proposal>>
          proposal;
      ordering::transport::RoundType round;
    };

    struct ProposalReject {
      ordering::transport::RoundType round;
    };

    using ProposalOutcomeType = boost::variant<ProposalCommit, ProposalReject>;

    /**
     * Allows to vote for proposals and receive consensus outcomes
     */
    class ProposalGate {
     public:
      /**
       * Vote for proposal
       * @param vote - structure with optional proposal and round number
       */
      virtual void vote(ProposalVote vote) = 0;

      /**
       * Receive consensus outcomes for rounds
       * @return - observable with outcomes, commits and rejects
       */
      virtual rxcpp::observable<ProposalOutcomeType> outcomes() = 0;

      virtual ~ProposalGate() = default;
    };

  }  // namespace network
}  // namespace iroha

#endif  // IROHA_PROPOSAL_GATE_HPP
