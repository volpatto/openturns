//                                               -*- C++ -*-
/**
 *  @brief Factory for Laplace distribution
 *
 *  Copyright 2005-2015 Airbus-EDF-IMACS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OPENTURNS_LAPLACEFACTORY_HXX
#define OPENTURNS_LAPLACEFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionFactoryImplementation.hxx"
#include "Laplace.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LaplaceFactory
 */
class OT_API LaplaceFactory
  : public DistributionFactoryImplementation
{
  CLASSNAME;
public:

  /** Default constructor */
  LaplaceFactory();

  /** Virtual constructor */
  virtual LaplaceFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionFactoryImplementation::build;

  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPoint & parameters) const;
  Implementation build() const;
  Laplace buildAsLaplace(const NumericalSample & sample) const;
  Laplace buildAsLaplace(const NumericalPoint & parameters) const;
  Laplace buildAsLaplace() const;

}; /* class LaplaceFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LAPLACEFACTORY_HXX */