function jd = sg2_ms_to_jd(ms)
% function jd = sg2_ms_to_jd(ms)
% Transformation of a 1-D array of Julian Date (jd) of size (M,1) into a
% 1-D int64 array of size (M,1) of milliseconds since 1970-01-01T00:00:00Z
%
% Input:
%
% ms: N-D int64 array
%
% Output:
%
% jd: N-D array of double with the same size of ms

if (~isa(ms,'int64'))
    error('input ms must be of type int64');
end

jd0 = 2440587.5;

shape = size(ms);
ms = ms(:);
ms_notnan = (ms~=int64(-2^63));

jd = nan(size(ms));
jd(ms_notnan) = jd0 + double(ms(ms_notnan))/86400000.0;

jd = reshape(jd, shape);

