#pragma once
#include <vector>
#include <stdexcept>

namespace LossFunction {
    using LossFunction = double (*)(const std::vector<double>& prediction, const std::vector<double>& target);

	double meanSquaredError(const std::vector<double>& prediction, const std::vector<double>& target) {
        if (prediction.size() != target.size()) {
            throw std::invalid_argument("Input sizes must match for calculating loss.");
        }

        double sumSquaredErrors = 0.0;
        for (size_t i = 0; i < prediction.size(); ++i) {
            double error = prediction[i] - target[i];
            sumSquaredErrors += error * error;
        }

        return sumSquaredErrors / prediction.size(); // Divide by the number of samples to get the mean
	}
}