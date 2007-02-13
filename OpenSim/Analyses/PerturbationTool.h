#ifndef __PerturbationTool_h__
#define __PerturbationTool_h__
// PerturbationTool.h
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <OpenSim/Tools/Object.h>
#include <OpenSim/Tools/PropertyDblArray.h>
#include <OpenSim/Tools/PropertyBool.h>
#include <OpenSim/Tools/PropertyStr.h>
#include <OpenSim/Tools/PropertyInt.h>
#include <OpenSim/Tools/Storage.h>
#include <OpenSim/Simulation/Model/SimulationTool.h>
#include <OpenSim/Simulation/Control/ControlSet.h>
#include "Analyses.h"
#include "suAnalysesDLL.h"
#ifdef SWIG
	#ifdef SUANALYSES_API
		#undef SUANALYSES_API
		#define SUANALYSES_API
	#endif
#endif

namespace OpenSim { 

//=============================================================================
//=============================================================================
/**
 * An abstract class for specifying the interface for an investigation.
 *
 * @author Frank C. Anderson
 * @version 1.0
 */
class SUANALYSES_API PerturbationTool: public SimulationTool
{
//=============================================================================
// MEMBER VARIABLES
//=============================================================================
private:
	// PERTURBATION PARAMETERS
	/** Perturbation time window. */
	PropertyDbl _pertWindowProp;
	double &_pertWindow;
	/** Time increment between perturbation windows. */
	PropertyDbl _pertIncrementProp;
	double &_pertIncrement;
	/** Magnitude of perturbation. */
	PropertyDbl _pertDFProp;
	double &_pertDF;
	/** Name of the controls file. */
	PropertyStr _controlsFileNameProp;
	std::string &_controlsFileName;
	/** Name of the center of pressure file. */
	PropertyStr _copFileNameProp;
	std::string &_copFileName;
	/** Name of the generalized coordinate file. */
	PropertyStr _qFileNameProp;
	std::string &_qFileName;
	/** Name of the generalized speed file. */
	PropertyStr _uFileNameProp;
	std::string &_uFileName;
	/** Name of the states file. */
	PropertyStr _yFileNameProp;
	std::string &_yFileName;

	// FOOT CONTACT EVENT TIMES
	/** Time of right heel strike. */
	PropertyDbl _rHeelStrikeProp;
	double &_rHeelStrike;
	/** Time of right foot flat. */
	PropertyDbl _rFootFlatProp;
	double &_rFootFlat;
	/** Time of right heel off. */
	PropertyDbl _rHeelOffProp;
	double &_rHeelOff;
	/** Time of right toe off. */
	PropertyDbl _rToeOffProp;
	double &_rToeOff;
	/** Time of left heel strike. */
	PropertyDbl _lHeelStrikeProp;
	double &_lHeelStrike;
	/** Time of left foot flat. */
	PropertyDbl _lFootFlatProp;
	double &_lFootFlat;
	/** Time of left heel off. */
	PropertyDbl _lHeelOffProp;
	double &_lHeelOff;
	/** Time of left toe off. */
	PropertyDbl _lToeOffProp;
	double &_lToeOff;

	// CORRECTIVE SPRING PARAMETERS
	/** Rise time for scaling functions. */
	PropertyDbl _tauProp;
	double &_tau;
	/** Stiffness for linear corrective springs. */
	PropertyDblArray _kLinProp;
	Array<double> &_kLin;
	/** Damping for linear corrective springs. */
	PropertyDblArray _bLinProp;
	Array<double> &_bLin;
	/** Stiffness for torsional corrective springs. */
	PropertyDblArray _kTorProp;
	Array<double> &_kTor;
	/** Damping for torsional corrective springs. */
	PropertyDblArray _bTorProp;
	Array<double> &_bTor;

	// EXTERNAL LOAD PARAMETERS
	/** Name of the file containing the external loads applied to the model. */
	OpenSim::PropertyStr _externalLoadsFileNameProp;
	std::string &_externalLoadsFileName;
	/** Name of the file containing the model kinematics corresponding to the
	external loads. */
	OpenSim::PropertyStr _externalLoadsModelKinematicsFileNameProp;
	std::string &_externalLoadsModelKinematicsFileName;
	/** Name of the body to which the first set of external loads should be
	applied (e.g., the body name for the right foot). */
	OpenSim::PropertyStr _externalLoadsBody1Prop;
	std::string &_externalLoadsBody1;
	/** Name of the body to which the second set of external loads should be
	applied (e.g., the body name for the left foot). */
	OpenSim::PropertyStr _externalLoadsBody2Prop;
	std::string &_externalLoadsBody2;
	/** Low-pass cut-off frequency for filtering the model kinematics corresponding
	to the external loads. A negative value results in no filtering.
	The default value is -1.0, so no filtering. */
	OpenSim::PropertyDbl _lowpassCutoffFrequencyForLoadKinematicsProp;
	double &_lowpassCutoffFrequencyForLoadKinematics;

//=============================================================================
// METHODS
//=============================================================================
	//--------------------------------------------------------------------------
	// CONSTRUCTION
	//--------------------------------------------------------------------------
public:
	PerturbationTool();
	PerturbationTool(const std::string &aFileName);
	virtual ~PerturbationTool();
	// Copy constrctor and virtual copy 
	PerturbationTool(const PerturbationTool &aObject);
	virtual Object* copy() const;
private:
	void setNull();
	void setupProperties();
	void constructCorrectiveSprings();

	//--------------------------------------------------------------------------
	// OPERATORS
	//--------------------------------------------------------------------------
public:
#ifndef SWIG
	PerturbationTool&
		operator=(const PerturbationTool &aPerturbationTool);
#endif

	//--------------------------------------------------------------------------
	// GET AND SET
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// INTERFACE
	//--------------------------------------------------------------------------
	virtual void run();
	virtual void printResults(const std::string &aBaseName,const std::string &aDir="",
		double aDT=-1.0,const std::string &aExtension=".sto");

//=============================================================================
};	// END of class PerturbationTool

}; //namespace
//=============================================================================
//=============================================================================

#endif // __PerturbationTool_h__


