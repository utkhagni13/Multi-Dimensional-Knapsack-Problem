# Multi-Dimensional-Knapsack-Problem
A simple Knapsack problem consists of items that have only one weight and one value/price/reward associated to them. It can be solved using deterministic algorithms such as dynamic programming(DP) or eve greedy in case of fractional knapsack.
But when the number of constraints associated with items are much more than one then using DP in such case will lead to heavy computational complexity and time limit exceeding errors.
\
\
This projects uses a meta-heuristic approach, i.e, Genetic Algorithm(GA) for solving the Multidimensional Knapsack Problem(MKP).
\
The MKP is composed of `n` items and a knapsack with `m` different capacities c<sub>i</sub> (where, i ε 1,..., m). Each item `j` (where, j ε 1,..., n)
has a weight w<sub>ij</sub> on each capacity `i` of the knapsack and a value p<sub>j</sub>. The goal is to pack the items in the knapsack so as to maximise the overall value without exceeding the capacities of the knapsack.
\
The MKP model can be represented by the following equations:
\
\
**To Maximize**
```math
\sum_{j=1}^n p_j x_j
```
**Subject To**
```math
\left( \sum_{j=1}^n w_{ij} x_j \right) \leq \left( c_i \right)
```
**where**
```math
\left( i \right) \in \left( \{ 1, .... ,m \} \right)
```
```math
\left( j \right) \in \left( \{ 1, .... ,n \} \right)
```
```math
\left( x_j \right) \in \left( \{ 0,1 \} \right)
```
