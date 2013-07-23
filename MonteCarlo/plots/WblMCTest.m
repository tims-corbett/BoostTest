
fails = wblrnd(33.2358,3.71391,10^5,1);
reps = exprnd(1,10^5,1);

both = [fails; reps];
both=sort(both);