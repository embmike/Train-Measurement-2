#include "device.hpp"
#include "matrix.hpp"
#include <iostream>
#include <iomanip>


Device::Device( double velocity_target, // m/s
                double velocity_stddev, // %
                double dt,			    // s
                double par_m,
                double par_r,
                double par_c)
{
    // Zufallsgenerator für die Geschwindigkeitsmessung
    std::random_device rd {};
	_generator = std::mt19937 {rd()};
	//_distribution = std::normal_distribution<double> {vel_mean, vel_stddev};

    // Initialisiere Filter mit dem Mittelwert
    std::for_each(_filterValues.begin(), _filterValues.end(), [&velocity_target](double& d){d = velocity_target;});

    // Abtastrate
    _dt = dt;

    // Systemnatix A
    _system_A = {{
        {1.0,  dt},
        {0.0, 1.0}
    }};

    // Initialisiere Zustandsvektor x = (position, velocity)
    _pose_x = {
        0.0,
        velocity_target
    };

    _logDataStream.open(LOG_DATA_PATH.data(), std::ofstream::out);
    if (_logDataStream.fail() or !_logDataStream.is_open())
    {
        std::cerr << "Log file could not open!" << std::endl;
    }
    _logDataStream << "time,measurement,velocity,position" << std::endl;

    _state = DeviceState::INITILIZED;
}


Device::~Device()
{
    _logDataStream.close();
}

double Device::Calculate_Target_Velocity()
{
    _state = DeviceState::TARGET_VELOCITY_CALCULATED;
    return 0.0;
}

double Device::Calculate_Device_Velocity()
{
    _state = DeviceState::ACTUAL_VELOCITY_CALCULATED;
    return 0.0;
}

double Device::Measure_Velocity()
{
    _measurement = _distribution(_generator);

    _state = DeviceState::VELOCITY_MEASURED;
    return _measurement;
}


double Device::Filter_Velocity()
{
    // Gleitender Mittelwertfilter
    std::rotate(_filterValues.rbegin(), _filterValues.rbegin() + 1, _filterValues.rend());
    _filterValues.at(0) = _measurement;
    Set_Velocity(std::accumulate(_filterValues.begin(), _filterValues.end(), 0) / _filterValues.size());

    _state = DeviceState::VELOCITY_FILTERED;
    return Get_Velocity();
}


double Device::Calculate_Position()
{
    // Berechne die neue Position x(p) = A * x
    _pose_x = mvmul(_system_A, _pose_x);

    _state = DeviceState::POSITION_ESTIMATED;
    return Get_Position();
}


void Device::Plot(std::size_t& iter)
{
    std::cout << std::setprecision(1) << std::fixed << iter * _dt << " s: "
              << "v=" << Get_Velocity() << " m/s"
              << " / " 
              << "s=" << std::setw(5) << std::setfill(' ') << Get_Position() << " m\n";

    _state = DeviceState::PLOTTED;
}


void Device::Store(std::size_t& iter)
{
    _logDataStream << std::setprecision(1) << std::fixed << iter * _dt
                   << "," << _measurement
                   << "," << Get_Velocity() 
                   << "," << Get_Position()
                   << std::endl;
}
