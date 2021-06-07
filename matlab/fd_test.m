% File: fd_test.m
% Author: Urs Hofmann
% Mail: hofmannu@ethz.ch
% Date: 19.05.2021

% Description: performs a frequency domain delay and sum reconstruction procedure based
% on the virtual detector concept

clear all;

filePath = '/mnt/hofmannu/elements/aroam/11_HybridSystem/2021_04_28_sphereMeasurements/mid_sphere/';
fileName = 'sphereStack_ar_532_preproc.mat';
load([filePath, fileName], 'vol', 'origin', 'dr');

SOS = 1495;
fd = 8e-3;
tCenter = fd / SOS;

tVec = origin(1) + dr(1) * (1:size(vol, 1));
[~, focIdx] = min(abs(tVec - tCenter));
rangeIdx = min([focIdx-1, size(vol, 1) - focIdx]);
vol = vol(focIdx-rangeIdx:focIdx+rangeIdx, :, :);

volFft = fftshift(fftn(vol));

Fst = 1 / dr(1);
Fsx = 1 / dr(2);
Fsy = 1 / dr(3);
Fsz = 1 / (dr(1) * SOS);

[Lt, Lx, Ly] = size(vol);

% define vectors
kx = Fsx * ((-(Lx - 1)/2):((Lx - 1)/2)) / Lx;
ky = Fsy * ((-(Ly - 1)/2):((Ly - 1)/2)) / Ly;
kt = Fst * ((-(Lt - 1)/2):((Lt - 1)/2)) / Lt;
kz = Fsz * ((-(Lt - 1)/2):((Lt - 1)/2)) / Lt;

volOutFft = zeros(size(volFft));

kx2 = kx.^2;
ky2 = ky.^2;
kz2 = kz.^2;

for (iZ=1:Lt)
	for (iX=1:Lx)
		for (iY=1:Ly)
			ktCurr = -SOS * sqrt(kx2(iX) + ky2(iY) + kz2(iZ));
			volOutFft(iZ, iX, iY) = interp1(kt, squeeze(volFft(:, iX, iY)), ktCurr, 'linear', 0);
		end
	end
end

volOut = ifftn(ifftshift(volOutFft));

figure()
subplot(2, 1, 1)
imagesc(max(abs(vol), [], 3));

subplot(2, 1, 2)
imagesc(max(abs(volOut), [], 3));

colormap(bone(1024));