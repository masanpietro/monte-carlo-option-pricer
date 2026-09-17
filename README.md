# monte-carlo-option-pricer
# Monte Carlo Option Pricer (C++)

A from-scratch Monte Carlo engine for pricing options under geometric Brownian
motion, written by hand in C++ to build a deep, first-principles understanding
of derivatives pricing and numerical methods. Every component — the GBM path
simulation, the risk-neutral valuation, and the variance-reduction — was
implemented directly rather than pulled from a quant library.

> **Status: actively under development.** The European vanilla pricer is complete
> and validated against the closed-form Black-Scholes price. Path-dependent
> options and additional variance-reduction techniques are in progress (see
> [Roadmap](#roadmap)).

## Why I built this

I wanted to understand option pricing from the ground up — not just call a
library, but implement the mathematics myself and confirm it against a known
analytical benchmark. Building it by hand forced me to work through the pieces
that matter: the Itô correction in the GBM solution, why the drift is the
risk-free rate under the risk-neutral measure, discounting, and how variance
reduction improves convergence without simply throwing more samples at the
problem.

## What it does

- Prices **European call options** via Monte Carlo simulation of terminal
  asset prices under risk-neutral geometric Brownian motion.
- Uses the exact GBM solution
  `S_T = S_0 · exp((r − σ²/2)·T + σ·√T·Z)`, with `Z ~ N(0,1)` drawn from
  C++'s `<random>` (`std::mt19937` + `std::normal_distribution`).
- Applies **antithetic variates** for variance reduction: each standard-normal
  draw `Z` is paired with its mirror `−Z`, exploiting the symmetry of the normal
  distribution to lower the estimator's variance at no extra sampling cost.
- Discounts the average payoff to present value at the risk-free rate.

## Validation

The Monte Carlo price is checked against the **closed-form Black-Scholes price**
for the same parameters. The two agree to within Monte Carlo sampling error, and
the difference shrinks as the number of simulations increases — consistent with
the theoretical `O(1/√N)` convergence rate of Monte Carlo estimators. (A price
that matched *exactly* would be a red flag; the small, shrinking residual is the
signature of a correct, unbiased estimator.)

## Key concepts implemented

- **Risk-neutral valuation** — the drift is the risk-free rate `r`, not the
  asset's real-world expected return; the real-world drift drops out under
  no-arbitrage pricing.
- **The Itô / volatility-drag correction** — the `−σ²/2` term in the GBM
  exponent.
- **Variance reduction** — antithetic variates, chosen because it reduces
  estimator variance while leaving the expected value (the price) unchanged.
- **Monte Carlo convergence** — error scales as `1/√N` (central limit theorem).

## Build & run

```bash
g++ -std=c++17 -O2 MonteCarlo.cpp -o mc_pricer
./mc_pricer
```

(Requires a C++17-capable compiler for `<random>` and modern features.)

## Roadmap

Actively working on:

- [ ] **Path-dependent options** (Asian, barrier) — these require simulating full
      price paths rather than jumping to the terminal price, and have no
      closed-form Black-Scholes solution, so Monte Carlo is the natural (and
      sometimes only) tool.
- [ ] **Control variates** — a second variance-reduction technique, using a
      quantity with a known expectation to correct the estimate.
- [ ] **Greeks via Monte Carlo** — estimating delta, vega, etc. by
      finite-difference bumping and/or pathwise methods.
- [ ] **Put options** and **put-call parity** as an additional validation check.
- [ ] Refactor into a class-based structure (`Option`, `PricingEngine`) with
      separate header/implementation files.

## Notes

This is a personal learning project, written by hand and iteratively refined.
The emphasis is on correctness, transparency, and understanding the underlying
mathematics rather than on production performance. Feedback and suggestions are
welcome.
