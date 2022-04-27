function ymdh = sg2_jd_to_ymdh(jd)
% function ymdh = sg2_jd_to_ymdh(jd)
%
% Transformation of a N-D array of Julian Date (jd) into a
% (N+1)-D array of size with (year month day hour)
%
% Input:
%
% jd: N-D array of double
%
% Output:
%
% ymdh: (N+1)-D array of size [size(jd) 4] with (y,m,d,h) elements where:
%        - y is an array of year number
%        - m is an array of month number in [1,12]
%        - d is an array of day of month in [1,31]
%        - h is an array of decimal hour within the day, ex. 12:30 is 12.5

shape = size(jd);
jd = jd(:);

H = (jd + 0.5 - floor(jd + 0.5)) * 24;
L = floor(jd + 0.5) + 68569;
N = floor(4 * L / 146097);
L = L - floor((146097 * N + 3) / 4);
I = floor(4000 * (L + 1) / 1461001);
L = L - floor(1461 * I / 4) + 31;

J = floor(80 * L / 2447);
K = L - floor(2447 * J / 80);
L = floor(J / 11);
J = J + 2 - 12 * L;
I = 100 * (N - 49) + I + L;

ymdh = reshape([I J K H],[shape 4]);




