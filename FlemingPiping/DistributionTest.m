


expMean = 10;
wblScale = 11.2456; wblShape=2.6957;
lognmlLoc = 2.22838; lognmlScale = 0.385253;
gammaScale = 1.6; gammaShape=6.25;


figure;
hold on;
T = 0:20;

expCDF = expcdf(T, expMean);
wblCDF = wblcdf(T, wblScale, wblShape);
lognmlCDF = logncdf(T, lognmlLoc, lognmlScale);
gammaCDF = gamcdf(T, gammaScale, gammaShape);


plot(T, expCDF, 'b');
plot(T, wblCDF, 'r');
plot(T, lognmlCDF, 'k');
plot(T, gammaCDF, 'g');
