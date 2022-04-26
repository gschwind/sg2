% function jd = sg2_ymdh_to_jd(ymdh)
%
% Transformation of a 2-D array of size (M,4) (year month day hour) into
% a 1-D array of size (M,1) of julian dates
%
% Input:
%
% ymdh: 2-D array of size (M,4) (y,m,d,h) where:
%        - y is an array of year number
%        - m is an array of month number in [1,12]
%        - d is an array of day of month in [1,31]
%        - h is an array of decimal hour within the day, ex. 12:30 is 12.5
%
% Output:
%
% jd: 1-D array of double of size (M,1)

