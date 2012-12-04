KeepMax
=======

An R data structure that keeps only the upper tail of its inputs around. 

Examples
========

```

library(KeepMax) 


# get top 20k values from 2m values
vec <- rnorm(2e6)

# all in one vector
top20k.r <- function(vec) vec[order(vec) > (length(vec) - 20000)] 
top20k.km <- function(vec) { 
	km <- new(KeepMax, 20000); 
	km$addMany(vec); 
	km$vector 
}


# adding one by one
top20k.r.one <- function(vec) { 
	# take the first 20k
	keep <- vec[1:20000]; 
	# then 
	for(i in 20001:length(vec)) { 
		# if > the smallest we'd keep so far
		if(vec[i] > keep[1]) { 
			# add and re-sort
			keep <- sort(c(keep, vec[i]))[-1]
		} 
	}
	keep
}

top20k.km.one <- function(vec) { 
	km <- new(KeepMax, 20000); 

	# 10x performance improvement
	func <- km$addOne

	for(i in seq_along(vec)) { 
		func(vec[i]) 
	} 

	km$vector 
}

````

Benchmark
=========

````
library(microbenchmark)

microbenchmark(top20k.r(vec), top20k.km(vec))

microbenchmark(top20k.r.one(vec), top20k.km.one(vec), times=10)
```

yields

```
> benchmark(top20k.r(vec), top20k.km(vec))
Unit: milliseconds
            expr        min         lq     median         uq        max
1 top20k.km(vec)   43.61215   43.86759   46.20854   47.48182   51.71146
2  top20k.r(vec) 1415.76254 1422.43753 1426.78174 1448.90953 1478.22839

benchmark(top20k.r.one(vec), top20k.km.one(vec), times=10)
Unit: seconds
                expr      min       lq   median       uq      max
1 top20k.km.one(vec)  4.76322 4.794606 4.804996 4.812961 4.827485
2  top20k.r.one(vec) 86.87602 86.91814 86.92890 86.97854 87.17521
```

