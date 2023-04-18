function output = cubInterp(frame, wave, size)

idx0 = floor(frame-1);
idx1 = floor(frame);
idx2 = floor(frame+1);
idx3 = floor(frame+2);

offset0 = frame - idx0;
offset1 = frame - idx1;
offset2 = frame - idx2;
offset3 = frame - idx3;

value0 = wave(mod(idx0,size) + 1);
value1 = wave(idx1 + 1);
value2 = wave(mod(idx2,size) + 1);
value3 = wave(mod(idx3,size) + 1);

stage0 = -value0/6*offset1*offset2*offset3;
stage1 = stage0 + value1/2*offset0*offset2*offset3;
stage2 = stage1 - value2/2*offset0*offset1*offset3;
output = stage2 + value3/6*offset0*offset1*offset2;