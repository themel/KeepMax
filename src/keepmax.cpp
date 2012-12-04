#include <float.h>
#include <iostream>
#include <set>
#include <vector>
#include <Rcpp.h>



class KeepMax { 

	unsigned int maxSize_;
	double threshold_; 

	std::multiset<double> kept_;
public:

	KeepMax(unsigned int maxSize) { 
		maxSize_ = maxSize;
		threshold_ = -DBL_MAX;
	}

	void add_one(double val) { 
		// remember value if it's greater than the 
		// current threshold
		if(val > threshold_) { 	
			kept_.insert(val);

			// remove the lowest value and set the
			// threshold to the second-lowest value
			// if we are overfull.
			if(kept_.size() > maxSize_) { 
				kept_.erase(kept_.begin());
				threshold_ = *kept_.begin();
			}
		}
	}


	void add_many(const std::vector<double>& vals) { 
		for(std::vector<double>::const_iterator it = vals.begin(); it != vals.end(); ++it) { 
			add_one(*it);
		} 
	}

	Rcpp::NumericVector get_vector() { 
		return Rcpp::wrap(std::vector<double>(kept_.begin(), kept_.end())); 
	} 
};


using namespace Rcpp;

RCPP_MODULE(KeepMax) { 

	class_<KeepMax>("KeepMax")

	.constructor<unsigned int>()

	.property("vector", &KeepMax::get_vector)

	.method("addOne", &KeepMax::add_one)
	.method("addMany", &KeepMax::add_many)

	;
} 
