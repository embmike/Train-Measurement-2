#include "device.hpp"
#include "matrix.hpp"
#include <iostream>
#include <iomanip>
#include <math.h>


Device::Device( double velocity_target, // m/s
                double velocity_stddev, // %
                double dt,			    // s
                double dv,			    // m/s
                double par_m,
                double par_r,
                double par_c)
{
        // Sollgeschwindigkeit
    _velocity_target = velocity_target;
    _velocity_stddev = velocity_stddev;

    // Zufallsgenerator für die Geschwindigkeitsmessung
    std::random_device rd {};
	_generator = std::mt19937 {rd()};

    // Initialisiere Filter mit dem Mittelwert
    std::for_each(_filterValues.begin(), _filterValues.end(), [&velocity_target](double& d){d = 0.0;});

    // Abtastrate
    _dt = dt;

    // Rampenrate
    _dv = dv;

    // PT2
    // Systemnatix A
    double a1 = -par_r / (par_m * dt);
    double a0 = -par_c / (par_m * pow(dt, 2));

    _system_PT2_A = {{
        {0.0,  a1,  a0}, 
        { dt, 1.0, 0.0},
        {0.0,  dt, 1.0} 
    }};

    // Eingangsvektor b
    double b0 = 1.0 / (par_m * pow(dt, 2));

    _input_PT2_b = {
         b0,
        0.0,
        0.0
    };

    // Position
    // Systemnatix A
    _system_Pos_A = {{
        {1.0,  dt},
        {0.0, 1.0}
    }};

    _logDataStream.open(LOG_DATA_PATH.data(), std::ofstream::out);
    if (_logDataStream.fail() or !_logDataStream.is_open())
    {
        std::cerr << "Log file could not open!" << std::endl;
    }
    _logDataStream << "time,velocity_s,jerk,acceleration,velocity,velocity_m,velocity_f,position" << std::endl;

    _state = DeviceState::INITILIZED;
}


Device::~Device()
{
    _logDataStream.close();
}

double Device::Calculate_Target_Velocity()
{
    if (_velocity_ramp < _velocity_target)
    {
        _velocity_ramp += _dv;    
    }

    _state = DeviceState::TARGET_VELOCITY_CALCULATED;
    return _velocity_ramp;
}

double Device::Calculate_Device_Velocity()
{
    // x(k+1) = A * x(k) + b * u(k)
    _pose_PT2_x = vvadd( mvmul(_system_PT2_A, _pose_PT2_x), vsmul(_input_PT2_b, _velocity_ramp) );

    _state = DeviceState::ACTUAL_VELOCITY_CALCULATED;
    return Get_PT2_Velocity();
}

double Device::Measure_Velocity()
{
    _distribution = std::normal_distribution<double> {Get_PT2_Velocity(), Get_PT2_Velocity() * _velocity_stddev};
    _measuredVelocity = _distribution(_generator);

    if (_measuredVelocity < 0.0)
    {
        _measuredVelocity = 0.0;
    }

    _state = DeviceState::VELOCITY_MEASURED;
    return _measuredVelocity;
}


double Device::Filter_Velocity()
{
    // Gleitender Mittelwertfilter
    std::rotate(_filterValues.rbegin(), _filterValues.rbegin() + 1, _filterValues.rend());
    _filterValues.at(0) = _measuredVelocity;
    Set_Velocity(std::accumulate(_filterValues.begin(), _filterValues.end(), 0) / _filterValues.size());

    _state = DeviceState::VELOCITY_FILTERED;
    return Get_Velocity();
}


double Device::Calculate_Position()
{
    // Berechne die neue Position x(p) = A * x
    _pose_Pos_x = mvmul(_system_Pos_A, _pose_Pos_x);

    _state = DeviceState::POSITION_ESTIMATED;
    return Get_Position();
}


void Device::Plot(std::size_t& iter)
{
    std::cout << std::setprecision(1) << std::fixed << (iter + 1) * _dt << " s: "
              << std::setprecision(6) << std::fixed
              << "vs=" << _velocity_ramp << " m/s"
              << " / "
              << "r=" << Get_PT2_Jerk() << " m/s³"
              << " / "
              << "a=" << Get_PT2_Accelaration() << " m/s²"
              << " / " 
              << "v=" << Get_PT2_Velocity() << " m/s"
              << " / " 
              << "vm=" << _measuredVelocity << " m/s"
              << " / " 
              << "vf=" << Get_Velocity() << " m/s"
              << " / " 
              << "s=" << std::setw(5) << std::setfill(' ') << Get_Position() << " m\n";

    _state = DeviceState::PLOTTED;
}


void Device::Store(std::size_t& iter)
{
    _logDataStream << std::setprecision(1) << std::fixed << (iter + 1) * _dt
                   << std::setprecision(6) << std::fixed
                   << "," << _velocity_ramp
                   << "," << Get_PT2_Jerk()
                   << "," << Get_PT2_Accelaration()
                   << "," << Get_PT2_Velocity()
                   << "," << _measuredVelocity
                   << "," << Get_Velocity() 
                   << "," << Get_Position()
                   << std::endl;
}
