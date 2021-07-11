#include "SoundSpeed.h"
#include <cmath>


namespace ssp
{

double Gravity(double latitudeDeg)
{
    double sinLat = sin(latitudeDeg * 0.017453292519943295);  // Convert to radians
    double sin2 = sinLat * sinLat;

    double g = 9.780318 * (1.0 + 5.2788e-3 * sin2 + 2.36e-5 * sin2*sin2);
    return g;
}


double Depth(double pressureDBar, double latitudeDeg)
{
    const double g = Gravity(latitudeDeg);

    double P = pressureDBar * 0.01;  // Decibar to MegaPascal
    double Z = 9.72659e2 * P - 2.2512e-1 * P * P + 2.279e-4 * P * P * P - 1.82e-7 * P * P * P * P;
    Z /= (g + 1.092e-4 * P);

    return Z;
}

};  // End namespace ssp
