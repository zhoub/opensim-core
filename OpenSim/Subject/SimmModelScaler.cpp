// SimmModelScaler.cpp
// Author: Peter Loan
/*
 * Copyright (c) 2006, Stanford University. All rights reserved. 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including 
 * without limitation the rights to use, copy, modify, merge, publish, 
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//=============================================================================
// INCLUDES
//=============================================================================
#include <OpenSim/Tools/ScaleSet.h>
#include "SimmModelScaler.h"
#include <OpenSim/Simulation/SIMM/AbstractModel.h>
#include <OpenSim/Simulation/SIMM/AbstractDynamicsEngine.h>
#include <OpenSim/Simulation/SIMM/SimmMarkerData.h>
#include <OpenSim/Simulation/SIMM/BodySet.h>
#include <OpenSim/Simulation/SIMM/MarkerSet.h>
#include "SimmFileWriter.h"

//=============================================================================
// STATICS
//=============================================================================
using namespace std;
using namespace OpenSim;

//=============================================================================
// CONSTRUCTOR(S) AND DESTRUCTOR
//=============================================================================
//_____________________________________________________________________________
/**
 * Default constructor.
 */
SimmModelScaler::SimmModelScaler() :
	_scalingOrder(_scalingOrderProp.getValueStrArray()),
	_measurementSetProp(PropertyObj("", SimmMeasurementSet())),
	_measurementSet((SimmMeasurementSet&)_measurementSetProp.getValueObj()),
	_scaleSetProp(PropertyObj("", ScaleSet())),
	_scaleSet((ScaleSet&)_scaleSetProp.getValueObj()),
	_markerFileName(_markerFileNameProp.getValueStr()),
	_timeRange(_timeRangeProp.getValueDblArray()),
	_preserveMassDist(_preserveMassDistProp.getValueBool()),
	_outputJointFileName(_outputJointFileNameProp.getValueStr()),
	_outputMuscleFileName(_outputMuscleFileNameProp.getValueStr()),
	_outputModelFileName(_outputModelFileNameProp.getValueStr()),
	_outputScaleFileName(_outputScaleFileNameProp.getValueStr()),
	_maxMarkerMovement(_maxMarkerMovementProp.getValueDbl())
{
	setNull();
	setupProperties();
}

//_____________________________________________________________________________
/**
 * Destructor.
 */
SimmModelScaler::~SimmModelScaler()
{
}

//_____________________________________________________________________________
/**
 * Copy constructor.
 *
 * @param aModelScaler SimmModelScaler to be copied.
 */
SimmModelScaler::SimmModelScaler(const SimmModelScaler &aModelScaler) :
   Object(aModelScaler),
	_scalingOrder(_scalingOrderProp.getValueStrArray()),
	_measurementSetProp(PropertyObj("", SimmMeasurementSet())),
	_measurementSet((SimmMeasurementSet&)_measurementSetProp.getValueObj()),
	_scaleSetProp(PropertyObj("", ScaleSet())),
	_scaleSet((ScaleSet&)_scaleSetProp.getValueObj()),
   _markerFileName(_markerFileNameProp.getValueStr()),
	_timeRange(_timeRangeProp.getValueDblArray()),
	_preserveMassDist(_preserveMassDistProp.getValueBool()),
	_outputJointFileName(_outputJointFileNameProp.getValueStr()),
	_outputMuscleFileName(_outputMuscleFileNameProp.getValueStr()),
	_outputModelFileName(_outputModelFileNameProp.getValueStr()),
	_outputScaleFileName(_outputScaleFileNameProp.getValueStr()),
	_maxMarkerMovement(_maxMarkerMovementProp.getValueDbl())
{
	setNull();
	setupProperties();
	copyData(aModelScaler);
}

//_____________________________________________________________________________
/**
 * Copy this scaling params and return a pointer to the copy.
 * The copy constructor for this class is used.
 *
 * @return Pointer to a copy of this SimmModelScaler.
 */
Object* SimmModelScaler::copy() const
{
	SimmModelScaler *scalingParams = new SimmModelScaler(*this);
	return(scalingParams);
}

//=============================================================================
// CONSTRUCTION
//=============================================================================
//_____________________________________________________________________________
/**
 * Copy data members from one SimmModelScaler to another.
 *
 * @param aModelScaler SimmModelScaler to be copied.
 */
void SimmModelScaler::copyData(const SimmModelScaler &aModelScaler)
{
	_scalingOrder = aModelScaler._scalingOrder;
	_measurementSet = aModelScaler._measurementSet;
	_scaleSet = aModelScaler._scaleSet;
	_markerFileName = aModelScaler._markerFileName;
	_timeRange = aModelScaler._timeRange;
	_preserveMassDist = aModelScaler._preserveMassDist;
	_outputJointFileName = aModelScaler._outputJointFileName;
	_outputMuscleFileName = aModelScaler._outputMuscleFileName;
	_outputModelFileName = aModelScaler._outputModelFileName;
	_outputScaleFileName = aModelScaler._outputScaleFileName;
	_maxMarkerMovement = aModelScaler._maxMarkerMovement;
}

//_____________________________________________________________________________
/**
 * Set the data members of this SimmModelScaler to their null values.
 */
void SimmModelScaler::setNull()
{
	setType("SimmModelScaler");
}

//_____________________________________________________________________________
/**
 * Connect properties to local pointers.
 */
void SimmModelScaler::setupProperties()
{
	_scalingOrderProp.setComment("Specifies the scaling method and order. "
		"Valid options are 'measurements', 'manualScale', singly or both in any sequence.");
	_scalingOrderProp.setName("scaling_order");
	Array<string> sorder("");
	_scalingOrderProp.setValue(sorder);
	_propertySet.append(&_scalingOrderProp);

	_measurementSetProp.setComment("Specifies the measurements by which body segments are to be scaled.");
	_measurementSetProp.setName("SimmMeasurementSet");
	_propertySet.append(&_measurementSetProp);

	_scaleSetProp.setComment("Scale factors to be used for manual scaling.");
	_scaleSetProp.setName("ScaleSet");
	_propertySet.append(&_scaleSetProp);

	_markerFileNameProp.setComment("TRC file (.trc) containing the marker positions used for measurement-based scaling. "
		"This is usually a static trial, but doesn't need to be.  The marker-pair distances are computed for each "
		"time step in the TRC file and averaged across the time range.");
	_markerFileNameProp.setName("marker_file");
	_propertySet.append(&_markerFileNameProp);

	_timeRangeProp.setComment("Time range over which to average marker-pair distances in the marker file (.trc) for "
		"measurement-based scaling.");
	const double defaultTimeRange[] = {-1.0, -1.0};
	_timeRangeProp.setName("time_range");
	_timeRangeProp.setValue(2, defaultTimeRange);
	_propertySet.append(&_timeRangeProp);

	_preserveMassDistProp.setComment("Flag (true or false) indicating whether or not to preserve relative mass between segments.");
	_preserveMassDistProp.setName("preserve_mass_distribution");
	_propertySet.append(&_preserveMassDistProp);

	_outputJointFileNameProp.setComment("Name of SIMM joint file to write when done scaling. "
		"If not specified, a file will not be written.");
	_outputJointFileNameProp.setName("output_joint_file");
	_propertySet.append(&_outputJointFileNameProp);

	_outputMuscleFileNameProp.setComment("Name of SIMM muscle file to write when done scaling. "
		"If not specified, a file will not be written.");
	_outputMuscleFileNameProp.setName("output_muscle_file");
	_propertySet.append(&_outputMuscleFileNameProp);

	_outputModelFileNameProp.setComment("Name of OpenSim model file (.osim) to write when done scaling.");
	_outputModelFileNameProp.setName("output_model_file");
	_propertySet.append(&_outputModelFileNameProp);

	_outputScaleFileNameProp.setComment("Name of file to write containing the scale factors that were applied to the unscaled model (optional).");
	_outputScaleFileNameProp.setName("output_scale_file");
	_propertySet.append(&_outputScaleFileNameProp);

	_maxMarkerMovementProp.setComment("Maximum movement a marker is allowed to move during a static trial. "
		"A negative value, which is the default, means there is no limit.");
	_maxMarkerMovementProp.setName("max_marker_movement");
	_maxMarkerMovementProp.setValue(-1.0); // units of this value are the units of the marker data in the static pose (usually mm)
	_propertySet.append(&_maxMarkerMovementProp);
}

//_____________________________________________________________________________
/**
 * Register the types used by this class.
 */
void SimmModelScaler::registerTypes()
{
	Object::RegisterType(SimmMeasurement());
	//Object::RegisterType(Scale());
	SimmMeasurement::registerTypes();
}

//=============================================================================
// OPERATORS
//=============================================================================
//_____________________________________________________________________________
/**
 * Assignment operator.
 *
 * @return Reference to this object.
 */
SimmModelScaler& SimmModelScaler::operator=(const SimmModelScaler &aModelScaler)
{
	// BASE CLASS
	Object::operator=(aModelScaler);

	copyData(aModelScaler);

	return(*this);
}

//=============================================================================
// UTILITY
//=============================================================================
//_____________________________________________________________________________
/**
 * This method scales a model based on user-specified XYZ body scale factors
 * and/or a set of marker-to-marker distance measurements.
 *
 * @param aModel the model to scale.
 * @param aSubjectMass the final mass of the model after scaling.
 * @return Whether the scaling process was successful or not.
 */
bool SimmModelScaler::processModel(AbstractModel* aModel, const string& aPathToSubject, double aSubjectMass)
{
	int i;
	ScaleSet theScaleSet;
	Array<double> unity(1.0, 3);

	cout << endl << "Step 2: Scaling generic model" << endl;

	const BodySet* bodySet = aModel->getDynamicsEngine().getBodySet();

	/* Make a scale set with an Scale for each body.
	 * Initialize all factors to 1.0.
	 */
	for (i = 0; i < bodySet->getSize(); i++)
	{
		Scale* bodyScale = new Scale();
		bodyScale->setSegmentName(bodySet->get(i)->getName());
		bodyScale->setScaleFactors(unity);
		bodyScale->setApply(true);
		theScaleSet.append(bodyScale);
	}

	try
	{
		/* Make adjustments to theScaleSet, in the user-specified order. */
		for (i = 0; i < _scalingOrder.getSize(); i++)
		{
			/* For measurements, measure the distance between a pair of markers
			 * in the model, and in the static pose. The latter divided by the
			 * former is the scale factor. Put that scale factor in theScaleSet,
			 * using the body/axis names specified in the measurement to
			 * determine in what place[s] to put the factor.
			 */
			if (_scalingOrder[i] == "measurements")
			{
				/* Load the static pose marker file, and convert units.
			    */
				SimmMarkerData markerData(aPathToSubject + _markerFileName);
				markerData.convertToUnits(aModel->getLengthUnits());

				/* Now take and apply the measurements. */
				for (int j = 0; j < _measurementSet.getSize(); j++)
				{
					if (_measurementSet.get(j)->getApply())
					{
						double scaleFactor = computeMeasurementScaleFactor(*aModel, markerData, *_measurementSet.get(j));
						if (scaleFactor != rdMath::NAN) 
							_measurementSet.get(j)->applyScaleFactor(scaleFactor, theScaleSet);
						else
							cout << "___WARNING___: " << _measurementSet.get(j)->getName() << " measurement not used to scale " << aModel->getName() << endl;
					}
				}
			}
			/* For manual scales, just copy the XYZ scale factors from
		  	 * the manual scale into theScaleSet.
	  		 */
			else if (_scalingOrder[i] == "manualScale")
			{
				for (int j = 0; j < _scaleSet.getSize(); j++)
				{
					if (_scaleSet[j]->getApply())
					{
						const string& bodyName = _scaleSet[j]->getSegmentName();
						Array<double> factors(1.0, 3);
						_scaleSet[j]->getScaleFactors(factors);
						for (int k = 0; k < theScaleSet.getSize(); k++)
						{
							if (theScaleSet[k]->getSegmentName() == bodyName)
								theScaleSet[k]->setScaleFactors(factors);
						}
					}
				}
			}
			else
			{
				// TODO: print error
			}
		}

		/* Now scale the model. */
		aModel->scale(theScaleSet, aSubjectMass, _preserveMassDist);

		/* Write output files, if names specified by the user. */
		SimmFileWriter *sfw = new SimmFileWriter(aModel);
		if (sfw)
		{
			if (!_outputJointFileNameProp.getUseDefault())
				sfw->writeJointFile(_outputJointFileName);

			if (!_outputMuscleFileNameProp.getUseDefault())
				sfw->writeMuscleFile(_outputMuscleFileName);

			delete sfw;
		}

		if (!_outputModelFileNameProp.getUseDefault())
		{
			if (aModel->print(_outputModelFileName))
				cout << "Wrote model file " << _outputModelFileName << " from model " << aModel->getName() << endl;
		}

		if (!_outputScaleFileNameProp.getUseDefault())
		{
			if (theScaleSet.print(_outputScaleFileName))
				cout << "Wrote scale file " << _outputScaleFileName << " for model " << aModel->getName() << endl;
		}
	}
	catch (Exception &x)
	{
		x.print(cout);
		return false;
	}

	return true;
}

//_____________________________________________________________________________
/**
 * For measurement based scaling, we average the scale factors across the different marker pairs used.
 * For each marker pair, the scale factor is computed by dividing the average distance between the pair 
 * in the experimental marker data by the distance between the pair on the model.
 */
double SimmModelScaler::computeMeasurementScaleFactor(const AbstractModel& aModel, const SimmMarkerData& aMarkerData, const SimmMeasurement& aMeasurement) const
{
	double scaleFactor = 0;
	cout << "Measurement '" << aMeasurement.getName() << "'" << endl;
	for(int i=0; i<aMeasurement.getNumMarkerPairs(); i++) {
		const SimmMarkerPair& pair = aMeasurement.getMarkerPair(i);
		string name1, name2;
		pair.getMarkerNames(name1, name2);
		double modelLength = takeModelMeasurement(aModel, name1, name2, aMeasurement.getName());
		double experimentalLength = takeExperimentalMarkerMeasurement(aMarkerData, name1, name2, aMeasurement.getName());
		if(modelLength == rdMath::NAN || experimentalLength == rdMath::NAN) return rdMath::NAN;
		cout << "\tpair " << i << " (" << name1 << ", " << name2 << "): model = " << modelLength << ", experimental = " << experimentalLength << endl;
		scaleFactor += experimentalLength / modelLength;
	}
	scaleFactor /= aMeasurement.getNumMarkerPairs();
	cout << "\toverall scale factor = " << scaleFactor << endl;
	return scaleFactor;
}

//_____________________________________________________________________________
/**
 * Measure the distance between two model markers.
 *
 * @return The measured distance.
 */
double SimmModelScaler::takeModelMeasurement(const AbstractModel& aModel, const string& aName1, const string& aName2, const string& aMeasurementName) const
{
	AbstractDynamicsEngine& engine = aModel.getDynamicsEngine();
	const AbstractMarker* marker1 = engine.getMarkerSet()->get(aName1);
	const AbstractMarker* marker2 = engine.getMarkerSet()->get(aName2);

	if (marker1 && marker2) {
		return engine.calcDistance(*marker1->getBody(), marker1->getOffset(), *marker2->getBody(), marker2->getOffset());
	} else {
		if (!marker1)
			cout << "___WARNING___: marker " << aName1 << " in " << aMeasurementName << " measurement not found in " << aModel.getName() << endl;
		if (!marker2)
			cout << "___WARNING___: marker " << aName2 << " in " << aMeasurementName << " measurement not found in " << aModel.getName() << endl;
		return rdMath::NAN;
	}
}

//_____________________________________________________________________________
/**
 * Measure the average distance between a marker pair in an experimental marker data.
 */
double SimmModelScaler::takeExperimentalMarkerMeasurement(const SimmMarkerData& aMarkerData, const string& aName1, const string& aName2, const string& aMeasurementName) const
{
	const Array<string>& experimentalMarkerNames = aMarkerData.getMarkerNames();
	int marker1 = experimentalMarkerNames.findIndex(aName1);
	int marker2 = experimentalMarkerNames.findIndex(aName2);
	if (marker1 >= 0 && marker2 >= 0) {
		int startIndex, endIndex;
		aMarkerData.findFrameRange(_timeRange[0], _timeRange[1], startIndex, endIndex);
		double length = 0;
		for(int i=startIndex; i<=endIndex; i++) {
			double* p1 = aMarkerData.getFrame(i)->getMarker(marker1).get();
			double* p2 = aMarkerData.getFrame(i)->getMarker(marker2).get();
			length += sqrt((p2[0]-p1[0])*(p2[0]-p1[0]) + (p2[1]-p1[1])*(p2[1]-p1[1]) + (p2[2]-p1[2])*(p2[2]-p1[2]));
		}
		return length/(endIndex-startIndex+1);
	} else {
		if (marker1 < 0)
			cout << "___WARNING___: marker " << aName1 << " in " << aMeasurementName << " measurement not found in " << aMarkerData.getFileName() << endl;
		if (marker2 < 0)
			cout << "___WARNING___: marker " << aName2 << " in " << aMeasurementName << " measurement not found in " << aMarkerData.getFileName() << endl;
		return rdMath::NAN;
	}
}

void SimmModelScaler::peteTest() const
{
	int i;
	Array<double> factors(0.0);

	cout << "   SimmModelScaler: " << getName() << endl;
	cout << "      scaling_order: " << _scalingOrder << endl;
	cout << "      measurements (" << _measurementSet.getSize() << "):" << endl;
	for (i = 0; i < _measurementSet.getSize(); i++)
		_measurementSet.get(i)->peteTest();
	cout << "      scales (" << _scaleSet.getSize() << "):" << endl;
	for (i = 0; i < _scaleSet.getSize(); i++)
	{
		_scaleSet[i]->getScaleFactors(factors);
		cout << "      Scale:" << endl;
		cout << "         body: " << _scaleSet[i]->getSegmentName() << endl;
		cout << "         scale factors: " << factors << endl;
		cout << "         apply: " << _scaleSet[i]->getApply() << endl;
	}
	cout << "      markerFileName: " << _markerFileName << endl;
	cout << "      timeRange: " << _timeRange << endl;
	cout << "      preserveMassDist: " << _preserveMassDist << endl;
	cout << "      outputJointFile: " << _outputJointFileName << endl;
	cout << "      outputMuscleFile: " << _outputMuscleFileName << endl;
	cout << "      outputModelFile: " << _outputModelFileName << endl;
	cout << "      outputScaleFile: " << _outputScaleFileName << endl;
}
