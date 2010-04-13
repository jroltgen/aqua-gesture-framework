using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AquaWiiRemote
{
    class UnifiedZoomEvent : UnifiedEvent
    {
        private float _scale;
        private float[] _center = new float[3];

        public UnifiedZoomEvent(String name, String desc, byte type, int id,
                float[] location, float scale, float[] center)
            : base(name, desc, type, id, location)
        {
            _scale = scale;
            _center[0] = center[0];
            _center[1] = center[1];
            _center[2] = center[2];
        }

        protected override byte[] serializeData(out short outLength)
        {
            outLength = 16;
            byte[] ret = new byte[16];
            float tempScale, tempX, tempY, tempZ;
            byte[] temp = new byte[4];
            int index = 0;

            tempScale = _scale;
            tempX = _center[0];
            tempY = _center[1];
            tempZ = _center[2];

            // Scale
            temp = System.BitConverter.GetBytes(_scale);
            endianSwap(ref temp);
            Array.Copy(temp, 0, ret, index, temp.Length);
            index += temp.Length;

            // Location
            for (int i = 0; i < 3; i++)
            {
                temp = System.BitConverter.GetBytes(_center[i]);
                endianSwap(ref temp);
                Array.Copy(temp, 0, ret, index, temp.Length);
                index += temp.Length;
            }

            return ret;
            
        }
    }
}
