using System;
using Unity.Mathematics;
using UnityEngine;

namespace Obi
{
    [Serializable]public class CRProperty:MonoBehaviour
    {
        public float E;
        [NonSerialized]public float A;
        [NonSerialized]public float G;
        public float Poisson;
        public float Density;
        public float R;
        public float restLength;
        public int PdIteration;
        public bool enable;
        public float totalLength;
        
        public bool uniformSampled;

        public CRProperty(float E, float poisson, float density, float R)
        {
            this.R = R;
            this.A = R*R*math.PI;
            this.Poisson = poisson;
            this.Density = density;
            this.E = E;
            G = this.E/(2*(1+Poisson));
        }
        //TODO:sjw , hardcoding test for now
        public CRProperty()
        {
            this.R = 0.1f;
            this.A = R*R*3.1415926f;
            this.Poisson = 1.5f;
            this.Density = 10f;
            this.E = 100f;
            G = this.E/(2*(1+Poisson));
        }

        public void OnEnable()
        {
            G = this.E/(2*(1+Poisson));
            this.A = R*R*math.PI;
        }
    }

    public static class Global
    {
        public static bool debugMode = false;
        public static bool animationConstrain = true;
        public static bool uniformSampled = false;
        public static bool particleOritentation = false;
    }
}