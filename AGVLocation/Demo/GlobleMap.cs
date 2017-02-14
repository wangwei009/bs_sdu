using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace Demo
{
    public class LandmarkToLandmark
    {
        int A { get; set; }

        int B { get; set; }


    }
    public class GlobleMap
    {


        private static GlobleMap _int;

        public static GlobleMap Initstence => _int ?? (_int = new GlobleMap());

        public Dictionary<int, Point> LandMarkAbsLocation;

          // 关键 --距离 
        public Dictionary<int, List<LandmarkToLandmark>> LandMarkToLandMarkLenth;

        public static int Threshold = 30;  // 距离key值判断 30mm

        /// <summary>
        /// 定位模式加载地图
        /// </summary>
        /// <returns></returns>
        public bool LoadMap()
        {
            LandMarkAbsLocation = new Dictionary<int, Point>();
            LandMarkToLandMarkLenth = new Dictionary<int, List<LandmarkToLandmark>>();

          
            return true;
        }
        /// <summary>
        /// 初始建图模式
        /// </summary>
        public void CreatMap()
        {
            LandMarkAbsLocation = new Dictionary<int, Point>();
            LandMarkToLandMarkLenth =new Dictionary<int, List<LandmarkToLandmark>>();


        }
        /// <summary>
        /// 保存地图   datatable 转 xml
        /// </summary>
        public void SaveMap()
        {



        }

    }
  
 

}
