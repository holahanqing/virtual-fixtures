#ifndef VIRTUAL_MECHANISM_GMR_H
#define VIRTUAL_MECHANISM_GMR_H

////////// VirtualMechanismInterface
#include <virtual_mechanism/virtual_mechanism_interface.h>

////////// Function Approximator
#include <functionapproximators/FunctionApproximatorGMR.hpp>
#include <functionapproximators/ModelParametersGMR.hpp>
#include <functionapproximators/MetaParametersGMR.hpp>

////////// BOOST
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

////////// Toolbox
#include "toolbox/spline/spline.h"
#include "toolbox/dtw/dtw.h"

namespace virtual_mechanism
{
  
  typedef DmpBbo::FunctionApproximatorGMR fa_t;

template <class VM_t>  
class VirtualMechanismGmr: public VM_t
{
	public:

      VirtualMechanismGmr();
      VirtualMechanismGmr(const std::string file_path);
      VirtualMechanismGmr(const Eigen::MatrixXd& data);
      VirtualMechanismGmr(const fa_t* const fa);
      ~VirtualMechanismGmr();

      virtual VirtualMechanismInterface* Clone();

      virtual double getDistance(const Eigen::VectorXd& pos);
      virtual double getScale(const Eigen::VectorXd& pos, const double convergence_factor = 1.0);
      virtual bool CreateModelFromData(const Eigen::MatrixXd& data);
      virtual bool CreateModelFromFile(const std::string file_path);
      virtual bool SaveModelToFile(const std::string file_path);

      void ComputeStateGivenPhase(const double abscisse_in, Eigen::VectorXd& state_out);
      void AlignAndUpateGuide(const Eigen::MatrixXd& data);
      double ComputeResponsability(const Eigen::MatrixXd& pos);
      double GetResponsability();
      bool ReadConfig();
	  
	protected:
	  
      void TrainModel(const Eigen::MatrixXd& data);
	  virtual void UpdateJacobian();
	  virtual void UpdateState();
	  virtual void ComputeInitialState();
      virtual void ComputeFinalState();

      void UpdateInvCov();
      double ComputeProbability(const Eigen::VectorXd& pos);

      fa_t* fa_; // Function Approximator

	  Eigen::MatrixXd fa_input_;
	  Eigen::MatrixXd fa_output_;
	  Eigen::MatrixXd fa_output_dot_;
	  Eigen::MatrixXd variance_;
	  Eigen::MatrixXd covariance_;
      Eigen::MatrixXd covariance_inv_;
	  Eigen::VectorXd err_;

      int n_gaussians_;
};

template <typename VM_t>
class VirtualMechanismGmrNormalized: public VirtualMechanismGmr<VM_t>
{
    public:

      VirtualMechanismGmrNormalized();
      VirtualMechanismGmrNormalized(const std::string file_path);
      VirtualMechanismGmrNormalized(const Eigen::MatrixXd& data);
      VirtualMechanismGmrNormalized(const fa_t* const fa);

      virtual VirtualMechanismInterface* Clone();

      void ComputeStateGivenPhase(const double phase_in, Eigen::VectorXd& state_out, Eigen::VectorXd& state_out_dot, double& phase_out, double& phase_out_dot);
      void AlignAndUpateGuide(const Eigen::MatrixXd& data);
      bool ReadConfig();
      virtual bool CreateModelFromData(const Eigen::MatrixXd& data);
      virtual bool CreateModelFromFile(const std::string file_path);

    protected:

      void TrainModel(const Eigen::MatrixXd& data);
      virtual void UpdateJacobian();
      virtual void UpdateState();
      virtual void UpdateStateDot();
      void Normalize();

      tk::spline spline_phase_;
      tk::spline spline_phase_inv_;
      std::vector<tk::spline > splines_xyz_;
      bool use_spline_xyz_;
      int n_points_splines_;
      double exec_time_;

      double z_;
      double z_dot_;
      double z_dot_ref_;

      Eigen::MatrixXd Jz_;

      long long loopCnt;
};

}

#endif
