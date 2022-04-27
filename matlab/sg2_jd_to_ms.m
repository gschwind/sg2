function ms = sg2_jd_to_ms(jd)
% function ms = sg2_ms_to_jd(jd)
% Transformation 1-D int64 array of size (M,1) of milliseconds since
% 1970-01-01T00:00:00Z into a 1-D array of Julian Date (jd) of size (M,1)
%
%
% Input:
%
% jd: N-D array of double
%
% Output:
%
% ms: N-D int64 array with the same size of jd

shape = size(jd);
jd = jd(:);

jd_notnan = ~isnan(jd);

jd0 = 2440587.5;

ms = repmat(int64(-2^63),size(jd));
ms(jd_notnan) = int64((jd(jd_notnan)-jd0)*86400000.0);
ms = reshape(ms, shape);


