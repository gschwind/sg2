% function gamma_S = sg2_topocentric_correction_refraction_ZIM(gamma_S0, P, T)
%
% Application of the atmospheric refraction correction of to a 1-D array of
% solar elevation following the method ZIM
%
% Inputs:
%
% gamma_S0: 1-D array of size (N,1) of solar elevation with no refraction
% correction, in radians
%
% P : 1-D array of size (N,1) of atm. pressure in Pa (mbar)
%
% T : 1-D array of size (N,1) of air temperature in Celsius
%
% Output:
%
% gamma_S : 1-D array of size (N,1) of corrected solar elevation in radians
