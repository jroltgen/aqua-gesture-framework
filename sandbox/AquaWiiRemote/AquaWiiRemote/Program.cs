using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WiimoteLib;
using System.Threading;
using System.Net.Sockets;

namespace AquaWiiRemote
{
    class Program
    {
        Wiimote wm = new Wiimote();
        Socket aquaSocket;
        bool pressed;

        static void Main(string[] args)
        {
            new Program().Run();
        }

        private void Run()
        {
            // Set up Aqua connection
            byte[] buffer = new byte[1];
            buffer[0] = 2;
            aquaSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            aquaSocket.Connect("localhost", 3007);
            aquaSocket.Send(buffer);

            // Set up Wiimote
            wm.WiimoteChanged += WiiMoteChanged;
            wm.Connect();
            wm.SetReportType(InputReport.IRAccel, true);

            

            

            // Keep running
            while (true)
            {
                Thread.Sleep(1000);
            }
        }

        private void WiiMoteChanged(object sender, WiimoteChangedEventArgs args)
        {
            WiimoteState state = args.WiimoteState;

            float x = state.IRState.Midpoint.X;
            float y = state.IRState.Midpoint.Y;
            

            float[] location = new float[3];
            location[0] = (float)(1 - x);
            location[1] = (float)(y);
            location[2] = 0;

            if (state.ButtonState.B)
            {
                if (!pressed)
                {
                    DoEvent(UnifiedEvent.EVENT_TYPE_DOWN, location);
                    pressed = true;
                }
                else
                {
                    DoEvent(UnifiedEvent.EVENT_TYPE_MOVE, location);
                }
            }
            else
            {
                if (pressed)
                {
                    DoEvent(UnifiedEvent.EVENT_TYPE_UP, location);
                    pressed = false;
                }
                else
                {
                    DoEvent(UnifiedEvent.EVENT_TYPE_HOVER, location);
                }
            }

        }

        private void DoEvent(int type, float[] location)
        {
            UnifiedEvent e = new UnifiedEvent("UnifiedEvent", "WiiMoteTouch",
                    (byte)type, 1, location);
            short length = 0;
            byte[] bytesToSend = e.serialize(out length);
            short tmpLength = System.Net.IPAddress.HostToNetworkOrder(length);
            byte[] tmp = System.BitConverter.GetBytes(tmpLength);
            aquaSocket.Send(tmp);
            aquaSocket.Send(bytesToSend);
            Console.WriteLine("Type: " + type + ", loc: " + location[0] + ", " + location[1] + ", " + location[2]);
        }
    }
}
