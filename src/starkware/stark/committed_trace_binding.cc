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

using namespace starkware;

extern "C" {
void *runBenchmark(size_t trace_length, size_t blowup) {
    //std::cout << "runBenchmark(" << trace_length << ", " << blowup << ")" << std::endl;
    
    // Config
    bool eval_in_natural_order = true;
    size_t n_columns = 1;

    // Prover setup
    const EvaluationDomain evaluation_domain(trace_length, blowup);
    Prng prng;
    ProverChannel prover_channel(ProverChannel(prng.Clone()));

    TableProverFactory<BaseFieldElement> table_prover_factory =
        GetTableProverFactory<BaseFieldElement>(&prover_channel);

    // Random trace
    std::vector<std::vector<BaseFieldElement>> trace_columns;
    trace_columns.reserve(n_columns);
    for (size_t i = 0; i < n_columns; ++i) {
        trace_columns.emplace_back(prng.RandomFieldElementVector<BaseFieldElement>(trace_length));
    }

    // Commit
    CommittedTraceProver<BaseFieldElement> prover(UseOwned(&evaluation_domain), n_columns, table_prover_factory);
    prover.Commit(
        TraceBase<BaseFieldElement>(std::move(trace_columns)), evaluation_domain.TraceDomain(),
        eval_in_natural_order);
}
}