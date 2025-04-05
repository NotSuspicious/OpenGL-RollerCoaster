//
// Created by William Zhao on 4/4/25.
//

#include "Spline.h"
#include "Math.h"
#include <iostream>

std::vector<float> catmullBasis = {-0.5f, 1.0f, -0.5f, 0.0f,
                                   1.5f, -2.5f, 0.0f, 1.0f,
                                   -1.5f, 2.0f, 0.5f, 0.0f,
                                   0.5f, -0.5f, 0.0f, 0.0f};

struct Spline spline;
int numVerticesPerSpline = 50;
std::vector<float> splinePoints;
std::vector<Vector3> splineNormals;

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
        std::vector<float> normal;
        point.resize(3);
        normal.resize(3);
        createControlMatrix(controlMatrix, a, b, c, d);

        std::vector<float> uPrimeVect;

        for (int j = 0; j < numVerticesPerSpline; j++) {
            float u = static_cast<float>(j) / static_cast<float>(numVerticesPerSpline - 1);
            createUMatrix(uVect, u);

            MultiplyMatrices(4, 4, 3, &catmullBasis[0], &controlMatrix[0], &basisControlMatrix[0]);

            //Calculate the point on the spline
            MultiplyMatrices(1,4,3, &uVect[0], &basisControlMatrix[0], &point[0]);

            splinePoints.push_back(point[0]);
            splinePoints.push_back(point[1]);
            splinePoints.push_back(point[2]);

            //Calculate the normal
            createUPrimeMatrix(uPrimeVect, u);
            MultiplyMatrices(1,4,3, &uPrimeVect[0], &basisControlMatrix[0], &normal[0]);
            Vector3 normalVector (normal[0], normal[1], normal[2]);
            normalVector.Normalize();
            splineNormals.push_back(normalVector);
        }
    }
}

void createUPrimeMatrix(std::vector<float>& vect, float u) {
    vect.resize(4);
    vect[0] = 3.0f * u * u;
    vect[1] = 2.0f * u;
    vect[2] = 1.0f;
    vect[3] = 0.0f;
}
