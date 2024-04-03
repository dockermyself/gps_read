#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <cmath>
#include "gps_reader.h"

namespace GPS
{
    struct Point3d
    {
        double x;
        double y;
        double z;
    };
    constexpr double DEG_TO_RAD = M_PI / 180.0;
    constexpr double a = 6378137.0;
    constexpr double b = 6356752.3142;
    constexpr double EARTH_RADIUS = (a + b) / 2.0;

    void ConvertLLAToENU(const LLA &init_lla, const LLA &point_lla, Point3d &enu)
    {
        double lat_rad = init_lla.lat * DEG_TO_RAD;
        double lon_rad = init_lla.lon * DEG_TO_RAD;

        double delta_lat = (point_lla.lat - init_lla.lat) * DEG_TO_RAD;
        double delta_lon = (point_lla.lon - init_lla.lon) * DEG_TO_RAD;

        enu.x = delta_lon * cos(lat_rad) * EARTH_RADIUS;
        enu.y = delta_lat * EARTH_RADIUS;
        enu.z = point_lla.alt - init_lla.alt;
    }
    double Square(double x)
    {
        return x * x;
    }
    double Distance(const Point3d &p1, const Point3d &p2)
    {
        return sqrt(Square(p1.x - p2.x) + Square(p1.y - p2.y) + Square(p1.z - p2.z));
    }
}

int main(int argc, char *argv[])
{

    GPSReader gps_reader("/dev/ttyUSB0", B9600);
    gps_reader.EnableReadThread();
    std::this_thread::sleep_for(std::chrono::milliseconds(60*20)); // 等待GPS模块初始化
    // 当前时间
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    // h:mm:ss
    std::time_t tt = std::chrono::system_clock::to_time_t(start);
    tm *ltm = localtime(&tt);
    char time_str[100] = {0};
    sprintf(time_str, "%d:%d:%d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    std::fstream file("gps_data_" + std::string(time_str) + ".txt", std::ios::out);
    LLA init_lla;
    LLA lla;
    bool first = true;
    GPS::Point3d last_pos = {0, 0, 0};
    double time_interval = 0.5;
    double last_timestamp = 0;

    // 读取GPS数据
    while (std::chrono::system_clock::now() - start < std::chrono::seconds(3600))
    {
        if (!gps_reader.getData(lla))
        {
            printf("gps reader not running\n");
            continue;
        }

        if (first)
        {
            init_lla = lla;
            last_timestamp = lla.timestamp;
            first = false;
        }
        else if (lla.timestamp - last_timestamp > time_interval)
        {
            GPS::Point3d enu;
            GPS::ConvertLLAToENU(init_lla, lla, enu);
            // 计算disrance
            double distance = GPS::Distance(enu, last_pos);
            if (distance > 0.1)
            {

                last_timestamp = lla.timestamp;
                last_pos = enu;
                // printf("latitude: %f, longitude: %f, altitude: %f, timestamp: %f, status: %d\n", lla.lat, lla.lon, lla.alt, lla.timestamp, lla.status);
                printf("x = %f , y = %f , z = %f \n", enu.x, enu.y, enu.z);
                file << enu.x << " " << enu.y << " " << enu.z << std::endl;
            }
        }
        // sleep 0.1s
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    file.close();
    gps_reader.DisableReadThread();
    return 0;
}