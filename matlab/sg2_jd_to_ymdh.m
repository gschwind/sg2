% function ymdh = sg2_jd_to_ymdh(jd)
%
% Transformation of a 1-D array of Julian Date (jd) of size (M,1) into a
% 2-D array of size (M,4) with (year month day hour)
%
% Input:
%
% jd: 1-D array of double of size (M,1)
%
% Output:
%
% ymdh: 2-D array of size (M,4) (y,m,d,h) where:
%        - y is an array of year number
%        - m is an array of month number in [1,12]
%        - d is an array of day of month in [1,31]
%        - h is an array of decimal hour within the day, ex. 12:30 is 12.5



