#include "models/motor_info.hpp"

namespace OneMotorGUI
{
    std::vector<std::shared_ptr<MotorInfo>> MotorDatabase::getAllMotors()
    {
        return {
            std::make_shared<MotorInfo>("M3508", MotorType::DJI_M3508, "大疆"),
            std::make_shared<MotorInfo>("G6020", MotorType::DJI_G6020, "大疆"),
            std::make_shared<MotorInfo>("J4310", MotorType::DAMIAO_J4310, "达妙"),
            std::make_shared<MotorInfo>("J10010", MotorType::DAMIAO_J10010, "达妙")};
    }

    std::vector<std::string> MotorDatabase::getBrands()
    {
        return {"大疆", "达妙"};
    }

    std::vector<std::shared_ptr<MotorInfo>> MotorDatabase::getMotorsByBrand(const std::string &brand)
    {
        std::vector<std::shared_ptr<MotorInfo>> result;
        auto allMotors = getAllMotors();

        for (const auto &motor : allMotors)
        {
            if (motor->brand == brand)
            {
                result.push_back(motor);
            }
        }

        return result;
    }
}
