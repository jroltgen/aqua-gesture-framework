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
        WiimoteCollection wc;
        Wiimote wm = new Wiimote();
        Socket aquaSocket;
        Dictionary<Guid, int> _hashcodetoid;
        Dictionary<int, bool> pressed;
        Dictionary<int, float> lastZoomDistances;
        Dictionary<int, float> lastAngles;
        Dictionary<int, bool> discard;

        static void Main(string[] args)
        {
            new Program().Run();
        }

        private void Run()
        {
            // Set up Aqua connection
            int index = 1;
            byte[] buffer = new byte[1];
            buffer[0] = 2;
            aquaSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            aquaSocket.Connect("localhost", 3007);
            aquaSocket.Send(buffer);

            // Set up Wiimote
            wc = new WiimoteCollection();
            try
            {
                wc.FindAllWiimotes();
            }
            catch (Exception)
            {
                Console.WriteLine("Error finding wiimotes.");
            }
            Console.WriteLine("Got collection");
            _hashcodetoid = new Dictionary<Guid, int>();
            lastAngles = new Dictionary<int, float>();
            lastZoomDistances = new Dictionary<int, float>();
            pressed = new Dictionary<int, bool>();
            discard = new Dictionary<int, bool>();
            foreach (Wiimote wm in wc)
            {
                wm.Connect();
                wm.SetReportType(InputReport.IRAccel, true);
                _hashcodetoid.Add(wm.ID, index);
                Console.WriteLine("Connection: " + _hashcodetoid[wm.ID]);
                lastAngles.Add(index, -1);
                lastZoomDistances.Add(index, -1);
                pressed.Add(index, false);
                discard.Add(index, false);
                wm.SetLEDs(index++);
                wm.WiimoteChanged += WiiMoteChanged;
            }            

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
            int id = _hashcodetoid[((Wiimote)sender).ID];
            
            

            float[] location = new float[3];
            location[0] = (float)(1 - x);
            location[1] = (float)(y);
            location[2] = 0;

            if (state.ButtonState.B)
            {
                if (!pressed[id])
                {
                    DoEvent(UnifiedEvent.EVENT_TYPE_DOWN, location, id);
                    pressed[id] = true;
                }
                else
                {
                    if (!discard[id])
                    {
                        // Do move, zoom and rotate if necessary.
                        if (location[0] != 1 && location[1] != 0)
                        {
                            float dx = state.IRState.IRSensors[0].Position.X * 1024 - state.IRState.IRSensors[1].Position.X * 1024;
                            float dy = state.IRState.IRSensors[0].Position.Y * 768 - state.IRState.IRSensors[1].Position.Y * 768;
                            float newZoomDistance = (float)Math.Sqrt(dx * dx + dy * dy);
                            float newAngle = (float)Math.Atan2(dy, dx);

                            if (lastZoomDistances[id] != -1)
                            {
                                float scale = lastZoomDistances[id] / newZoomDistance;
                                scale = (scale - 1) * 3 + 1;
                                DoZoomEvent(location, scale, id);
                            }

                            if (lastAngles[id] != -1)
                            {
                                float angle = newAngle - lastAngles[id];
                                //DoRotate(location, angle, id);
                                //Console.WriteLine("Angle: " + angle);
                            }
                            DoEvent(UnifiedEvent.EVENT_TYPE_MOVE, location, id);

                            lastZoomDistances[id] = newZoomDistance;
                            lastAngles[id] = newAngle;
                        }
                    }
                    discard[id] = !discard[id];
                }
            }
            else
            {
                if (pressed[id])
                {
                    DoEvent(UnifiedEvent.EVENT_TYPE_UP, location, id);
                    lastZoomDistances[id] = -1;
                    lastAngles[id] = -1;
                    pressed[id] = false;
                }
                else
                {
                    if (!discard[id])
                    {
                        if (location[0] != 1 && location[1] != 0)
                            DoEvent(UnifiedEvent.EVENT_TYPE_HOVER, location, id);
                    }
                    discard[id] = !discard[id];
                }
            }

        }

        private void DoZoomEvent(float[] location, float scale, int id)
        {
            
            UnifiedZoomEvent e = new UnifiedZoomEvent("UnifiedZoomEvent", "WiiMoteZoom",
                    (byte)UnifiedEvent.EVENT_TYPE_OTHER, 0, location, scale, location);
            sendEvent(e);
        }

        private void DoRotate(float[] location, float angle, int id)
        {

            Unified2DRotateEvent e = new Unified2DRotateEvent("Unified2DRotateEvent", "WiiMoteRotate",
                    (byte)UnifiedEvent.EVENT_TYPE_OTHER, 0, location, angle, location);
            //sendEvent(e);
            Console.WriteLine(angle);
            short length;
            byte[] bytesToSend = e.serialize(out length);
            //Console.Write("Len " + length + ", Data: ");
            //for (int i = 0; i < bytesToSend.Length; i++)
            //{
            //    Console.Write(bytesToSend[i] + " ");
            //}
            //Console.WriteLine();
        }

        private void DoEvent(int type, float[] location, int id)
        {
            UnifiedEvent e = new UnifiedEvent("UnifiedEvent", "WiiMoteTouch",
                    (byte)type, id, location);
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
