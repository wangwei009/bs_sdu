using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using Demo.Interfece;

namespace Demo
{
    class LocationMap 
    {
        // 车体局部坐标
        public Dictionary<int, Point> LaserCartesianLocation;
        //激光 极坐标
        public Dictionary<int, Polar> LaserPolarLocation;
        // 局部坐标下的距离字典
        public Dictionary<int, List<LandmarkToLandmark>> LandMarkToLandMarkLenth;
        // 激光与反光板的角度信息
        public List<double> LaserToLandmarkAngle;




    }
}
