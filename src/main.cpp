#include <iostream>
#include "functional_iter.hpp"
#include "device.hpp"

#define Drive_Train for_each_iter

//Erstelle Device und lege Parameter fest
namespace TestSet
{
    // Geschwindkeit und Standardabweichung
    constexpr double velocity_target { 80.0 }; // m/s
    constexpr double velocity_stddev { 0.016 };  // %
    
    // Abtastung
    constexpr double dt { 0.1 };    // 100ms
    
    // PT2-Werte des Feder-Dämpfer-Systems
    constexpr double par_m { 200000.0 };
    constexpr double par_r { 700 };
    constexpr double par_c { 1 };

    // Rampen für 3min
    constexpr double dv { velocity_target / ((3 * 60) / dt) };
    
    // Das Objekt
    Device device { velocity_target, velocity_stddev, 
                    dt, dv,
                    par_m, par_r, par_c};
    
    // Testlauf
    constexpr double time { 500.0 }; // s
    constexpr std::size_t samples { static_cast<uint32_t>(time / dt) };
    std::size_t counter { 0 };
}

void Monitor_TrainDrive(std::size_t& iter)
{
    // Der Zug
    Device& train { TestSet::device };

    // 1.1. - Berechne die Soll-Geschwindigkeit (Rampe)
    train.Calculate_Target_Velocity();
    
    // 1.2. - Berechne die Ist-Geschwindigkeit (PT2)
    train.Calculate_Device_Velocity();

    // 1.3. - Messe die Geschwindigkeit
    train.Measure_Velocity();

    // 1.4. - Filtere die Geschwindigkeit
    train.Filter_Velocity();

    // 1.5. - Brechne den Weg
    train.Calculate_Position();

    // 1.6. - Plotte eine Weg-Zeit-Übersicht und den Geschwindigkeitsverlauf
    train.Plot(iter);
    train.Store(iter);
}


int main(int, char**)
{
    // Der Zug, ein Shinkansen beschleunigt vo 0 auf eine mittlere Geschwindigkeit von 80m/s.
    // Das Monitorgerät soll aus der Geschwindigkeit die aktuelle Position berechnen;
    // Geschwindigkeits- und Positionsverlauf anzeigen.
    Drive_Train(TestSet::counter, TestSet::samples, Monitor_TrainDrive);
}
