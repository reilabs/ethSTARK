//
// Created by Marcin Kostrzewa on 08/05/2023.
//
#include "starkware/stark/committed_trace.h"
#include "starkware/algebra/fields/extension_field_element.h"
#include "starkware/channel/prover_channel.h"
#include "starkware/commitment_scheme/table_verifier.h"
#include "starkware/commitment_scheme/table_verifier_impl.h"
#include "starkware/stark/utils.h"
#include <iostream>

extern "C" {
void *doTheDooblyDoop(size_t trace_length, size_t blowup) {
    std::cout << "doTheDooblyDoop(" << trace_length << ", " << blowup << ")" << std::endl;
    const starkware::EvaluationDomain evaluation_domain(trace_length, blowup);
    starkware::ProverChannel prover_channel;
    starkware::TableProverFactory<starkware::BaseFieldElement> table_prover_factory =
            starkware::GetTableProverFactory<starkware::BaseFieldElement>(&prover_channel);
    return new starkware::CommittedTraceProver<starkware::BaseFieldElement>(UseOwned(&evaluation_domain), 1, table_prover_factory);
}

}