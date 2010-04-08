

using System;
using System.Collections.Generic;
using System.Text;



namespace AquaWiiRemote
{
    class UnifiedEvent
    {
        public static int EVENT_TYPE_DOWN = 0;
        public static int EVENT_TYPE_MOVE = 1;
        public static int EVENT_TYPE_UP = 2;
        public static int EVENT_TYPE_HOVER = 3;
        public static int EVENT_TYPE_OTHER = 4;

        private String _name;
        private String _description;
        private byte _type;
        private int _id;
        private float[] _location = new float[3];

        public UnifiedEvent(String name, String desc, byte type, int id,
                float[] location)
        {
            _name = name;
            _description = desc;
            _type = type;
            _id = id;
            _location[0] = location[0];
            _location[1] = location[1];
            _location[2] = location[2];
        }

        public byte[] serialize(out short outLength)
        {
            int i;
            short myLength = (short)((_name.Length + _description.Length + 2) + 17);
            
            short subclassLength;
            byte[] subclassData = serializeData(out subclassLength);

            outLength = (short)(myLength + subclassLength);
            byte[] ret = new byte[myLength + subclassLength];

            int tempInt;
            byte[] temp;
            int index = 0;


            // Name
            temp = Encoding.ASCII.GetBytes(_name + "\0");
            Array.Copy(temp, 0, ret, index, temp.Length);
            index += temp.Length;

            // Description
            temp = Encoding.ASCII.GetBytes(_description + "\0");
            Array.Copy(temp, 0, ret, index, temp.Length);
            index += temp.Length;

            // Type
            ret[index] = _type;
            index += 1;

            // ID
            tempInt = System.Net.IPAddress.HostToNetworkOrder(_id);
            temp = System.BitConverter.GetBytes(tempInt);
            Array.Copy(temp, 0, ret, index, temp.Length);
            index += temp.Length;

            // Location
            for (i = 0; i < 3; i++)
            {
                temp = System.BitConverter.GetBytes(_location[i]);
                endianSwap(ref temp);
                Array.Copy(temp, 0, ret, index, temp.Length);
                index += temp.Length;
            }

            // Subclass data.
            if (subclassData != null)
            {
                System.Array.Copy(subclassData, 0, ret, index, subclassLength);
            }
            return ret;
        }

        protected void endianSwap(ref byte[] temp)
        {
            if (BitConverter.IsLittleEndian)
            {
                for (int i = 0; i < temp.Length / 2; i++)
                {
                    byte tmp = temp[i];
                    temp[i] = temp[temp.Length - i - 1];
                    temp[temp.Length - i - 1] = tmp;

                }
            }
        }

        protected virtual byte[] serializeData(out short outLength)
        {
            outLength = 0;
            return null;
        }


    }


}
