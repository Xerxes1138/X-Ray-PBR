# X-Ray PBR

An attempt at converting X-Ray 1.0 engine (SoC) to PBR(ish).

# FEATURES
* SSAO (Crytek) or Raymarched AO
* Stochastic Screen Space Reflection
* Screen Space SubSurface Scattering
* Temporal AA
* Motion Vector (Camera Only)
* Contact Shadows
* Tonemap (ACES or Hable)

# KNOWN ISSUES
- MANY :) !
- Temporal AA ghosting with moving objects
- Depth issues when camera intersect walls or collide with invisible trigger objects

# COMMANDs

Add -sjitter and -temporalaa to your "XR_3DA.exe" shortcut to enable soft shadows and temporalAA.
