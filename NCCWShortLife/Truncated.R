 library(msm) ## for dtnorm 

x <- seq(50, 90, by=1)
plot(x, dnorm(x, 70, 10), type="l", ylim=c(0,0.06)) ## standard Normal distribution
lines(x, ptnorm(x, 70, 10, 60, 80), type="l")       ## truncated Normal distribution