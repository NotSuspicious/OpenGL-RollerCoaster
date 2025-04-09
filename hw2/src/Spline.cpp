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
int numVerticesPerSpline = 100;
std::vector<Vector3> splinePoints;

std::vector<Vector3> splineVertices;
std::vector<Vector3> splineTangents;
std::vector<Vector3> splineNormals;

std::vector<int> splineIndices;
std::vector<SplineCrossSection> splineCrossSections;

std::vector<SplineCrossSection> leftRailCrossSections;
std::vector<SplineCrossSection> rightRailCrossSections;



Vector3 getBinormal(Vector3 tangent, Vector3 up) {
    // Check if tangent and up are almost parallel using fabs from <cmath>
    if (std::abs(Vector3::Dot(tangent, up)) > 0.99f) {
        // Use an alternative vector if they are almost parallel, e.g., UnitZ
        Vector3 alternative = Vector3::UnitZ;
        Vector3 binormal = Vector3::Cross(tangent, alternative);
        if (binormal.Length() < 0.001f) { // if tangent is also parallel to UnitZ, use UnitX
            binormal = Vector3::Cross(tangent, Vector3::UnitX);
        }
        binormal.Normalize();
        return binormal;
    }
    Vector3 binormal = Vector3::Cross(tangent, up);
    binormal.Normalize();
    return binormal;
}

Vector3 getNormal(Vector3 tangent, Vector3 up) {
    Vector3 binormal = getBinormal(tangent, up);
    Vector3 normal = Vector3::Cross(binormal, tangent);
    normal.Normalize();
    return normal;
}

SplineCrossSection createSplineCrossSection(float radius, int numSegments, int startIndex, Vector3 position, Vector3 tangent, Vector3& up) {
    SplineCrossSection crossSection;
    crossSection.position = position;
    crossSection.tangent = tangent;
    Vector3 binormal = getBinormal(tangent, up);
    Vector3 normal = getNormal(tangent, up);
    crossSection.binormal = binormal;
    crossSection.normal = normal;
    crossSection.startIndex = startIndex;
    crossSection.generateVertices(radius, numSegments);
    return crossSection;
}

void generateSplineCrossSections(float radius, int numSegments, float railWidth) {
    splineCrossSections.clear();
    Vector3 up = Vector3::UnitY;
    for (int i = 0; i < splinePoints.size(); i++){
        SplineCrossSection crossSection;
        Vector3 position = splinePoints[i] - getBinormal(splineTangents[i], up) * railWidth;
        Vector3 tangent = splineTangents[i];

        crossSection = createSplineCrossSection(radius, numSegments, splineVertices.size(), position, tangent, up);

        splineCrossSections.push_back(crossSection);
        splineVertices.insert(splineVertices.end(), crossSection.vertices.begin(), crossSection.vertices.end());
        splineNormals.insert(splineNormals.end(), crossSection.vertexNormals.begin(), crossSection.vertexNormals.end());
    }

    for (int i = 0; i < splinePoints.size(); i++){
        SplineCrossSection crossSection;
        Vector3 position = splinePoints[i] + getBinormal(splineTangents[i], up) * railWidth;
        Vector3 tangent = splineTangents[i];

        crossSection = createSplineCrossSection(radius, numSegments, splineVertices.size(), position, tangent, up);

        splineCrossSections.push_back(crossSection);
        splineVertices.insert(splineVertices.end(), crossSection.vertices.begin(), crossSection.vertices.end());
        splineNormals.insert(splineNormals.end(), crossSection.vertexNormals.begin(), crossSection.vertexNormals.end());
    }
}

std::vector<float> Vector3ToFloat(const Vector3& vec) {
    return {vec.x, vec.y, vec.z};
}

void generateSplineIndices() {
    splineIndices.clear();
//    SplineCrossSection::generateFaceIndices(splineCrossSections[0], splineIndices);
    for (int i = 0 ; i < splineCrossSections.size()/2-1; ++i) {
        SplineCrossSection::generateSideIndices(splineCrossSections[i], splineCrossSections[i + 1], splineIndices);
    }
    for (int i = splineCrossSections.size()/2 ; i < splineCrossSections.size()-1; ++i) {
        SplineCrossSection::generateSideIndices(splineCrossSections[i], splineCrossSections[i + 1], splineIndices);
    }
//    SplineCrossSection::generateFaceIndices(splineCrossSections[splineCrossSections.size()-1], splineIndices);
}

Vector3 getPointOnSpline(float u) {
    float stepSize = 1.0f / numVerticesPerSpline;

    float roundedValue = std::round(u / stepSize) * stepSize;

    int index = static_cast<int>(roundedValue / stepSize);

    return splinePoints[index];
}

Vector3 getNextPointOnSpline(float u) {
    float stepSize = 1.0f / numVerticesPerSpline;

    float roundedValue = std::round(u / stepSize) * stepSize;

    int index = static_cast<int>(roundedValue / stepSize);

    if (index + 1 >= splinePoints.size()) {
        index = 0;
    }

    return splinePoints[index + 1];
}

int getSplineIndex(float u) {
    float stepSize = 1.0f / numVerticesPerSpline;

    float roundedValue = std::round(u / stepSize) * stepSize;

    int index = static_cast<int>(roundedValue / stepSize);

    return index;
}

void initSpline() {
    for (int i = 1; i < spline.numControlPoints-3; i++) {
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
    generateSplineCrossSections(0.05f, 20, 0.2f);
    generateSplineIndices();
}

void createUPrimeMatrix(std::vector<float>& vect, float u) {
    vect.resize(4);
    vect[0] = 3.0f * u * u;
    vect[1] = 2.0f * u;
    vect[2] = 1.0f;
    vect[3] = 0.0f;
}