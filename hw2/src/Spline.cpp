//
// Created by William Zhao on 4/4/25.
//

#include "Spline.h"
#include <iostream>

std::vector<float> catmullBasis = {-0.5f, 1.0f, -0.5f, 0.0f,
                                   1.5f, -2.5f, 0.0f, 1.0f,
                                   -1.5f, 2.0f, 0.5f, 0.0f,
                                   0.5f, -0.5f, 0.0f, 0.0f};

struct Spline spline;
int numVerticesPerSpline = 10;
std::vector<float> splinePoints;

Point getPointOnSpline(float u) {
    // Calculate the step size
    float stepSize = 1.0f / numVerticesPerSpline;

    // Round the value to the nearest multiple of the step size
    float roundedValue = std::round(u / stepSize) * stepSize;

    // Find the index on splinePoints
    int index = static_cast<int>(roundedValue / stepSize) * 3;

    Point point;
    point.x = splinePoints[index];
    point.y = splinePoints[index + 1];
    point.z = splinePoints[index + 2];
    return point;
}

void initSpline() {
    for (int i = 1; i < spline.numControlPoints-2; i++) {
        Point a = spline.points[i-1];
        Point b = spline.points[i];
        Point c = spline.points[i+1];
        Point d = spline.points[i+2];
        std::vector<float> controlMatrix;
        std::vector<float> uVect;
        std::vector<float> basisControlMatrix;
        basisControlMatrix.resize(12);
        std::vector<float> point;
        point.resize(3);
        createControlMatrix(controlMatrix, a, b, c, d);

        for (int j = 0; j < numVerticesPerSpline; j++) {
            float u = static_cast<float>(j) / static_cast<float>(numVerticesPerSpline - 1);
            createUMatrix(uVect, u);
            MultiplyMatrices(4, 4, 3, &catmullBasis[0], &controlMatrix[0], &basisControlMatrix[0]);
            MultiplyMatrices(1,4,3, &uVect[0], &basisControlMatrix[0], &point[0]);
            splinePoints.push_back(point[0]);
            splinePoints.push_back(point[1]);
            splinePoints.push_back(point[2]);
        }
    }
}