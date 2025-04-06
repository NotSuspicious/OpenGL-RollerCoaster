#pragma once
#include <cstdio>
#include <vector>
#include "Math.h"

// Represents one spline control point.
struct Point 
{
    float x, y, z;
};

extern std::vector<float> catmullBasis;
extern struct Spline spline;
extern int numVerticesPerSpline;
extern std::vector<Vector3> splinePoints;
extern std::vector<Vector4> splineColors;
extern std::vector<int> splineIndices;
extern std::vector<Vector3> splineTangents;
extern std::vector<struct SplineCrossSection> splineCrossSections;



struct SplineCrossSection {
    Vector3 normal, binormal, tangent;
    Vector3 position;
    int startIndex;
    int centerVertexIndex;
    std::vector<Vector3> vertices;
    // Generate vertices in a circle given the radius and number of segments.
    void generateVertices(float radius, int numSegments, bool isFace) {
        vertices.clear();
        for (int i = 0; i < numSegments; ++i) {
            const float angle = Math::TwoPi * i / numSegments;
            Vector3 cos = radius * std::cos(angle) * binormal;
            Vector3 sin = radius * std::sin(angle) * normal;
            vertices.push_back(sin + cos);
        }
        if (isFace) {
            vertices.push_back(position);
        }
    }

    static void generateSideIndices(const SplineCrossSection& a, const SplineCrossSection& b, std::vector<int>& indices) {
        for (int i = 0 ; i < a.vertices.size(); ++i) {
            int aIndex = a.startIndex + i;
            int bNextIndex = b.startIndex + (i + 1) % b.vertices.size();
            int bIndex = b.startIndex + i;
            int aNextIndex = a.startIndex + (i + 1) % a.vertices.size();
            // Create two triangles for each quad
            indices.push_back(aIndex);
            indices.push_back(aNextIndex);
            indices.push_back(bIndex);
            indices.push_back(bIndex);
            indices.push_back(bNextIndex);
            indices.push_back(aNextIndex);
        }
    }

    static void generateFaceIndices(const SplineCrossSection& a, std::vector<int>& indices) {
        for (int i = 0; i < a.vertices.size(); ++i) {
            int index = a.startIndex + i;
            int nextIndex = (i + 1) % a.vertices.size();
            indices.push_back(index);
            indices.push_back(a.centerVertexIndex);
            indices.push_back(nextIndex);
        }
    }
};

// Contains the control points of the spline.
struct Spline
{
    int numControlPoints;
    Point * points;
    int numPoints;
};

inline void createControlMatrix(std::vector<float>& vect, const Point point1, const Point point2, const Point point3, const Point point4) {
    vect.resize(12);
    vect[0] = point1.x;
    vect[1] = point2.x;
    vect[2] = point3.x;
    vect[3] = point4.x;

    vect[4] = point1.y;
    vect[5] = point2.y;
    vect[6] = point3.y;
    vect[7] = point4.y;

    vect[8] = point1.z;
    vect[9] = point2.z;
    vect[10] = point3.z;
    vect[11] = point4.z;
}

inline void createUMatrix(std::vector<float>& vect, float u) {
    vect.resize(4);
    vect[0] = u * u * u;
    vect[1] = u * u;
    vect[2] = u;
    vect[3] = 1.0f;
}

Vector3 calculateNormalFromTangent(const Vector3& tangent);
Vector3 calculateBinormalFromTangent(const Vector3& tangent);

void createUPrimeMatrix(std::vector<float>& vect, float u);

inline void loadSpline(char * argv)
{
    FILE * fileSpline = fopen(argv, "r");
    if (fileSpline == NULL) 
    {
        printf ("Cannot open file %s.\n", argv);
        exit(1);
    }

    // Read the number of spline control points.
    fscanf(fileSpline, "%d\n", &spline.numControlPoints);
    printf("Detected %d control points.\n", spline.numControlPoints);
    spline.numPoints = spline.numControlPoints - 3;

    // Allocate memory.
    spline.points = (Point *) malloc(spline.numControlPoints * sizeof(Point));
    // Load the control points.
    for(int i=0; i<spline.numControlPoints; i++)
    {
        if (fscanf(fileSpline, "%f %f %f", 
               &spline.points[i].x, 
           &spline.points[i].y, 
           &spline.points[i].z) != 3)
        {
            printf("Error: incorrect number of control points in file %s.\n", argv);
            exit(1);
        }
    }
}

void initSpline();
Vector3 getPointOnSpline(float u);
void generateSplineVAO();
void generateSplineIndices();
void generateSplineColors();
void drawGeometry();
void initColor(std::vector<float>& vect, int numVertices);
std::vector<float> Vector3ToFloat(const Vector3& vec);

// Multiply C = A * B, where A is a m x p matrix, and B is a p x n matrix.
// All matrices A, B, C must be pre-allocated (say, using malloc or similar).
// The memory storage for C must *not* overlap in memory with either A or B.
// That is, you **cannot** do C = A * C, or C = C * B. However, A and B can overlap, and so C = A * A is fine, as long as the memory buffer for A is not overlaping in memory with that of C.
// Very important: All matrices are stored in **column-major** format.
// Example. Suppose
//      [ 1 8 2 ]
//  A = [ 3 5 7 ]
//      [ 0 2 4 ]
//  Then, the storage in memory is
//   1, 3, 0, 8, 5, 2, 2, 7, 4.
inline void MultiplyMatrices(int m, int p, int n, const float * A, const float * B, float * C)
{
    for(int i=0; i<m; i++)
    {
        for(int j=0; j<n; j++)
        {
            float entry = 0.0;
            for(int k=0; k<p; k++)
                entry += A[k * m + i] * B[j * p + k];
            C[m * j + i] = entry;
        }
    }
}