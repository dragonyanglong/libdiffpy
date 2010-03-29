/*****************************************************************************
*
* diffpy.srreal     by DANSE Diffraction group
*                   Simon J. L. Billinge
*                   (c) 2009 Trustees of the Columbia University
*                   in the City of New York.  All rights reserved.
*
* File coded by:    Pavol Juhas
*
* See AUTHORS.txt for a list of people who contributed.
* See LICENSE.txt for license information.
*
******************************************************************************
*
* class PeakWidthModel -- base class for calculation of peak widths.
*
* $Id$
*
*****************************************************************************/

#include <diffpy/srreal/PeakWidthModel.hpp>
#include <diffpy/ClassRegistry.hpp>

using namespace std;
using diffpy::ClassRegistry;

namespace diffpy {
namespace srreal {

// class PeakWidthModelOwner -------------------------------------------------

void PeakWidthModelOwner::setPeakWidthModel(const PeakWidthModel& pwm)
{
    if (mpwmodel.get() == &pwm)   return;
    mpwmodel.reset(pwm.copy());
}


void PeakWidthModelOwner::setPeakWidthModel(const string& tp)
{
    auto_ptr<PeakWidthModel> ppwm(createPeakWidthModel(tp));
    this->setPeakWidthModel(*ppwm);
}


PeakWidthModel& PeakWidthModelOwner::getPeakWidthModel()
{
    assert(mpwmodel.get());
    return *mpwmodel;
}


const PeakWidthModel& PeakWidthModelOwner::getPeakWidthModel() const
{
    assert(mpwmodel.get());
    return *mpwmodel;
}


// Factory Functions ---------------------------------------------------------

PeakWidthModel* createPeakWidthModel(const string& tp)
{
    return ClassRegistry<PeakWidthModel>::create(tp);
}


bool registerPeakWidthModel(const PeakWidthModel& ref)
{
    return ClassRegistry<PeakWidthModel>::add(ref);
}


bool aliasPeakWidthModel(const string& tp, const string& al)
{
    return ClassRegistry<PeakWidthModel>::alias(tp, al);
}


set<string> getPeakWidthTypes()
{
    return ClassRegistry<PeakWidthModel>::getTypes();
}

}   // namespace srreal
}   // namespace diffpy

// End of file
