//
// Created by William Zhao on 4/4/25.
//

#include "Camera.h"
#include "Spline.h"
#include <iostream>
#include <cmath>

Vector3 cameraPosition = Vector3(0.0f, 0.0f, 0.0f);
Point cameraRollPitchYaw = {0.0f, 0.0f, 0.0f};
Point cameraTarget = {0.0f, 0.0f, 0.0f};
Point cameraUp = {0.0f, 1.0f, 0.0f};
float cameraSplinePosition = 0.0f;
float previousCameraSplinePosition = -0.1f;
float maxHeight = 500.0f;
float g = 0.5f;

void updateCameraPosition(float deltaTime) {
//    cameraSplinePosition += cameraSpeed * deltaTime;


    Vector3 p = getPointOnSpline(cameraSplinePosition);
    Vector3 nextP = getPointOnSpline(cameraSplinePosition + 0.1f);
	Vector3 tangent = splineTangents[getSplineIndex(cameraSplinePosition)];

    Vector3 tangentToNext = nextP - p;
    std::cout << "tangentToNext: " << tangentToNext.x << ", " << tangentToNext.y << ", " << tangentToNext.z << std::endl;
    float length = tangentToNext.Length();
    float dp = length / (deltaTime);


    cameraSplinePosition = cameraSplinePosition + deltaTime * std::sqrt(2.0f * g * (maxHeight - p.y)) / dp;


    if (cameraSplinePosition >= spline.numPoints-1) {
        cameraSplinePosition = 0.0f;
    }
    if (getNextPointOnSpline(cameraSplinePosition) == getPointOnSpline(0.0f)) {
        cameraSplinePosition = 0.0f;
    }
    int cameraSplineIndex = static_cast<int>(cameraSplinePosition);

    p = getPointOnSpline(cameraSplinePosition);
    tangent = splineTangents[getSplineIndex(cameraSplinePosition)];

    cameraPosition.x = p.x;
    cameraPosition.y = p.y;
    cameraPosition.z = p.z;
    cameraPosition = cameraPosition + splineCrossSections[cameraSplineIndex].normal * 0.5f;

    cameraTarget.x = p.x + tangent.x;
    cameraTarget.y = p.y + tangent.y;
    cameraTarget.z = p.z + tangent.z;

    cameraUp.x = splineCrossSections[cameraSplineIndex].normal.x;
    cameraUp.y = splineCrossSections[cameraSplineIndex].normal.y;
    cameraUp.z = splineCrossSections[cameraSplineIndex].normal.z;
}