/*****************************************************************************
*
* libdiffpy         by DANSE Diffraction group
*                   Simon J. L. Billinge
*                   (c) 2009 The Trustees of Columbia University
*                   in the City of New York.  All rights reserved.
*
* File coded by:    Pavol Juhas
*
* See AUTHORS.txt for a list of people who contributed.
* See LICENSE_DANSE.txt for license information.
*
******************************************************************************
*
* class PeakWidthModel -- base class for calculation of peak widths.
*     The calculate function takes a BondGenerator instance and
*     returns full width at half maximum, based on peak model parameters
*     and anisotropic displacement parameters of atoms in the pair.
*
* class PeakWidthModelOwner -- to be used as a base class for classes
*     that own PeakWidthModel
*
*****************************************************************************/

#ifndef PEAKWIDTHMODEL_HPP_INCLUDED
#define PEAKWIDTHMODEL_HPP_INCLUDED

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/split_free.hpp>

#include <diffpy/Attributes.hpp>
#include <diffpy/HasClassRegistry.hpp>
#include <diffpy/EventTicker.hpp>
#include <diffpy/srreal/BaseBondGenerator.hpp>

namespace diffpy {
namespace srreal {

class PeakWidthModel :
    public diffpy::Attributes,
    public diffpy::HasClassRegistry<PeakWidthModel>
{
    public:

        // methods
        virtual double calculate(const BaseBondGenerator&) const = 0;
        virtual double maxWidth(StructureAdapterPtr,
                double rmin, double rmax) const = 0;
        virtual eventticker::EventTicker& ticker() const  { return mticker; }

    protected:

        // data
        mutable eventticker::EventTicker mticker;

};


typedef PeakWidthModel::SharedPtr PeakWidthModelPtr;


class PeakWidthModelOwner
{
    public:

        // PDF peak width configuration
        void setPeakWidthModel(PeakWidthModelPtr);
        void setPeakWidthModelByType(const std::string& tp);
        PeakWidthModelPtr& getPeakWidthModel();
        const PeakWidthModelPtr& getPeakWidthModel() const;
        eventticker::EventTicker& ticker() const;

    private:

        // data
        PeakWidthModelPtr mpwmodel;
        mutable eventticker::EventTicker mprivateticker;

        // serialization
        friend class boost::serialization::access;

        template<class Archive>
            void serialize(Archive& ar, const unsigned int version)
        {
            ar & mpwmodel & mprivateticker;
        }

};

}   // namespace srreal
}   // namespace diffpy

// Serialization -------------------------------------------------------------

namespace boost {
namespace serialization {

template<class Archive>
void save(Archive& ar,
        const diffpy::srreal::PeakWidthModelPtr& ptr,
        const unsigned int version)
{
    using namespace diffpy::attributes;
    std::string tp;
    AttributesDataMap dt;
    diffpy::eventticker::EventTicker tc;
    if (ptr.get())
    {
        tp = ptr->type();
        dt = saveAttributesData(*ptr);
        tc = ptr->ticker();
    }
    ar & tp & dt & tc;
}


template<class Archive>
void load(Archive& ar,
        diffpy::srreal::PeakWidthModelPtr& ptr,
        const unsigned int version)
{
    using namespace diffpy::attributes;
    using namespace diffpy::srreal;
    std::string tp;
    AttributesDataMap dt;
    diffpy::eventticker::EventTicker tc;
    ar & tp & dt & tc;
    if (!tp.empty())
    {
        ptr = PeakWidthModel::createByType(tp);
        loadAttributesData(*ptr, dt);
        ptr->ticker() = tc;
    }
    else  ptr.reset();
}

}   // namespace serialization
}   // namespace boost

BOOST_SERIALIZATION_SPLIT_FREE(diffpy::srreal::PeakWidthModelPtr)

#endif  // PEAKWIDTHMODEL_HPP_INCLUDED
