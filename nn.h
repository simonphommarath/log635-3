/*
 * NN - Minimal C Artificial Neural Network
 *
 * Copyright (c) 2015, 2016 Lewis Van Winkle
 *
 * http://CodePlea.com
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgement in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */


#ifndef __NN_H__
#define __NN_H__

#include <stdio.h>
#include <vector>
#include "neuron.h"
#include "layer.h"



class NN {

private :
    /* How many inputs, outputs, and hidden neurons. */
    //int inputs, hidden_layers, hidden, outputs;

    /* Which activation function to use for hidden neurons. Default: gennann_act_sigmoid_cached*/
    //nn_actfun activation_hidden;

    /* Which activation function to use for output. Default: gennann_act_sigmoid_cached*/
    //nn_actfun activation_output;

    /* Total number of weights, and size of weights buffer. */
    int total_weights{};

    /* Total number of neurons + inputs and size of output buffer. */
    int total_neurons{};

    Layer input_layer;
    std::vector<Layer> hidden_layers;
    Layer output_layer;

    std::vector<Layer*> layers;
    /* All weights (total_weights long). */
    //double *weight;
    

    /* Stores input array and output of each neuron (total_neurons long). */
    //double *output;

    /* Stores delta of each hidden and output neuron (total_neurons - inputs long). */
    //double *delta;

   public :
    // Constructor
    NN(int inputs, int nb_hidden_layers, int hidden, int outputs);

    /* Sets weights randomly. Called by init. */
    void randomize();

    /* Runs the feedforward algorithm to calculate the ann's output. */
    std::vector<double> run(const std::vector<double> * inputs);

    /* Does a single backprop update. */
    void train(const std::vector<double> *inputs, const std::vector<double> *desired_outputs, double learning_rate);
    void back_propagation(Layer *layer, Layer *previous_layer, const std::vector<double>* desired_outputs, double learning_rate);

    /* Saves the ann. */
    void write(FILE *out);

    /* Creates ANN from file saved with nn_write. */
    NN *read(FILE *in);

    double nn_act_sigmoid(double a);
    double nn_act_sigmoid_cached(double a);
    double nn_act_threshold(double a);
    double nn_act_linear(double a);

};

#endif /*__NN_H__*/
