#ifndef DEVICE_H
#define DEVICE_H

#include <cstdint>
#include <array>
#include <random>
#include <algorithm>
#include <string_view>
#include <fstream>


class Device final
{
public:
    using filter_array = std::array<double,10>;
    using pose_array = std::array<double,2>;
    using system_matrix = std::array<std::array<double,2>,2>;

    enum struct DeviceState
    {
        UNDEFINED,
        INITILIZED,
        TARGET_VELOCITY_CALCULATED,
        ACTUAL_VELOCITY_CALCULATED,
        VELOCITY_MEASURED,
        VELOCITY_FILTERED,
        POSITION_ESTIMATED,
        PLOTTED
    };

    // Initialisiere das Gerät
    Device(double velocity_target, // m/s
           double velocity_stddev, // %
           double dt,			   // s
           double par_m,
           double par_r,
           double par_c);

    ~Device();
    
    // Berechne die Soll-Geschwindigkeit (Rampe)
    double Calculate_Target_Velocity();
    
    // Berechne die Ist-Geschwindigkeit (PT2)
    double Calculate_Device_Velocity();

    // Messe die Geschwindigkeit
    double Measure_Velocity();

    // Filtere die Geschwindigkeit
    double Filter_Velocity();

    // Brechne den Weg
    double Calculate_Position();

    // Plotte eine Weg-Zeit-Übersicht und den Geschwindigkeitsverlauf
    void Plot(std::size_t& iter);

    // Siche die Daten in einer Datei
    void Store(std::size_t& iter);

private:
    Device() = delete;

    DeviceState _state { DeviceState::UNDEFINED };

    // Zufallsgenerator für die Geschwindigkeitsmessung
    std::mt19937 _generator {};
    std::normal_distribution<double> _distribution;
    double _measurement { 0.0 };

    // Filter der Geschwindigkeit
    filter_array _filterValues { 0.0 };

    // Berechnung der Position
    double _dt { 0.1 };

    // Systemgleichung: x(p) = A * x
    system_matrix _system_A {{ 
        {0.0,0.0}, 
        {0.0,0.0} 
    }};

    // Zustandsvektor x = (Position, Geschwindigkeit)
    pose_array _pose_x { 
        0.0, 
        0.0
    };

    void Set_Velocity(double val) {_pose_x[1] = val;}
    double Get_Velocity() {return _pose_x[1];}
    double Get_Position() {return _pose_x[0];}

    std::string_view LOG_DATA_PATH { "../analysis/log_data.csv" };
    std::ofstream _logDataStream;
};


#endif //DEVICE_H