clear

dh = 0.05;
% dh = 0.1;

% absorp = 1.0;
% absorp = 0.8;
absorp = 0.5;
% absorp = 0.2;

switch dh
    case 0.05
        sr = 16000;
        fcut = 3000;
    case 0.1
        sr = 8000;
        fcut = 1500;
end
        


rr = load([num2str(dh) '_' num2str(absorp)  '/response_0.txt']);
src = load([num2str(dh) '_' num2str(absorp) '/source_0.txt']);
Ns = size(rr,1);

rt =int16(0.16*7600/2720/absorp*sr);
display(['RT60: ' num2str(rt/sr) ' sec.']);
rr(rt+1:end)=0;

rir = xcorr(rr,src(end:-1:1));
rir = rir(1:(end+1)/2);
b = fir1(1024,fcut/(sr/2));
rir = fftfilt(b,rir);
rir = normalizeIR(rir);

figure
subplot(211)
plot(rr)
title('Room response');
subplot(212)
plot(rir)
title('Room impluse response');
% subplot(313)
% figure
% t60(rir,sr,1);

[file,path] = uigetfile({'*.mp3';'*.wav'});
[filepath,name,ext] = fileparts(file);
[x ori_sr] = audioread([path file]);

x = resample(x, sr, ori_sr);

audiowrite([num2str(dh) '_' num2str(absorp)  '/' name '-resample.wav'],x, sr);



% rr = normalizeIR(rr);
% src = normalizeIR(src);

% rtf = fft(rr',Ns)./fft(src',Ns);


% rir = real(ifft(rtf));


y1 = fftfilt(rir,x);
% rtf = fft(rir,Ns);
% figure();
% plot(abs(rtf(1:end/2)));
% plot(rir);



b = fir1(1024,fcut/(sr/2),'high');
y2 = fftfilt(b,x);
y = y1+0.9*y2;
y = normalizeIR(y);

audiowrite([num2str(dh) '_' num2str(absorp)  '/' name '-reverb-' num2str(absorp) '.wav'],y, sr);