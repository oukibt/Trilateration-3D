#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

struct Vector4D
{
    double X;
    double Y;
    double Z;
    double W;

    Vector4D()
    {
        X = 0.0;
        Y = 0.0;
        Z = 0.0;
        W = 0.0;
    }

    Vector4D(double x, double y, double z, double w)
    {
        this->X = x;
        this->Y = y;
        this->Z = z;
        this->W = w;
    }
};

struct Vector3D
{
    double X;
    double Y;
    double Z;

    Vector3D()
    {
        X = 0.0;
        Y = 0.0;
        Z = 0.0;
    }

    Vector3D(double x, double y, double z)
    {
        this->X = x;
        this->Y = y;
        this->Z = z;
    }
};

bool TryCalculateLocation3D(std::vector<Vector4D> points, Vector3D& result)
{
    int size = points.size();
    if (size < 4) return false;

    int rows = size * (size - 1) / 2,
        row = 0,
        i, j;

    // matrix m, vector b
    double** m = new double* [rows];
    for (i = 0; i < rows; i++)
    {
        m[i] = new double[3];
    }
    double* b = new double[rows];

    double X1, X2, Y1, Y2, Z1, Z2, R1, R2;

    for (i = 0; i < size; ++i)
    {
        for (j = i + 1; j < size; ++j)
        {
            X1 = points[i].X, Y1 = points[i].Y, Z1 = points[i].Z;
            X2 = points[j].X, Y2 = points[j].Y, Z2 = points[j].Z;

            R1 = points[i].W;
            R2 = points[j].W;

            m[row][0] = X1 - X2;
            m[row][1] = Y1 - Y2;
            m[row][2] = Z1 - Z2;
            b[row] = ((X1 * X1 - X2 * X2) + (Y1 * Y1 - Y2 * Y2) + (Z1 * Z1 - Z2 * Z2) - (R1 * R1 - R2 * R2)) / 2.0;

            row++;
        }
    }

    // Gaussian elimination
    for (i = 0; i < 3; i++)
    {
        for (j = i + 1; j < rows; j++)
        {
            double factor = m[j][i] / m[i][i];
            for (int k = i; k < 3; k++)
            {
                m[j][k] -= factor * m[i][k];
            }

            b[j] -= factor * b[i];
        }
    }

    result.Z = b[2] / m[2][2];
    result.Y = (b[1] - m[1][2] * result.Z) / m[1][1];
    result.X = (b[0] - m[0][1] * result.Y - m[0][2] * result.Z) / m[0][0];

    // deallocate memory
    for (i = 0; i < rows; i++)
    {
        delete[] m[i];
    }

    delete[] m;
    delete[] b;

    return true;
}

int main()
{
    std::vector<Vector4D> points;

    // your data
    // Vector4D (Position X, Position Y, Position Z, Distance to Position)
    points.push_back(Vector4D(85770.88, 50983.76, -32898.6, 2593700.0));
    points.push_back(Vector4D(-812728.56, 276743.11, 2092921.88, 736410.0));
    points.push_back(Vector4D(-850338.03, 264370.08, 2089410.23, 698820.0));
    points.push_back(Vector4D(-205057.25, 153577.64, 1158853.46, 1569600.0));

    Vector3D target = Vector3D(0.0, 0.0, 0.0);

    if (TryCalculateLocation3D(points, target))
    {
        printf("%.2f, %.2f, %.2f\n", target.X, target.Y, target.Z);
    }
    else
    {
        // possible only if you use less than 4 points
        printf("Failed\n");
    }

    system("pause");

    return 0;
}