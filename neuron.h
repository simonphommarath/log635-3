#pragma once

#include <ctime>
#include <vector>

class Neuron {

  public :
	  std::vector<double> weights;
		double output;
		double delta;
		double bias;

		Neuron() : output(0), delta(0), bias(1){}
		Neuron(int nb_weight): weights(nb_weight, 1), output(0), delta(0), bias(1) {}

		void randomize(){
			bias = ((double)std::rand() / RAND_MAX) - 0.5;
			for(auto& weight : weights) {
				// Sets weights from -0.5 to 0.5.
				weight = ((double)std::rand() / RAND_MAX) - 0.5;
			};
		}
};
