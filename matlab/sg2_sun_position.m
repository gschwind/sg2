% function out = sg2_sun_rise(geopoints, timestamp, fields)
%
% Computes the Sun-Earth position (geocentric) and the local sun position
% (topocentric)
%
% Inputs:
%
% geopoints : 2-D array of double
%     This is a list of N geopoints as 2-D array of size (N,3) where each
%     row is repectively longitude in degrees, latitude in degrees and
%     altitude in meters.
%
% timstamp: 2-D array of double or 2-D array of int64
%     Array of timestamps at wich the computation will be done. If the 2-D
%     array is of type int64, the timestamps are read as integer numbers of
%     milliseconds since 1970-01-01T00:00:00Z. If the 2-D array if of type
%     double, the timestamps are read as julian date (decimal number of
%     day from Julian Day 0, which began at noon on January 1, 4713 B. C.).
%     Moreover the shape of the array can be (M,1) or (M,2) in the later
%     case the row are respectively a timestamp in UT and the terrestrial
%     time (TT). In the case of (M,1) it must contain only the timestamp
%     in UT, in that case the terrestrial time will be computed
%     with a predefined piecewise polynomial function.
%     The valid time range is between 1950-01-01 and 2100-12-31
%
% fields: 1D Cells of char strings
%     This is the list of requested outputs. See returns value for more details
%
% Output:
%
% out: a structure of fields computed as requested:
%     * "geoc.ut" **timestamp** as array of datetime64[ms]
%     * "geoc.tt" **terrestrial time** as array of datetime64[ms]
%     * "geoc.R" **Radius Sun-Earth** in astronomical unit (au) as array of double
%     * "geoc.L" **Heliocentric Earth true longitude** in radians as array of double
%     * "geoc.delta" **Geocentric declination** in radians as array of double
%     * "geoc.EOT" **Equation of Time: difference between apparent solar time
%                    and mean solar time** in radians as array of double
%                    (to be multiplied by pi/12 for hourly values)
%     * "geoc.Theta_a" **Geocentric Earth true longitude** in radians as array of double
%     * "geoc.epsilon" **Earth true obliquity** in radians as array of double
%     * "geoc.nu" **Apparent sideral time** in radians as array of double
%     * "geoc.r_alpha" **Geocentric right ascension** in radians as array of double
%     * "geoc.Delta_psi" **Nutation in Geocentric Sun longitude** in radians as array of double
%     * "gp.lambda" **Longitude** in radians as array of double
%     * "gp.phi" **Latitude** in radians as array of double
%     * "gp.u" **phi geocentric**
%     * "gp.x"
%     * "gp.y"
%     * "topoc.delta" **Topocentric sun declination** in radians as array of double
%     * "topoc.alpha_S" **Topocentric sun azimuth** in radians as array of double
%     * "topoc.gamma_S0" **Topocentric sun elevation angle without correction
%                          of atm. refraction.** in radians as array of double
%     * "topoc.omega" **Topocentric local hour angle** in radians as array of double
%     * "topoc.r_alpha" **Topocentric right sun ascension** in radians as array of double
%     * "topoc.toa_hi" **Top of atmosphere horizontal irradiance** in W.m^{-2} as array of double
%     * "topoc.toa_ni" **Top of atmosphere normal irradiance** in W.m^{-2} as array of double
%
%     The abreviations geoc and topoc stand for respectively geocentric and topocentric.
%
%     Moreover the size of arrays of geoc.* fields will be (1,M), the size of
%     arrays of gp.* fields will be (N,1) and the size of arrays of topoc.* will
%     be (N,M).
%
%





