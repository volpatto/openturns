#! /usr/bin/env python

from __future__ import print_function
import openturns as ot

ot.PlatformInfo.SetNumericalPrecision(3)
distribution = ot.Normal()
sample = distribution.getSample(1000)
factory = ot.MaximumLikelihoodFactory(distribution)
distribution = factory.build(sample)
print(distribution)

ot.PlatformInfo.SetNumericalPrecision(3)
inner_factory = ot.NormalFactory()
factory = ot.MaximumLikelihoodFactory(inner_factory)
distribution = factory.build(sample)
print(distribution)
