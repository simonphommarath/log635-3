#include <string>
#include <cmath>
#include <cassert>
#include <cstdio>

#include "nn.h"
#include "layer.h"
#include "neuron.h"

double NN::sigmoid(double a) {
	if (a < -45.0) {
		return 0;
	}
	if (a > 45.0) {
		return 1;
	}
	return 1.0 / (1 + exp(-a));
}


NN::NN(int inputs, int nb_hidden_layers, int hidden, int outputs)
	: input_layer(inputs, nb_hidden_layers > 0 ? hidden : outputs ),
		output_layer(outputs)
{
	for(int i =0; i< nb_hidden_layers; i++) {
		int nb_weight;
		if (i == nb_hidden_layers -1) {
			nb_weight = outputs;
		} else {
			nb_weight = hidden;
		}
		hidden_layers.emplace_back(hidden, nb_weight);
	}

	// on va merger les layer dans un vector pour les algos dans le futur
	layers.push_back(&input_layer);
	for(auto& layer : hidden_layers) {
		layers.push_back(&layer);
	}
	layers.push_back(&output_layer);

	randomize();

	sigmoid(0);
}


void NN::randomize() {
	for(auto& layer : layers) {
		layer->randomize();
	}
}



std::vector<double> NN::run(const std::vector<double> *inputs) {

	// On va update les ouput du input layer
	for(int i=0; i< input_layer.neurons.size(); i++){
		input_layer.neurons[i].output = (*inputs)[i];
	}

	// On va update les ouput du hidden layer
	std::vector<double> output;
	for(int i = 0; i < hidden_layers.size(); i++) {
		Layer *layer = &hidden_layers[i];

		for(int j = 0; j < layer->neurons.size(); j++){
			//for(auto& neuron : layer->neurons){
			double sum = layer->neurons[j].bias;

			Layer * previous_layer;
			if (i == 0) {
				previous_layer = &input_layer;
			} else {
				previous_layer = &hidden_layers[i - 1];
			}

			for(auto& previous_neuron : previous_layer->neurons){
				sum += previous_neuron.weights[j] * previous_neuron.output;
			}
			layer->neurons[j].output = sigmoid(sum);
			//output.push_back(neuron.output);
		}
	}

	// On va update les ouput du output layer
	for(int i = 0; i < output_layer.neurons.size(); i++){
		//for (auto& neuron : output_layer.neurons){
		double sum = output_layer.neurons[i].bias;

		Layer * previous_layer;
		if (hidden_layers.empty()) {
			previous_layer = &input_layer;
		} else {
			previous_layer = &(hidden_layers[hidden_layers.size() - 1]);
		}

		for (auto& previous_neuron : previous_layer->neurons){
			sum += previous_neuron.weights[i] * previous_neuron.output;
		}
		output_layer.neurons[i].output = sigmoid(sum);
		output.push_back(output_layer.neurons[i].output);
	}


	return output;
}

void NN::back_propagation(Layer *layer, Layer *previous_layer, const std::vector<double>*  /*desired_outputs*/, double learning_rate){
	for(int i = 0; i < layer->neurons.size(); i++){
		Neuron *neuron = &layer->neurons[i];
		//b = b + [ T - A ]
		neuron->bias += learning_rate * neuron->delta * neuron->bias;
		for (auto& previous_neuron : previous_layer->neurons){
			//W(i) = W(i) + [ T - A ] * P(i)
			previous_neuron.weights[i] += learning_rate * neuron->delta * previous_neuron.output;

		}
	}
}

void NN::train(const std::vector<double> *inputs, const std::vector<double>* desired_outputs, double learning_rate) {
	/* To begin with, we must run the network forward. */
	run(inputs);

	for(int i = 0; i < output_layer.neurons.size(); i++){
		Neuron *output = &output_layer.neurons[i];
		//Delta = out * ( 1 - out) * (desired - out)
		output->delta =  output->output * (1 - output->output ) * ( (*desired_outputs)[i] - output->output);
	}


	// Get the delta value for the hidden layer
	for (int i = 0; i < hidden_layers.size(); i++)
	{
		Layer *layer = &hidden_layers[i];
		Layer *next_layer;
		if (i == hidden_layers.size() - 1) {
			next_layer = &output_layer;
		} else {
			next_layer = &(hidden_layers[i + 1]);
		}

		double error = 0.0;

		for (auto & j : layer->neurons) {

			Neuron *neuron = &j;
			for (int k = 0; k < next_layer->neurons.size(); k++)
			{
				Neuron *next_neuron = &next_layer->neurons[k];
				error += neuron->weights[k] * next_neuron->delta;
			}
			neuron->delta = neuron->output * (1.0 - neuron->output) * error;
		}

	}


	/* Train the outputs. */
	{
		Layer * previous_layer;
		if (hidden_layers.empty()) {
			previous_layer = &input_layer;
		} else {
			previous_layer = &(hidden_layers[hidden_layers.size() - 1]);
		}

		back_propagation(&output_layer, previous_layer, desired_outputs, learning_rate);
	}


	/* Train the hidden layers. */
	for (int i = hidden_layers.size() - 1; i >= 0; i--){
		Layer *layer = &hidden_layers[i];
		Layer *previous_layer;
		if (i == 0) {
			previous_layer = &input_layer;
		} else {
			previous_layer = &(hidden_layers[i - 1]);
		}

		back_propagation(layer, previous_layer, desired_outputs, learning_rate);
	}

}
