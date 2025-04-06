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
std::vector<Vector3> splinePoints;
std::vector<Vector4> splineColors;
std::vector<Vector3> splineTangents;
std::vector<int> splineIndices;
std::vector<SplineCrossSection> splineCrossSections;

void generateSplineCrossSections(float radius, int numSegments) {
    splineCrossSections.clear();
    for (int i = 0; i < splinePoints.size() ; i++){
        SplineCrossSection crossSection;
        crossSection.position = splinePoints[i];
        crossSection.tangent = splineTangents[i];
        crossSection.binormal = calculateBinormalFromTangent(crossSection.tangent);
        crossSection.normal = calculateNormalFromTangent(crossSection.tangent);
        crossSection.startIndex = i * numSegments;
        crossSection.generateVertices(radius, numSegments, i == 0 || i == splinePoints.size() - 1);
        splineCrossSections.push_back(crossSection);
    }
}

std::vector<float> Vector3ToFloat(const Vector3& vec) {
    return {vec.x, vec.y, vec.z};
}

void generateSplineIndices() {
    splineIndices.clear();
    SplineCrossSection::generateFaceIndices(splineCrossSections[0], splineIndices);
    for (int i = 0 ; i < splineCrossSections.size()-1; ++i) {
        SplineCrossSection::generateSideIndices(splineCrossSections[i], splineCrossSections[i + 1], splineIndices);
    }
    SplineCrossSection::generateFaceIndices(splineCrossSections[splineCrossSections.size()-1], splineIndices);
}

Vector3 getPointOnSpline(float u) {
    // Calculate the step size
    float stepSize = 1.0f / numVerticesPerSpline;

    // Round the value to the nearest multiple of the step size
    float roundedValue = std::round(u / stepSize) * stepSize;

    // Find the index on splinePoints
    int index = static_cast<int>(roundedValue / stepSize);

    return splinePoints[index];
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
        std::vector<float> tangent;
        point.resize(3);
        tangent.resize(3);
        createControlMatrix(controlMatrix, a, b, c, d);

        std::vector<float> uPrimeVect;

        for (int j = 0; j < numVerticesPerSpline; j++) {
            float u = static_cast<float>(j) / static_cast<float>(numVerticesPerSpline - 1);
            createUMatrix(uVect, u);

            MultiplyMatrices(4, 4, 3, &catmullBasis[0], &controlMatrix[0], &basisControlMatrix[0]);

            //Calculate the point on the spline
            MultiplyMatrices(1,4,3, &uVect[0], &basisControlMatrix[0], &point[0]);

            Vector3 pointVector (point[0], point[1], point[2]);
            splinePoints.push_back(pointVector);

            //Calculate the tangent
            createUPrimeMatrix(uPrimeVect, u);
            MultiplyMatrices(1,4,3, &uPrimeVect[0], &basisControlMatrix[0], &tangent[0]);
            Vector3 tangentVector (tangent[0], tangent[1], tangent[2]);
            tangentVector.Normalize();
            splineTangents.push_back(tangentVector);
        }
    }
    generateSplineCrossSections(0.5f, 10);
    generateSplineIndices();
    generateSplineColors();
}

void generateSplineColors() {
    splineColors.clear();
    for (int i = 0; i < splinePoints.size(); i++) {
        Vector4 color(1.0f, 1.0f, 1.0f, 1.0f);
        splineColors.push_back(color);
    }
}

void createUPrimeMatrix(std::vector<float>& vect, float u) {
    vect.resize(4);
    vect[0] = 3.0f * u * u;
    vect[1] = 2.0f * u;
    vect[2] = 1.0f;
    vect[3] = 0.0f;
}

Vector3 calculateBinormalFromTangent(const Vector3& tangent) {
    return Vector3::Cross(tangent, Vector3::UnitY);
}

Vector3 calculateNormalFromTangent(const Vector3& tangent){
  return Vector3::Cross(tangent, Vector3::UnitZ);
}