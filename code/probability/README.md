# Probability snippets

- dp.hpp: distribution of independent Bernoulli successes and expected cost on a directed acyclic state graph.

Both snippets use bits/stdc++.h and using namespace std; no main or unrelated template. The DAG routine assumes order is a valid topological order and every nonterminal state's outgoing probabilities sum to one. Run tests/probability.cpp after changes.
