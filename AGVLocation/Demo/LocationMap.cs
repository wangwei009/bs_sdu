using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;


namespace Demo
{
    public class Polar
    {

        int Lenth { get; set; }

        float Angle { get; set; }

    }

    public class CoTransformation
    {

        int Lenth { get; set; }

        float Angle { get; set; }

    }
    public class Carpoit
    {
        public float Angle { get; set; }


        public int X { get; set; }


        public int Y { get; set; }

    }
    public class LocationMap 
    {
        // 激光局部坐标 笛卡尔
        public Dictionary<int, Point> LaserCartesianLocation;
        // 激光    极坐标
        public Dictionary<int, Polar> LaserPolarLocation;
        // 局部坐标下的距离字典
        public Dictionary<int, List<LandmarkToLandmark>> LandMarkToLandMarkLenth;
      
        
        // 激光与反光板的角度信息
        public List<double> LaserToLandmarkAngle;

        // 局部转换到全局地图后  计算车在   全局坐标下的位置
        public Carpoit LaserPoint=new Carpoit();



    }
}
