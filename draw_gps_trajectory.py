import os
import sys
import matplotlib.pyplot as plt

if __name__ == "__main__":
    data_path = os.path.join(os.path.dirname(__file__), "data/gps_data_8_25_38.txt")
    # 绘制GPS轨迹
    plt.figure()
    x_data = []
    y_data = []

    with open(data_path, "r") as f:
        for data in f.readlines():
            data = data.split()
            x_data.append(float(data[0]))
            y_data.append(float(data[1]))
    # 轨迹绘制 颜色为红色
    plt.plot(x_data, y_data, color="red", label="GPS Trajectory")
    # 绘制起点和终点
    plt.scatter(x_data[0], y_data[0], color="blue", label="Start Point")
    plt.scatter(x_data[-1], y_data[-1], color="green", label="End Point")
    plt.xlabel("x:m")
    plt.ylabel("y:m")
    plt.title("GPS Trajectory")
    plt.legend()
    # save
    plt.savefig("GPS Trajectory.png")
    plt.show()
