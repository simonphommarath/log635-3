#ifndef __NN_H__
#define __NN_H__

#include <stdio.h>
#include <vector>
#include "neuron.h"
#include "layer.h"

class NN {

private :
	Layer input_layer;
	std::vector<Layer> hidden_layers;
	Layer output_layer;

	std::vector<Layer*> layers;

public :
	NN(int inputs, int nb_hidden_layers, int hidden, int outputs);

	void randomize();

	std::vector<double> run(const std::vector<double> * inputs);

	void train(const std::vector<double> *inputs, const std::vector<double> *desired_outputs, double learning_rate);

	void back_propagation(Layer *layer, Layer *previous_layer, const std::vector<double>* desired_outputs, double learning_rate);

	double sigmoid(double a);
};

#endif /*__NN_H__*/
