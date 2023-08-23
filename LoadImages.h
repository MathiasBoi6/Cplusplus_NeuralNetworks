#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include "NeuralNetwork.h"

std::vector<double> ImageLoad(const char* path) {
    const char* imageFilePath = path; //CHECK THIS
    int width, height, channels;
    unsigned char* image = stbi_load(imageFilePath, &width, &height, &channels, STBI_grey);
    std::vector<double> whitenessValues;


    if (image) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int pixelIndex = y * width + x;
                float whiteness = static_cast<float>(image[pixelIndex]) / 255.0f;
                whitenessValues.push_back(static_cast<double>(whiteness));
            }
        }

        stbi_image_free(image);
    }
    else {
        throw std::runtime_error("Failed to load image\n");
    }

    return whitenessValues;
}


NeuralNetwork::NeuralNetwork MakeImageNN() {
    //Image sizes are hardcoded
    int imageWidth = 28;
    int imageHeight = 28;
    int inputNeurons = imageHeight * imageWidth; //784 (one for each possible input)
    int hiddenLayer = inputNeurons / 4; //An arbitrary number
    int outputNeurons = 10; //0-9 (one for each possible output)


    NeuralNetwork::NeuralNetwork nn(inputNeurons, hiddenLayer, outputNeurons); //738, 200, 10
    std::vector<std::vector<double>> trainingInputs;
    std::vector<std::vector<double>> trainingTargets;


    const char* imageFilePath = "MNIST Dataset JPG format/MNIST - JPG - training/";

    for (boost::filesystem::directory_entry& entry : boost::filesystem::directory_iterator(imageFilePath))
    {
        int filenumber = 0;
        std::vector<double> trainingTarget(10, 0.0);
        trainingTarget[filenumber] = 1.0;

        for (boost::filesystem::directory_entry& entryFile : boost::filesystem::directory_iterator(entry.path())) {
            trainingInputs.push_back(ImageLoad(entryFile.path().string().c_str()));
            trainingTargets.push_back(trainingTarget);

            ++filenumber;;
            if (filenumber >= 10) {
                break; // Exit the loop after 100 iterations
            }
        }
    }


    int epochs = 100;
    double learningRate = 0.01;
    nn.train(trainingInputs, trainingTargets, epochs, learningRate);

    std::cout << "\n\nNetwork Trained\n\n";
    //NEURAL NETWORK SHOULD BE ABLE TO RECIVE THIS DATA, NOW GET A VALUE FROM THE GUI


    // Training example
    //std::vector<std::vector<double>> training_inputs = {
    //    {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0}, // Example training input
        // Add more training inputs here if needed
    //};
    //ImageLoader();

    return nn;
}

/*PRINT OUT A VECTOR VECTOR
    for (const auto& innerVec : trainingInputs) {
        for (const double& value : innerVec) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }*/
