//                                               -*- C++ -*-
/**
 * @brief The class that implements the composition between numerical
 *        math functions implementations
 *
 * Copyright 2005-2017 Airbus-EDF-IMACS-Phimeca
 *
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 *
 */

#include "openturns/ComposedEvaluation.hxx"
#include "openturns/PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ComposedEvaluation);

static const Factory<ComposedEvaluation> Factory_ComposedEvaluation;

/* Default constructor */
ComposedEvaluation::ComposedEvaluation(const EvaluationPointer & p_leftFunction,
    const EvaluationPointer & p_rightFunction)
  : EvaluationImplementation()
  , p_leftFunction_(p_leftFunction)
  , p_rightFunction_(p_rightFunction)
{
  // Check if the dimensions of the left and right functions are compatible
  if (p_leftFunction->getInputDimension() != p_rightFunction->getOutputDimension()) throw InvalidArgumentException(HERE) << "The input dimension=" << p_leftFunction->getInputDimension() << " of the left function must be equal to the output dimension=" << p_rightFunction->getOutputDimension() << " of the right function to compose them";
  setInputDescription(p_rightFunction->getInputDescription());
  setOutputDescription(p_leftFunction->getOutputDescription());
}

/* Virtual constructor */
ComposedEvaluation * ComposedEvaluation::clone() const
{
  return new ComposedEvaluation(*this);
}

/* Comparison operator */
Bool ComposedEvaluation::operator ==(const ComposedEvaluation & other) const
{
  return true;
}

/* Get the i-th marginal function */
ComposedEvaluation::Implementation ComposedEvaluation::getMarginal(const UnsignedInteger i) const
{
  if (i >= getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the index of a marginal function must be in the range [0, outputDimension-1]";
  return new ComposedEvaluation(p_leftFunction_->getMarginal(i), p_rightFunction_);
}

/* Get the function corresponding to indices components */
ComposedEvaluation::Implementation ComposedEvaluation::getMarginal(const Indices & indices) const
{
  if (!indices.check(getOutputDimension())) throw InvalidArgumentException(HERE) << "The indices of a marginal function must be in the range [0, dim-1] and must be different";
  return new ComposedEvaluation(p_leftFunction_->getMarginal(indices), p_rightFunction_);
}


/* String converter */
String ComposedEvaluation::__repr__() const
{
  OSS oss(true);
  oss << "class=" << ComposedEvaluation::GetClassName()
      << " name=" << getName()
      << " leftFunction=" << p_leftFunction_->__repr__()
      << " rightFunction=" << p_rightFunction_->__repr__();
  return oss;
}

String ComposedEvaluation::__str__(const String & offset) const
{
  OSS oss(false);
  if (hasVisibleName()) oss << offset << "name=" << getName() << "\n";
  oss << "(" << p_leftFunction_->__str__(offset) << ")o(";
  oss << p_rightFunction_->__str__(offset) << ")";
  return oss;
}

/* Operator () */
NumericalPoint ComposedEvaluation::operator() (const NumericalPoint & inP) const
{
  if (inP.getDimension() != getInputDimension()) throw InvalidArgumentException(HERE) << "Error: trying to evaluate a NumericalMathFunction with an argument of invalid dimension";
  ++callsNumber_;
  const NumericalPoint rightValue(p_rightFunction_->operator()(inP));
  const NumericalPoint leftValue(p_leftFunction_->operator()(rightValue));
  if (isHistoryEnabled_)
  {
    inputStrategy_.store(inP);
    outputStrategy_.store(leftValue);
  }
  return leftValue;
}

/* Operator () */
NumericalSample ComposedEvaluation::operator() (const NumericalSample & inSample) const
{
  callsNumber_ += inSample.getSize();
  const NumericalSample rightSample(p_rightFunction_->operator()(inSample));
  NumericalSample leftSample(p_leftFunction_->operator()(rightSample));
  leftSample.setDescription(getOutputDescription());
  if (isHistoryEnabled_)
  {
    inputStrategy_.store(inSample);
    outputStrategy_.store(leftSample);
  }
  return leftSample;
}

/* Parameters value accessor */
NumericalPoint ComposedEvaluation::getParameter() const
{
  NumericalPoint parameter(p_rightFunction_->getParameter());
  parameter.add(p_leftFunction_->getParameter());
  return parameter;
}

void ComposedEvaluation::setParameter(const NumericalPoint & parameter)
{
  NumericalPoint rightParameter(p_rightFunction_->getParameter());
  const UnsignedInteger rightDimension = rightParameter.getDimension();
  NumericalPoint leftParameter(p_leftFunction_->getParameter());
  const UnsignedInteger leftDimension = leftParameter.getDimension();
  UnsignedInteger index = 0;
  for (UnsignedInteger i = 0; i < rightDimension; ++ i)
  {
    rightParameter[i] = parameter[index];
    ++ index;
  }
  p_rightFunction_->setParameter(rightParameter);
  for (UnsignedInteger i = 0; i < leftDimension; ++ i)
  {
    leftParameter[i] = parameter[index];
    ++ index;
  }
  p_leftFunction_->setParameter(leftParameter);
}

/* Parameters description accessor */
Description ComposedEvaluation::getParameterDescription() const
{
  Description description(p_rightFunction_->getParameterDescription());
  description.add(p_leftFunction_->getParameterDescription());
  return description;
}

void ComposedEvaluation::setParameterDescription(const Description & description)
{
  Description rightDescription(p_rightFunction_->getParameterDescription());
  const UnsignedInteger rightDimension = rightDescription.getSize();
  Description leftDescription(p_leftFunction_->getParameterDescription());
  const UnsignedInteger leftDimension = leftDescription.getSize();
  UnsignedInteger index = 0;
  for (UnsignedInteger i = 0; i < rightDimension; ++ i)
  {
    rightDescription[i] = description[index];
    ++ index;
  }
  p_rightFunction_->setParameterDescription(rightDescription);
  for (UnsignedInteger i = 0; i < leftDimension; ++ i)
  {
    leftDescription[i] = description[index];
    ++ index;
  }
  p_leftFunction_->setParameterDescription(leftDescription);
}

/* Accessor for input point dimension */
UnsignedInteger ComposedEvaluation::getInputDimension() const
{
  return p_rightFunction_->getInputDimension();
}

/* Accessor for output point dimension */
UnsignedInteger ComposedEvaluation::getOutputDimension() const
{
  return p_leftFunction_->getOutputDimension();
}

/* Method save() stores the object through the StorageManager */
void ComposedEvaluation::save(Advocate & adv) const
{
  EvaluationImplementation::save(adv);
  adv.saveAttribute( "leftFunction_", *p_leftFunction_ );
  adv.saveAttribute( "rightFunction_", *p_rightFunction_ );
}

/* Method load() reloads the object from the StorageManager */
void ComposedEvaluation::load(Advocate & adv)
{
  TypedInterfaceObject<EvaluationImplementation> evaluationValue;
  EvaluationImplementation::load(adv);
  adv.loadAttribute( "leftFunction_", evaluationValue );
  p_leftFunction_ = evaluationValue.getImplementation();
  adv.loadAttribute( "rightFunction_", evaluationValue );
  p_rightFunction_ = evaluationValue.getImplementation();
}

/* Composed implementation accessor */
ComposedEvaluation::EvaluationPointer ComposedEvaluation::getLeftEvaluationImplementation() const
{
  return p_leftFunction_;
}

ComposedEvaluation::EvaluationPointer ComposedEvaluation::getRightEvaluationImplementation() const
{
  return p_rightFunction_;
}

END_NAMESPACE_OPENTURNS
