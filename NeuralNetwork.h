#pragma once
#include <cmath>
#include <vector>
#include "lossFunctions.h"
#include "Vector2D.h"
#include <random>

//This is an updated version of the .cpp file


namespace NeuralNetwork {

    // Activation functions
    double sigmoid(double x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    double sigmoid_derivative(double x) {
        return x * (1.0 - x);
    }

    // Neural Network Layer
    class Layer {
    public:
        // Default constructor | DO NOT USE, c++ got angry
        Layer() : input_size(0), output_size(0) {}

        // Constructor with parameters
        Layer(int input_size, int output_size) : input_size(input_size), output_size(output_size) {
            //Replace these with the rest, for zero initialization
            //weights.resize(input_size, std::vector<double>(output_size, 0.0));
            //biases.resize(output_size, 0.0);

            weights.resize(input_size, std::vector<double>(output_size));
            biases.resize(output_size);

            // Initialize weights with random values between -1 and 1
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> dist(-1.0, 1.0);

            for (int i = 0; i < input_size; ++i) {
                for (int j = 0; j < output_size; ++j) {
                    weights[i][j] = dist(gen);
                }
            }

            // Initialize biases with small random values
            for (int j = 0; j < output_size; ++j) {
                biases[j] = dist(gen) * 0.01;
            }
        }

        // Forward pass
        std::vector<double> forwardPass(const std::vector<double>& input) {
            std::vector<double> output(output_size, 0.0);

            for (int i = 0; i < input_size; ++i) {
                for (int j = 0; j < output_size; ++j) {
                    output[j] += input[i] * weights[i][j];
                }
            }

            // Apply activation function to the output
            for (int j = 0; j < output_size; ++j) {
                output[j] = sigmoid(output[j] + biases[j]);
            }

            return output;
        }



        std::vector<std::vector<double>> weights;
        std::vector<double> biases;
        int input_size;
        int output_size;
    };


    // Neural Network
    class NeuralNetwork {
    public:
        NeuralNetwork(int input_size, int hidden_size, int output_size) {
            input_layer = Layer(input_size, hidden_size);
            hidden_layer = Layer(hidden_size, output_size);
            output_layer = output_size;
        }

        //Predict 
        std::vector<double> predict(const std::vector<double>& input) {
            // Forward pass through the input layer
            std::vector<double> hidden_output = input_layer.forwardPass(input);

            // Forward pass through the hidden layer
            std::vector<double> output = hidden_layer.forwardPass(hidden_output);

            return output;
        }

        //Train
        void train(const std::vector<std::vector<double>>& inputs, const std::vector<std::vector<double>>& targets, int epochs, double learning_rate, LossFunction::LossFunction lossFunction) {
            std::vector<Layer*> layers;
            layers.push_back(&input_layer);
            layers.push_back(&hidden_layer); //TEMP FIX FOR ALLOWING ANY NETWORK DEPTH
            double prevLoss = 1.0;

            for (int epoch = 0; epoch < epochs; ++epoch) {
                double total_loss = 0.0;

                for (size_t i = 0; i < inputs.size(); ++i) {
                    std::vector<double> input = inputs[i];
                    std::vector<double> target = targets[i];

                    // Forward pass
                    std::vector<std::vector<double>> layerOutputs = forwardPass(input);
                    std::vector<double> prediction = layerOutputs.back();

                    // Calculate loss (example: mean squared error)
                    double loss = lossFunction(prediction, target);

                    // Backpropagation
                    std::vector<std::vector<double>> gradients = Backpropagate(prediction, target, layerOutputs);

                    // Update weights and biases
                    for (size_t layer_id = layers.size() - 1; layer_id > 0; --layer_id) {
                        Layer& layer = *layers[layer_id];
                        std::vector<double>& layer_output = layerOutputs[layer_id - 1];
                        std::vector<double>& gradient = gradients[layer_id];

                        for (int j = 0; j < layer.output_size; ++j) {
                            for (int k = 0; k < layer.input_size; ++k) {
                                /*
                                std::cout << "neg-change: " << learning_rate * gradient[j] * layer_output[k] << "\n";
                                std::cout << "gradient: " << gradient[j] << "\n";
                                std::cout << "layer_output: " << layer_output[k] << "\n";
                                std::cout << "before: " << layer.weights[k][j] << "\n";
                                */
                                layer.weights[k][j] -= learning_rate * gradient[j] * layer_output[k];
                                //std::cout << "after: " << layer.weights[k][j] << "\n";
                            }
                            layer.biases[j] -= learning_rate * gradient[j];
                        }
                    }

                    // Accumulate total_loss
                    total_loss += loss;
                }

                // Print average loss for the epoch
                std::cout << "Epoch " << epoch + 1 << ", Average Loss: " << total_loss / inputs.size() << std::endl;
                

                //printVector2D(hidden_layer.weights);
            }
        }

        //Train default
        void train(const std::vector<std::vector<double>>& inputs, const std::vector<std::vector<double>>& targets, int epochs, double learning_rate) {
            train(inputs, targets, epochs, learning_rate, LossFunction::meanSquaredError);
        }

    private:
        Layer input_layer;
        Layer hidden_layer;
        int output_layer;

        // Forward pass
        std::vector<std::vector<double>> forwardPass(const std::vector<double>& input) {
            std::vector<Layer*> layers;
            layers.push_back(&input_layer);
            layers.push_back(&hidden_layer); //TEMP FIX FOR ALLOWING ANY NETWORK DEPTH


            // Store the output of each layer for backpropagation
            std::vector<std::vector<double>> layer_outputs;

            //Store initial input
            layer_outputs.push_back(input);

            // Perform the forward pass through each layer and add the output to layer_outputs
            for (size_t layer_id = 0; layer_id < layers.size(); ++layer_id) {
                layer_outputs.push_back((*layers[layer_id]).forwardPass(layer_outputs.back()));
            }

            // Return the output of the final layer
            return layer_outputs;
        }

        // Backpropagate | Calculate gradiant
        std::vector<std::vector<double>> Backpropagate(const std::vector<double>& prediction, const std::vector<double>& target, const std::vector<std::vector<double>>& layer_outputs) {
            std::vector<Layer*> layers;
            layers.push_back(&input_layer);
            layers.push_back(&hidden_layer); //TEMP FIX FOR ALLOWING ANY NETWORK DEPTH



            std::vector<std::vector<double>> gradients;

            // Calculate output layer gradient
            std::vector<double> output_layer_gradient(prediction.size());
            for (size_t j = 0; j < prediction.size(); ++j) {
                output_layer_gradient[j] = (prediction[j] - target[j]) * sigmoid_derivative(prediction[j]);
            }
            gradients.push_back(output_layer_gradient);

            // Calculate hidden layer gradients
            for (size_t layer_id = layers.size() - 1; layer_id > 0; --layer_id) {
                const Layer& layer = *layers[layer_id];
                const std::vector<double>& prev_gradient = gradients.back();

                std::vector<double> layer_gradient(layer.input_size);
                for (int j = 0; j < layer.input_size; ++j) {
                    double sum = 0.0;
                    for (int k = 0; k < layer.output_size; ++k) {
                        sum += prev_gradient[k] * layer.weights[j][k];
                    }
                    layer_gradient[j] = sum * sigmoid_derivative(layer_outputs[layer_id][j]);
                }
                gradients.push_back(layer_gradient);
            }

            return gradients;
        }
    };
}