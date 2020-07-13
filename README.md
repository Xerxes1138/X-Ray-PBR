# X-Ray PBR

An attempt at converting X-Ray 1.0 engine (SoC) to PBR(ish).

# FEATURES
* SSAO (Crytek)
* Stochastic Screen Space Reflection
* Screen Space SubSurface Scattering
* Temporal AA
* Motion Vector (Camera Only)
* Contact Shadows
* Tonemap (ACES or Hable)

# TODO
* Bloom
* G-Buffer optimization
* GTAO
* Per object motion vector
* Fix particle fading
* More ?

# KNOWN ISSUES
- MANY :) !
- Temporal AA ghosting with moving objects
- Depth issues when camera intersect walls or collide with invisible trigger objects

