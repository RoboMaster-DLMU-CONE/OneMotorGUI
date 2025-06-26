#ifndef MOTOR_INFO_HPP
#define MOTOR_INFO_HPP

#include <string>
#include <utility>
#include <vector>
#include <memory>

namespace OneMotorGUI
{
    enum class MotorType
    {
        DJI_M3508,
        DJI_G6020,
        DAMIAO_J4310,
        DAMIAO_J10010
    };

    struct MotorInfo
    {
        std::string name;
        MotorType type;
        std::string brand;

        MotorInfo(std::string name, const MotorType type, std::string brand)
            : name(std::move(name)), type(type), brand(std::move(brand)) {}
    };

    class MotorDatabase
    {
    public:
        static std::vector<std::shared_ptr<MotorInfo>> getAllMotors();
        static std::vector<std::string> getBrands();
        static std::vector<std::shared_ptr<MotorInfo>> getMotorsByBrand(const std::string &brand);
    };
}

#endif // MOTOR_INFO_HPP
