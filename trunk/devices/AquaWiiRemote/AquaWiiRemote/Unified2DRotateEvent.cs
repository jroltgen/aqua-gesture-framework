using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AquaWiiRemote
{
    class Unified2DRotateEvent : UnifiedEvent
    {
        private float _rotateAngle;
        private float[] _center = new float[2];
        private byte[] _dataBuffer = new byte[12];

        public Unified2DRotateEvent(String name, String desc, byte type, int id,
                float[] location, float angle, float[] center)
            : base(name, desc, type, id, location)
        {
            _rotateAngle = angle;
            _center[0] = center[0];
            _center[1] = center[1];
        }

        protected override byte[] serializeData(out short outLength)
        {
            outLength = 12;
            byte[] ret = new byte[12];
            byte[] temp = new byte[4];
            int index = 0;

            // Scale
            temp = System.BitConverter.GetBytes(_rotateAngle);
            endianSwap(ref temp);
            Array.Copy(temp, 0, ret, index, temp.Length);
            index += 4;

            // Location
            for (int i = 0; i < 2; i++)
            {
                temp = System.BitConverter.GetBytes(_center[i]);
                endianSwap(ref temp);
                Array.Copy(temp, 0, ret, index, temp.Length);
                index += 4;
            }

            return ret;
            
        }
    }
}
