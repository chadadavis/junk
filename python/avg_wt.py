#!/usr/bin/env python

def avg_wt(vals: list, /, weights: list = []):
    '''Weighted average'''

    weights = weights or [1] * len(vals)
    if len(vals) != len(weights):
        raise ValueError(f"Inputs lists must be of the same length")
    weight = sum(weights)
    prod = [val * weight for val, weight in zip(vals, weights)]
    avg = sum(prod) / sum(weights)
    return avg
