function output = fftbandlimit(fs, f, waveform, size)

nyquist = fs/2;
y = fft(waveform);

maxHarmonic = size/2;
n = 0;
while (maxHarmonic - n)*f > nyquist
    y(maxHarmonic+n+1) = 0;
    y(maxHarmonic-n+1) = 0;
    n = n+1;
end

output = ifft(y);