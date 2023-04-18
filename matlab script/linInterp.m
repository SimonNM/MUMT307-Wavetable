function output = linInterp(frame, wave, size)

idx = floor(frame);     % lower index
offset = frame - idx;   % ratio between two sample

value0 = wave(idx + 1);
value1 = wave(mod(idx+1,size) + 1);

output = value0 + (value1 - value0) * offset;    % linear interpolation