#pragma once 

#include <ctime>
#include <vector>
#include "neuron.h"


class Layer {

  public :
    std::vector<Neuron> neurons;

		Layer(int nb_neuron) : neurons(nb_neuron) {}
		Layer(int nb_neuron, int nb_weight) : neurons(nb_neuron, Neuron(nb_weight)) {}

		void randomize(){
			for(auto& neuron : neurons) {
				// Sets weights from -0.5 to 0.5.
				neuron.randomize();
			};
		}
};
