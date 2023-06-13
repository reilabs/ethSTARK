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

void free_obj(void *input) {
    delete input;
}

void *new_input(size_t trace_length) {
    size_t n_columns = 1;
    Prng prng;

    auto trace_columns = new std::vector<std::vector<BaseFieldElement>>();
    trace_columns->reserve(n_columns);
    for (size_t i = 0; i < n_columns; ++i) {
        trace_columns->emplace_back(prng.RandomFieldElementVector<BaseFieldElement>(trace_length));
    }
    return trace_columns;
}

void *new_prover(size_t trace_length, size_t blowup) {
    // Config
    bool eval_in_natural_order = true;
    size_t n_columns = 1;

    // Prover setup
    Prng prng;
    ProverChannel prover_channel(ProverChannel(prng.Clone()));

    TableProverFactory<BaseFieldElement> table_prover_factory =
        GetTableProverFactory<BaseFieldElement>(&prover_channel);

    EvaluationDomain *evaluation_domain = new EvaluationDomain(trace_length, blowup);
    auto prover = new CommittedTraceProver<BaseFieldElement>(UseOwned(evaluation_domain), n_columns, table_prover_factory);
      //std::cout << "Domain ugh Length " << evaluation_domain.TraceSize() << "\n";


    return prover;
}


void run_bench(void *input, void *prover, size_t trace_length, size_t blowup) {
    // Config
    bool eval_in_natural_order = true;

    // Random trace
    auto trace_columns = static_cast<std::vector<std::vector<BaseFieldElement>> *>(input);
    std::vector<std::vector<BaseFieldElement>> trace(trace_columns->begin(), trace_columns->end());

    // Prover
    auto trace_prover = static_cast<CommittedTraceProver<BaseFieldElement> *>(prover);
    EvaluationDomain evaluation_domain(trace_length, blowup);
    auto domain = evaluation_domain.TraceDomain();
    //std::cout << "Domain wtf Length " << domain.TraceSize() << "\n";

    // Commit
    trace_prover->Commit(
        TraceBase<BaseFieldElement>(std::move(trace)),
        evaluation_domain.TraceDomain(),
        eval_in_natural_order
    );
}
}