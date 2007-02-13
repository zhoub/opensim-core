// VectorFunction.cpp
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
* Copyright (c) 2005, Stanford University. All rights reserved. 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions
* are met: 
*  - Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer. 
*  - Redistributions in binary form must reproduce the above copyright 
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the distribution. 
*  - Neither the name of the Stanford University nor the names of its 
*    contributors may be used to endorse or promote products derived 
*    from this software without specific prior written permission. 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE. 
*/

/* Note: This code was originally developed by Realistic Dynamics Inc. 
 * Author: Frank C. Anderson 
 */


// INCLUDES
#include "rdMath.h"
#include "VectorFunction.h"
#include "PropertyDbl.h"




using namespace OpenSim;
using namespace std;


//=============================================================================
// DESTRUCTOR AND CONSTRUCTORS
//=============================================================================
//_____________________________________________________________________________
/**
 * Destructor.
 */
VectorFunction::~VectorFunction()
{
}
//_____________________________________________________________________________
/**
 * Default constructor.
 */
VectorFunction::VectorFunction() :
	_minX(0.0), _maxX(0.0)
{
	setNull();
}
//_____________________________________________________________________________
/**
 * Default constructor.
 */
VectorFunction::VectorFunction(int aNX, int aNY) :
	_minX(rdMath::MINUS_INFINITY),
	_maxX(rdMath::PLUS_INFINITY)
{
	setNull();
	setNX(aNX);
	setNY(aNY);
}
//_____________________________________________________________________________
/**
 * Copy constructor.
 *
 * @param aVectorFunction Function to copy.
 */
VectorFunction::VectorFunction(const VectorFunction &aVectorFunction) :
	Object(aVectorFunction),
	_minX(rdMath::MINUS_INFINITY),
	_maxX(rdMath::PLUS_INFINITY)
{
	setNull();

	// ASSIGN
	setEqual(aVectorFunction);
}


//=============================================================================
// CONSTRUCTION
//=============================================================================
//_____________________________________________________________________________
/**
 * Set all member variables to NULL values.
 */
void VectorFunction::
setNull()
{
	setType("VectorFunction");
	_nX = 0;
	_nY = 0;
}

//_____________________________________________________________________________
/**
 * Set all member variables equal to the members of another object.
 * Note that this method is private.  It is only meant for copying the data
 * members defined in this class.  It does not, for example, make any changes
 * to data members of base classes.
 */
void VectorFunction::
setEqual(const VectorFunction &aVectorFunction)
{
	setNX(aVectorFunction.getNX());
	setNY(aVectorFunction.getNY());
	setMinX(aVectorFunction.getMinX());
	setMaxX(aVectorFunction.getMaxX());
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
VectorFunction& VectorFunction::
operator=(const VectorFunction &aVectorFunction)
{
	// BASE CLASS
	Object::operator=(aVectorFunction);

	// DATA
	setEqual(aVectorFunction);

	return(*this);
}


//=============================================================================
// SET AND GET
//=============================================================================
//-----------------------------------------------------------------------------
// NUMBER OF INDEPENDENT AND DEPENDENT VARIABLES
//-----------------------------------------------------------------------------
//_____________________________________________________________________________
/**
 * Set the number of independent variables.
 *
 * @param aNX Number of independent variables.
 */
void VectorFunction::
setNX(int aNX)
{
	_nX = aNX;
}
//_____________________________________________________________________________
/**
 * Get the number of independent variables.
 *
 * @return Number of independent variables.
 */
int VectorFunction::
getNX() const
{
	return(_nX);
}
//_____________________________________________________________________________
/**
 * Set the number of dependent variables.
 *
 * @param aNX Number of dependent variables.
 */
void VectorFunction::
setNY(int aNY)
{
	_nY = aNY;
}
//_____________________________________________________________________________
/**
 * Get the number of dependent variables.
 *
 * @return Number of dependent variables.
 */
int VectorFunction::
getNY() const
{
	return(_nY);
}
//-----------------------------------------------------------------------------
// MIN AND MAX INDPEPENDENT VARIABLES
//-----------------------------------------------------------------------------
//_____________________________________________________________________________
/**
 * Set the minimum values of the independent variables.
 *
 * @param aMinX Array of minimum values of the independent variables.
 */
void VectorFunction::
setMinX(const Array<double> &aMinX)
{
	if(aMinX.getSize()!=_nX) {
		string msg = "VectorFunction.setMinX: ERR- ";
		msg += "Array size does not match number of variables.";
		throw( Exception(msg,__FILE__,__LINE__) );
	}
	_minX = aMinX;
}
//_____________________________________________________________________________
/**
 * Get the minimum values of the independent variables.
 *
 * @return Array of minimum values of the independent variables.
 */
const Array<double>& VectorFunction::
getMinX() const
{
	return(_minX);
}
//_____________________________________________________________________________
/**
 * Set the minimum value of one of the independent variables.
 *
 * @param aXIndex Index of the independent variable value to be set.
 * @param aMinX Value of the independent variable specified by aX.
 */
void VectorFunction::
setMinX(int aXIndex, double aMinX)
{
	_minX.set(aXIndex, aMinX);
}
//_____________________________________________________________________________
/**
 * Get the minimum values of the independent variables.
 * 
 * @param aXIndex Index of the independent variable value to be set.
 * @return Array of minimum values of the independent variables.
 */
double VectorFunction::
getMinX(int aXIndex) const
{
	return(_minX.get(aXIndex));
}
//_____________________________________________________________________________
/**
 * Set the maximum values of the independent variables.
 *
 * @param aMaxX Array of maximum values of the independent variables.
 */
void VectorFunction::
setMaxX(const Array<double> &aMaxX)
{
	if(aMaxX.getSize()!=_nX) {
		string msg = "VectorFunction.setMaxX: ERR- ";
		msg += "Array size does not.match number of variables.";
		throw( Exception(msg,__FILE__,__LINE__) );
	}

	_maxX = aMaxX;}
//_____________________________________________________________________________
/**
 * Get the maximum values of the independent variables.
 *
 * @return Array of maximum values of the independent variables.
 */
const Array<double>& VectorFunction::
getMaxX() const
{
	return(_maxX);
}
//_____________________________________________________________________________
/**
 * Set the maximum value of one of the independent variables.
 *
 * @param aX Index of the independent variable value to be set.
 * @param aMaxX Value of the independent variable specified by aX.
 */
void VectorFunction::
setMaxX(int aXIndex, double aMaxX)
{
	_maxX.set(aXIndex, aMaxX);
}
//_____________________________________________________________________________
/**
 * Get the maximum values of the independent variables.
 *
 * @param aX Index of the independent variable value to be set.
 * @return Array of maximum values of the independent variables.
 */
double VectorFunction::
getMaxX(int aXIndex) const
{
	return(_maxX.get(aXIndex));
}


//_____________________________________________________________________________
/**
 * Update the bounding box for the abscissae.
 *
 * This method should be overrided as needed by derived classes.
 */
void VectorFunction::
updateBoundingBox()
{

}

