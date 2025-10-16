/*	NAME:
 *		RaccPPMToRTIWBridge.hpp
 *
 *	DESCRIPTION:
 *		Utilities to work with RaccPPM in the RTIW project.
 *
 *	CONTRIBUTORS:
 *		Racc 29-SEP-25
*/





#ifndef RACCPPM_TO_RTIW_BRIDGE_H
#define RACCPPM_TO_RTIW_BRIDGE_H





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include "ppm.hpp"
#include "color.hpp"
#include "vec3.hpp"





// ===========================================================================
//		RaccPPM to RTIW Bridge
// ---------------------------------------------------------------------------
class RaccPPMToRTIWBridge
{
public:
	static inline RACCPPM::RGBValue ColorToRaccRBG(const RTIW::color& color)
	{
		return RACCPPM::RGBValue{
			static_cast<uint8_t>(color.x() * 255.999),
			static_cast<uint8_t>(color.y() * 255.999),
			static_cast<uint8_t>(color.z() * 255.999)};
	}
};





#endif // RACCPPM_TO_RTIW_BRIDGE_H





