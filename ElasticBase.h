// $Id$
//==============================================================================
//!
//! \file ElasticBase.h
//!
//! \date Jul 04 2014
//!
//! \author Knut Morten Okstad / SINTEF
//!
//! \brief Base class representing FEM integrands for elasticity problems.
//!
//==============================================================================

#ifndef _ELASTIC_BASE_H
#define _ELASTIC_BASE_H

#include "IntegrandBase.h"
#include "Vec3.h"
#include "BDF.h"


/*!
  \brief Base class representing the FEM integrand of elasticity problems.
*/

class ElasticBase : public IntegrandBase
{
protected:
  //! \brief The default constructor is protected to allow sub-classes only.
  ElasticBase();

public:
  //! \brief Empty destructor.
  virtual ~ElasticBase() {}

  //! \brief Defines the gravitation vector.
  void setGravity(const Vec3& g) { gravity = g; }
  //! \brief Defines the gravitation vector.
  void setGravity(double gx, double gy = 0.0, double gz = 0.0)
  { gravity.x = gx; gravity.y = gy; gravity.z = gz; }

  //! \brief Defines the number solution vectors.
  void setNoSolutions(size_t n) { nSV = n; }

  //! \brief Defines the solution mode before the element assembly is started.
  //! \param[in] mode The solution mode to use
  virtual void setMode(SIM::SolutionMode mode);

  //! \brief Initializes an integration parameter for the integrand.
  //! \param[in] i Index of the integration parameter to define
  //! \param[in] prm The parameter value to assign
  virtual void setIntegrationPrm(unsigned short int i, double prm);
  //! \brief Returns an integration parameter for the integrand.
  //! \param[in] i Index of the integration parameter to return
  virtual double getIntegrationPrm(unsigned short int i) const;

  //! \brief Advances the BDF time step scheme one step forward.
  void advanceStep(double dt, double dtn) { bdf.advanceStep(dt,dtn); }

  //! \brief Returns whether this integrand has explicit boundary contributions.
  virtual bool hasBoundaryTerms() const { return false; }

  //! \brief Returns the number of primary/secondary solution field components.
  //! \param[in] fld which field set to consider (1=primary, 2=secondary)
  virtual size_t getNoFields(int fld = 2) const { return fld == 1 ? npv : 0; }
  //! \brief Returns the name of a primary solution field component.
  //! \param[in] i Field component index
  //! \param[in] prefix Name prefix for all components
  virtual std::string getField1Name(size_t i, const char* prefix = NULL) const;

  using IntegrandBase::finalizeElement;
  //! \brief Finalizes the element matrices after the numerical integration.
  //! \param elmInt The local integral object to receive the contributions
  //! \param[in] time Parameters for nonlinear and time-dependent simulations
  //!
  //! \details This method is used to pass time step size parameters to the
  //! integrand in case of a dynamics simulation, where it is needed to compute
  //! the effective stiffness matrix used in the Newton iterations.
  virtual bool finalizeElement(LocalIntegral& elmInt,
                               const TimeDomain& time, size_t);

protected:
  Vec3 gravity; //!< Gravitation vector

  // Finite element quantities, i.e., indices into element matrices and vectors.
  // These indices will be identical for all elements in a model and can thus
  // be stored here, even when doing multi-threading. Note that the indices are
  // 1-based, since the value zero is used to signal non-existing matrix/vector.
  unsigned short int eKm; //!< Index to element material stiffness matrix
  unsigned short int eKg; //!< Index to element geometric stiffness matrix
  unsigned short int eM;  //!< Index to element mass matrix
  unsigned short int eS;  //!< Index to element load vector
  unsigned short int iS;  //!< Index to element internal force vector
  unsigned short int nSV; //!< Number of consequtive solution vectors in core

  TimeIntegration::BDFD2 bdf; //!< BDF time discretization parameters

  double intPrm[5]; //!< Newmark time integration parameters
};

#endif
