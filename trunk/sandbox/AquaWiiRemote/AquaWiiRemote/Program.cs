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
        float lastZoomDistance;
        float lastAngle;

        bool discard = false;

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
                    discard = !discard;
                    if (!discard)
                    {
                        // Do move, zoom and rotate if necessary.
                        if (location[0] != 1 && location[1] != 0)
                        {
                            float dx = state.IRState.IRSensors[0].Position.X * 1024 - state.IRState.IRSensors[1].Position.X * 1024;
                            float dy = state.IRState.IRSensors[0].Position.Y * 768 - state.IRState.IRSensors[1].Position.Y * 768;
                            float newZoomDistance = (float)Math.Sqrt(dx * dx + dy * dy);
                            float newAngle = (float)Math.Atan2(dy, dx);

                            if (lastZoomDistance != -1)
                            {
                                float scale = lastZoomDistance / newZoomDistance;
                                scale = (scale - 1) * 4 + 1;
                                DoZoomEvent(location, scale);
                            }

                            if (lastAngle != -1)
                            {
                                float angle = newAngle - lastAngle;
                                DoRotate(location, angle * 2);
                                //Console.WriteLine("Angle: " + angle);
                            }
                            DoEvent(UnifiedEvent.EVENT_TYPE_MOVE, location);

                            lastZoomDistance = newZoomDistance;
                            lastAngle = newAngle;
                        }
                    }
                }
            }
            else
            {
                if (pressed)
                {
                    DoEvent(UnifiedEvent.EVENT_TYPE_UP, location);
                    lastZoomDistance = -1;
                    lastAngle = -1;
                    pressed = false;
                }
                else
                {
                    discard = !discard;
                    if (!discard)
                    {
                        if (location[0] != 1 && location[1] != 0)
                            DoEvent(UnifiedEvent.EVENT_TYPE_HOVER, location);
                    }
                }
            }

        }

        private void DoZoomEvent(float[] location, float scale)
        {
            
            UnifiedZoomEvent e = new UnifiedZoomEvent("UnifiedZoomEvent", "WiiMoteZoom",
                    (byte)UnifiedEvent.EVENT_TYPE_OTHER, 1, location, scale, location);
            sendEvent(e);
        }

        private void DoRotate(float[] location, float angle)
        {

            Unified2DRotateEvent e = new Unified2DRotateEvent("Unified2DRotateEvent", "WiiMoteRotate",
                    (byte)UnifiedEvent.EVENT_TYPE_OTHER, 1, location, angle, location);
            sendEvent(e);

            short length;
            byte[] bytesToSend = e.serialize(out length);
            //Console.Write("Len " + length + ", Data: ");
            //for (int i = 0; i < bytesToSend.Length; i++)
            //{
            //    Console.Write(bytesToSend[i] + " ");
            //}
            //Console.WriteLine();
        }

        private void DoEvent(int type, float[] location)
        {
            UnifiedEvent e = new UnifiedEvent("UnifiedEvent", "WiiMoteTouch",
                    (byte)type, 1, location);
            sendEvent(e);
            //Console.WriteLine("Type: " + type + ", loc: " + location[0] + ", " + location[1] + ", " + location[2]);
        }

        private void sendEvent(UnifiedEvent e)
        {
            short length = 0;
            byte[] bytesToSend = e.serialize(out length);
            short tmpLength = System.Net.IPAddress.HostToNetworkOrder(length);
            byte[] tmp = System.BitConverter.GetBytes(tmpLength);
            aquaSocket.Send(tmp);
            aquaSocket.Send(bytesToSend);
        }
    }
}
