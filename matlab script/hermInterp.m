function output = hermInterp(frame, wave, size)

idx = floor(frame);
offset = frame - idx;

value0 = wave(mod(idx-1,size) + 1);
value1 = wave(idx + 1);
value2 = wave(mod(idx+1,size) + 1);
value3 = wave(mod(idx+2,size) + 1);

slope0 = (value2 - value0) * 0.5;
slope1 = (value3 - value1) * 0.5;

v = value1 - value2;
w = slope0 + v;
a = w + v + slope1;
b_neg = w + a;

stage1 = a * offset - b_neg;
stage2 = stage1 * offset + slope0;
output = stage2 * offset + value1;