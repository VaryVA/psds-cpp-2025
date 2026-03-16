#include <cmath>
#include <iostream>
#include <limits>
#include <iomanip>

struct ExpTag {};
struct DegTag {};
struct AlgTag {};

class Phasor {
private:
    double real_;
    double imag_
    
    static double NormalizePhase(double phase) {
        const double TWO_PI = 2.0 * M_PI;
        phase = std::fmod(phase, TWO_PI);
        if (phase > M_PI) {
            phase -= TWO_PI;
        } else if (phase <= -M_PI) {
            phase += TWO_PI;
        }
        if (std::abs(phase) < std::numeric_limits<double>::epsilon() * 100) {
            phase = 0.0;
        }
        return phase;
    }
    
    static double NormalizePhaseDeg(double phaseDeg) {
        phaseDeg = std::fmod(phaseDeg, 360.0);
        if (phaseDeg > 180.0) {
            phaseDeg -= 360.0;
        } else if (phaseDeg <= -180.0) {
            phaseDeg += 360.0;
        }
        if (std::abs(phaseDeg) < std::numeric_limits<double>::epsilon() * 100) {
            phaseDeg = 0.0;
        }
        return phaseDeg;
    }
    
public:
    Phasor() : real_(0.0), imag_(0.0) {}
    
    Phasor(double mag, double phaseRad) {
        if (mag < 0) {
            mag = -mag;
            phaseRad += M_PI;
        }
        real_ = mag * std::cos(phaseRad);
        imag_ = mag * std::sin(phaseRad);
        
        if (std::abs(real_) < std::numeric_limits<double>::epsilon() * 100) {
            real_ = 0.0;
        }
        if (std::abs(imag_) < std::numeric_limits<double>::epsilon() * 100) {
            imag_ = 0.0;
        }
    }
    
    Phasor(double mag, double phaseRad, ExpTag) : Phasor(mag, phaseRad) {}
    
    Phasor(double mag, double phaseDeg, DegTag) 
        : Phasor(mag, phaseDeg * M_PI / 180.0) {}
    
    Phasor(double real, double imag, AlgTag) : real_(real), imag_(imag) {}
    
    void SetPolar(double mag, double phaseRad) {
        if (mag < 0) {
            mag = -mag;
            phaseRad += M_PI;
        }
        real_ = mag * std::cos(phaseRad);
        imag_ = mag * std::sin(phaseRad);
        
        if (std::abs(real_) < std::numeric_limits<double>::epsilon() * 100) {
            real_ = 0.0;
        }
        if (std::abs(imag_) < std::numeric_limits<double>::epsilon() * 100) {
            imag_ = 0.0;
        }
    }
    
    void SetCartesian(double real, double imag) {
        real_ = real;
        imag_ = imag;
    }
    
    double Magnitude() const {
        double mag = std::sqrt(real_ * real_ + imag_ * imag_);
        if (mag < std::numeric_limits<double>::epsilon() * 100) {
            return 0.0;
        }
        return mag;
    }
    
    double Phase() const {
        if (Magnitude() < std::numeric_limits<double>::epsilon() * 100) {
            return 0.0;
        }
        return NormalizePhase(std::atan2(imag_, real_));
    }
    
    double PhaseDeg() const {
        return NormalizePhaseDeg(Phase() * 180.0 / M_PI);
    }
    
    double Real() const { return real_; }
    double Imag() const { return imag_; }
    
    double Abs() const { return Magnitude(); }
    double Angle() const { return Phase(); }
    double AngleDeg() const { return PhaseDeg(); }
    
    Phasor operator-() const {
        return Phasor(-real_, -imag_, AlgTag{});
    }
    
    Phasor& operator+=(const Phasor& other) {
        real_ += other.real_;
        imag_ += other.imag_;
        
        if (std::abs(real_) < std::numeric_limits<double>::epsilon() * 100) {
            real_ = 0.0;
        }
        if (std::abs(imag_) < std::numeric_limits<double>::epsilon() * 100) {
            imag_ = 0.0;
        }
        return *this;
    }
    
    Phasor& operator-=(const Phasor& other) {
        real_ -= other.real_;
        imag_ -= other.imag_;
        
        if (std::abs(real_) < std::numeric_limits<double>::epsilon() * 100) {
            real_ = 0.0;
        }
        if (std::abs(imag_) < std::numeric_limits<double>::epsilon() * 100) {
            imag_ = 0.0;
        }
        return *this;
    }
    
    Phasor& operator*=(const Phasor& other) {
        double mag = Magnitude() * other.Magnitude();
        double phase = Phase() + other.Phase();
        
        phase = NormalizePhase(phase);
        SetPolar(mag, phase);
        return *this;
    }
    
    Phasor& operator/=(const Phasor& other) {
        double magOther = other.Magnitude();
        if (magOther < std::numeric_limits<double>::epsilon()) {
            throw std::runtime_error("Division by zero phasor");
        }
        double mag = Magnitude() / magOther;
        double phase = Phase() - other.Phase();
        
        phase = NormalizePhase(phase);
        SetPolar(mag, phase);
        return *this;
    }
    
    Phasor& operator*=(double scalar) {
        real_ *= scalar;
        imag_ *= scalar;
        return *this;
    }
    
    Phasor& operator/=(double scalar) {
        if (std::abs(scalar) < std::numeric_limits<double>::epsilon()) {
            throw std::runtime_error("Division by zero");
        }
        real_ /= scalar;
        imag_ /= scalar;
        return *this;
    }
    
    Phasor Conj() const {
        return Phasor(real_, -imag_, AlgTag{});
    }
    
    Phasor Inv() const {
        double mag = Magnitude();
        if (mag < std::numeric_limits<double>::epsilon()) {
            throw std::runtime_error("Inverse of zero phasor");
        }
        double phase = -Phase();
        return Phasor(1.0 / mag, phase, ExpTag{});
    }
    
    bool operator==(const Phasor& other) const {
        const double eps = std::numeric_limits<double>::epsilon() * 1000;
        return std::abs(real_ - other.real_) < eps && 
               std::abs(imag_ - other.imag_) < eps;
    }
    
    bool operator!=(const Phasor& other) const {
        return !(*this == other);
    }
};

inline Phasor operator+(const Phasor& lhs, const Phasor& rhs) {
    Phasor result = lhs;
    result += rhs;
    return result;
}

inline Phasor operator-(const Phasor& lhs, const Phasor& rhs) {
    Phasor result = lhs;
    result -= rhs;
    return result;
}

inline Phasor operator*(const Phasor& lhs, const Phasor& rhs) {
    Phasor result = lhs;
    result *= rhs;
    return result;
}

inline Phasor operator/(const Phasor& lhs, const Phasor& rhs) {
    Phasor result = lhs;
    result /= rhs;
    return result;
}

inline Phasor operator+(const Phasor& lhs, double rhs) {
    return lhs + Phasor(rhs, 0.0, AlgTag{});
}

inline Phasor operator-(const Phasor& lhs, double rhs) {
    return lhs - Phasor(rhs, 0.0, AlgTag{});
}

inline Phasor operator*(const Phasor& lhs, double rhs) {
    Phasor result = lhs;
    result *= rhs;
    return result;
}

inline Phasor operator/(const Phasor& lhs, double rhs) {
    Phasor result = lhs;
    result /= rhs;
    return result;
}

inline Phasor operator+(double lhs, const Phasor& rhs) {
    return Phasor(lhs, 0.0, AlgTag{}) + rhs;
}

inline Phasor operator-(double lhs, const Phasor& rhs) {
    return Phasor(lhs, 0.0, AlgTag{}) - rhs;
}

inline Phasor operator*(double lhs, const Phasor& rhs) {
    return rhs * lhs; 
}

inline Phasor operator/(double lhs, const Phasor& rhs) {
    return Phasor(lhs, 0.0, AlgTag{}) / rhs;
}

inline Phasor MakePhasorCartesian(double real, double imag) {
    return Phasor(real, imag, AlgTag{});
}

inline Phasor MakePhasorPolar(double mag, double phaseRad) {
    return Phasor(mag, phaseRad, ExpTag{});
}

inline Phasor MakePhasorPolarDeg(double mag, double phaseDeg) {
    return Phasor(mag, phaseDeg, DegTag{});
}

inline std::ostream& operator<<(std::ostream& os, const Phasor& p) {
    std::ios_base::fmtflags old_flags = os.flags();
    std::streamsize old_precision = os.precision();
    
    os << std::fixed << std::setprecision(3);
    
    os << p.Magnitude() << "*e(j*" << p.PhaseDeg() << "°) ["
       << p.Real() << " + j*" << p.Imag() << "]";
    
    os.flags(old_flags);
    os.precision(old_precision);
    
    return os;
}
