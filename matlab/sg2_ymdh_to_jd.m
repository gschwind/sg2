function jd = sg2_ymdh_to_jd(ymdh)
% function jd = sg2_ymdh_to_jd(ymdh)
%
% Transformation of (N+1)-D array with elements (year month day hour)
% into a N-D array of Julian Date (jd)
%
%
% Input:
%
% ymdh: (N+1)-D array of (n1, ..., nN, 4)  with (y,m,d,h) elements where:
%        - y is an array of year number
%        - m is an array of month number in [1,12]
%        - d is an array of day of month in [1,31]
%        - h is an array of decimal hour within the day, ex. 12:30 is 12.5
%
% Output:
%
% jd: N-D array of double of size (n1, ..., nN)

shape = size(ymdh);
if (shape(end)~=4)
    error('The last size of ymdh should be 4');
end

n = prod(shape(1:end-1));
ymdh = reshape(ymdh,[n 4]);

jd = zeros(n,1);

idx = find(ymdh(:,2)<3);
ymdh(idx,2) = ymdh(idx,2)+12;
ymdh(idx,1) = ymdh(idx,1)-1;

jd = 1721028 + ymdh(:,3) + floor((153 * ymdh(:,2) - 2) / 5) + 365 * ymdh(:,1) + floor(ymdh(:,1) / 4) ...
    - floor(ymdh(:,1) / 100) + floor(ymdh(:,1) / 400) + ymdh(:,4) / 24 - 0.5;

jd = reshape(jd, [shape(1:end-1) 1]);




