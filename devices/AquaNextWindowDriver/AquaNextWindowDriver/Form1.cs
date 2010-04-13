using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Net.Sockets;

namespace AquaNextWindowDriver
{
    public partial class Form1 : Form
    {
        #region DLL Import Functions

        //Declare all the functions we will be using from the Multi-Touch DLL.
        //These functions are all defined within NWMultiTouch.h but we have to redefine them here
        //for use within Managed C#.

        [DllImport("NWMultiTouch.dll")]
        public static extern Int32 OpenDevice(Int32 deviceID, NWReceiveTouchInfoDelegate callbackFn);
        [DllImport("NWMultiTouch.dll")]
        public static extern void CloseDevice(Int32 deviceID);
        [DllImport("NWMultiTouch.dll")]
        public static extern Int32 GetConnectedDeviceCount();
        [DllImport("NWMultiTouch.dll")]
        public static extern Int32 GetConnectedDeviceID(Int32 deviceNo);
        [DllImport("NWMultiTouch.dll")]
        public static extern Int32 GetTouch(Int32 deviceID, Int32 packetID, out NWTouchPoint touchPoint, Int32 touch, Int32 ghostTouch);
        [DllImport("NWMultiTouch.dll")]
        public static extern Int32 GetPolygon(Int32 deviceID, Int32 packetID, Int32 touches, Int32 ghostTouches, [In, Out] NWPoint[] pointArray, Int32 size);
        [DllImport("NWMultiTouch.dll")]
        public static extern Int32 GetTouchDeviceInfo(Int32 deviceID, out NWDeviceInfo deviceInfo);
        [DllImport("NWMultiTouch.dll")]
        public static extern Int32 SetReportMode(Int32 deviceID, Int32 reportMode);
        [DllImport("NWMultiTouch.dll")]
        public static extern Int32 GetReportMode(Int32 deviceID);
        [DllImport("NWMultiTouch.dll")]
        public static extern Int32 SetKalmanFilterStatus(Int32 deviceID, bool kalmanOn);
        [DllImport("NWMultiTouch.dll")]
        public static extern Int32 SetTouchScreenDimensions(Int32 deviceID, float xMin, float yMin, float xMax, float yMax);
        [DllImport("NWMultiTouch.dll")]
        public static extern void SetConnectEventHandler(NWConnectEventDelegate connectHandler);
        [DllImport("NWMultiTouch.dll")]
        public static extern void SetDisconnectEventHandler(NWDisconnectEventDelegate disconnectHandler);

        #endregion

        #region Event Handlers

        //Declare callback function prototype.
        public delegate void NWReceiveTouchInfoDelegate(Int32 deviceId, Int32 deviceStatus, Int32 packetID, Int32 touches, Int32 ghostTouches);

        //Declare connect event handler prototype
        public delegate void NWConnectEventDelegate(Int32 deviceId);

        //Declare disconnect event handler prototype
        public delegate void NWDisconnectEventDelegate(Int32 deviceId);

        #endregion

        #region Structures

        //Define the required structures - from NWMultiTouch.h
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct NWTouchPoint
        {
            public Int32 TouchID;
            public Int32 TouchType;
            public Int64 TouchStart;
            public NWPoint TouchPos;
            public float Velocity;
            public float Acceleration;
            public float TouchArea;
            public Int32 TouchEventType;
            public Int32 ConfidenceLevel;
            public float Height;
            public float Width;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct NWDeviceInfo
        {
            public Int32 SerialNumber;
            public Int32 ModelNumber;
            public Int32 VersionMajor;
            public Int32 VersionMinor;
            public Int32 ProductID;
            public Int32 VendorID;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct NWDebugTouchInfo
        {
            public NWPoint[][] actualSizes;
            public float touchSymmetry;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct NWPoint
        {
            public float x;
            public float y;
        }

        #endregion

        #region Enums

        //Define the enums that are used - from NWMultiTouch.h
        public enum SuccessCode
        {
            SUCCESS = 1,
            ERR_DEVICE_NOT_OPEN = -1,
            ERR_INVALID_PACKET_ID = -2,
            ERR_INVALID_TOUCH_ID = -3,
            ERR_TOO_MANY_TOUCHES = -4,
            ERR_DEVICE_DOES_NOT_EXIST = -5,
            ERR_DISPLAY_DOES_NOT_EXIST = -6,
            ERR_FUNCTION_NOT_SUPPORTED = -7,
            ERR_INVALID_SENSOR_NUMBER = -8,
            ERR_SLOPES_MODE_NOT_SUPPORTED = -9
        }

        public enum TouchEventType
        {
            TE_TOUCH_DOWN = 1,
            TE_TOUCHING = 2,
            TE_TOUCH_UP = 3
        }

        public enum TouchType
        {
            TT_TOUCH = 1,
            TT_GHOSTTOUCH = 2,
            TT_CENTROID = 3
        }

        public enum DeviceStatus
        {
            DS_CONNECTED = 1,
            DS_TOUCH_INFO = 2,
            DS_DISCONNECTED = 3
        }

        public enum ReportModes
        {
            RM_NONE = 0,
            RM_MOUSE = 1,
            RM_MULTITOUCH = 2,
            RM_DIGITISER = 4,
            RM_SLOPESMODE = 8
        }

        #endregion

        #region Constants

        public const int MAX_TOUCHES = 32;
        public const int NUM_POLYGON_POINTS = 4;

        #endregion

        #region Constructors

        public Form1()
        {
            InitializeComponent();

            //Create instances of the event handlers.
            eventHandler = new NWReceiveTouchInfoDelegate(ReceiveTouchInformation);
            connectEventHandler = new NWConnectEventDelegate(ConnectEventHandler);
            disconnectEventHandler = new NWDisconnectEventDelegate(DisconnectEventHandler);

            //Set the connect and disconnect event handlers.
            SetConnectEventHandler(connectEventHandler);
            SetDisconnectEventHandler(disconnectEventHandler);

            //Retrieve the number of currently connected devices.
            Int32 deviceCount = GetConnectedDeviceCount();

            myDeviceID = -1;

            if (deviceCount > 0)
            {
                //Just get the first touch screen reported.
                //This function returns the serial number of the touch screen which is used to uniquely 
                //identify the touch screen.
                myDeviceID = GetConnectedDeviceID(0);

                //Initialise the device and set up the touch event handler.
                //All multi-touch information will be received by the eventHandler function.
                OpenDevice(myDeviceID, eventHandler);

                //Now set the report mode to receive multi-touch information.
                //In this case we just want to receive MultiTouch information.
                //To combine both mouse and multi-touch we could use :
                //
                //          (int)ReportModes.RM_MULTITOUCH | (int)ReportModes.RM_MOUSE
                SetReportMode(myDeviceID, (int)ReportModes.RM_SLOPESMODE);

                //Set the screen dimensions. By default, these are set to the size of
                //the screen in pixels.
                int height = Screen.PrimaryScreen.Bounds.Height;
                int width = Screen.PrimaryScreen.Bounds.Width;
                SetTouchScreenDimensions(myDeviceID, 0, 0, width, height);

                //Turn the Kalman Filters on - they are turned off by default.
                SetKalmanFilterStatus(myDeviceID, true);
            }

            NWDeviceInfo deviceInfo = GetDeviceInfo();
            //DisplayConnectedDeviceInfo(deviceInfo);

            //Set up the drawing timer.
            /*
            drawTimer = new Timer();
            drawTimer.Tick += new EventHandler(DrawTimer_Tick);
            drawTimer.Interval = 1;
            drawTimer.Enabled = true;
            drawTimer.Start();
            */
            Control.CheckForIllegalCrossThreadCalls = false;

            // Set up Aqua socket.
            byte[] buffer = new byte[1];
            buffer[0] = 2;

            aquaSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            aquaSocket.Connect("localhost", 3007);
            aquaSocket.Send(buffer);
        }

        #endregion

        #region Destructors

        ~Form1()
        {
            //Cleanup the DLL resources on closing the application.

            //Close the event handlers.
            SetConnectEventHandler(null);
            SetDisconnectEventHandler(null);

            //Close the open device if necessary.
            if (myDeviceID >= 0)
                CloseDevice(myDeviceID);
        }

        #endregion

        #region Fields
        NWReceiveTouchInfoDelegate eventHandler;
        NWConnectEventDelegate connectEventHandler;
        NWDisconnectEventDelegate disconnectEventHandler;
        Int32 myDeviceID;
        Int32 currentPacketID;
        Int32 currentTouches;
        Int32 currentGhostTouches;

        Socket aquaSocket;
        #endregion

        #region Events

        /*
        void DrawTimer_Tick(object sender, EventArgs e)
        {
            Graphics g = DrawPanel.CreateGraphics();
            g.Clear(DrawPanel.BackColor);

            try
            {
                NWPoint[] tPts = new NWPoint[2];
                int ptCnt = 0;
                bool certaintyLevel = true;

                //Loop through all the touches.
                for (int tch = 0; tch < MAX_TOUCHES; tch++)
                {
                    //If the bit is set then a touch with this ID exists.
                    if ((currentTouches & (1 << tch)) > 0)
                    {
                        //Get the touch information.
                        NWTouchPoint touchPt = new NWTouchPoint();
                        SuccessCode retCode = (SuccessCode)GetTouch(myDeviceID, currentPacketID, out touchPt, (1 << tch), 0);
                        DrawTouch(g, touchPt);

                        //Now we can get the outline of the point.
                        NWPoint[] outlinePts = new NWPoint[4];
                        int numPts = GetPolygon(myDeviceID, currentPacketID, (1 << tch), 0, outlinePts, 4);
                        DrawTouchOutline(g, outlinePts, numPts);

                        certaintyLevel = touchPt.ConfidenceLevel > 75;
                        tPts[ptCnt] = touchPt.TouchPos;
                        ptCnt++;
                    }
                }

                if (ptCnt >= 2)
                    DrawLineBetweenTouches(g, tPts[0], tPts[1], certaintyLevel);

                //Loop through all the touches.
                for (int tch = 0; tch < MAX_TOUCHES; tch++)
                {
                    //If the bit is set then a touch with this ID exists.
                    if ((currentGhostTouches & (1 << tch)) > 0)
                    {
                        //Get the touch information.
                        NWTouchPoint touchPt = new NWTouchPoint();
                        SuccessCode retCode = (SuccessCode)GetTouch(myDeviceID, currentPacketID, out touchPt, 0, (1 << tch));
                        DrawTouch(g, touchPt);

                        //Now we can get the outline of the point.
                        NWPoint[] outlinePts = new NWPoint[4];
                        int numPts = GetPolygon(myDeviceID, currentPacketID, 0, (1 << tch), outlinePts, 4);
                        DrawTouchOutline(g, outlinePts, numPts);
                    }
                }
            }
            catch (Exception ex)
            {
                int i = 0;
            }
            finally
            {
                g.Dispose();
            }
        }
        */
        #endregion

        #region Public Methods
               
        #endregion

        #region Private Methods
        /*
        private void DrawLineBetweenTouches(Graphics g, NWPoint startPt, NWPoint endPt, bool certaintyLevel)
        {
            Color drawColour = certaintyLevel ? Color.Blue : Color.SkyBlue;
            Pen drawPen = new Pen(drawColour, 3);

            Point pt1 = new Point(Convert.ToInt32(startPt.x), Convert.ToInt32(startPt.y));
            Point pt2 = new Point(Convert.ToInt32(endPt.x), Convert.ToInt32(endPt.y));

            g.DrawLine(drawPen, pt1, pt2);
            drawPen.Dispose();
        }
        */
        private void ConnectEventHandler (Int32 deviceId)
        {
            //This will get called every time a touch screen device is plugged into the system.
        }

        private void DisconnectEventHandler(Int32 deviceId)
        {
            //This will get called every time a touch screen device is unplugged from the system.
        }

        //Event Handler that receives all the multi-touch information.
        private void ReceiveTouchInformation(Int32 deviceId, Int32 deviceStatus, Int32 packetID, Int32 touches, Int32 ghostTouches)
        {
            //Check if we have received touch information.
            if (deviceStatus == (int)DeviceStatus.DS_TOUCH_INFO)
            {
                currentPacketID = packetID;
                currentTouches = touches;
                currentGhostTouches = ghostTouches;

                //Loop through all the touches.
                for (int tch = 0; tch < MAX_TOUCHES; tch++)
                {
                    //If the bit is set then a touch with this ID exists.
                    if ((currentTouches & (1 << tch)) > 0)
                    {
                        //Get the touch information.
                        NWTouchPoint touchPt = new NWTouchPoint();
                        SuccessCode retCode = (SuccessCode)GetTouch(myDeviceID, currentPacketID, out touchPt, (1 << tch), 0);
                        // TODO send the touch.
                        //DisplayTouchEventInfo(touchPt);
                        byte type;
                        if (touchPt.TouchEventType == (int) TouchEventType.TE_TOUCH_DOWN) {
                            type = (byte) UnifiedEvent.EVENT_TYPE_DOWN;
                        } else if (touchPt.TouchEventType == (int) TouchEventType.TE_TOUCH_UP) {
                            type = (byte) UnifiedEvent.EVENT_TYPE_UP;
                        } else if (touchPt.TouchEventType == (int) TouchEventType.TE_TOUCHING) {
                            type = (byte) UnifiedEvent.EVENT_TYPE_MOVE;
                        } else {
                            // Invalid type.
                            throw new Exception("Invalid type.\n");
                        }

                        float[] location = new float[] {touchPt.TouchPos.x / Screen.PrimaryScreen.Bounds.Width, 
                                touchPt.TouchPos.y / Screen.PrimaryScreen.Bounds.Height, 0};

                        UnifiedEvent ue = new UnifiedEvent("UnifiedEvent", "NWTouchEvent", type, tch, location);
                        short length = 0;
                        byte[] bytesToSend = ue.serialize(out length);
                        short tmpLength = System.Net.IPAddress.HostToNetworkOrder(length);
                        byte[] tmp = System.BitConverter.GetBytes(tmpLength);
                        aquaSocket.Send(tmp);
                        aquaSocket.Send(bytesToSend);
                    }
                }
            }
        }

        /*
        private void DrawTouchActualSize(Graphics g, NWDebugTouchInfo debugTouchInfo)
        {
            Pen drawPen = new Pen(Color.Black);
            g.DrawLine(drawPen,
                       debugTouchInfo.actualSizes[0][0].x * this.Width,
                       debugTouchInfo.actualSizes[0][0].y * this.Height,
                       debugTouchInfo.actualSizes[0][1].x * this.Width,
                       debugTouchInfo.actualSizes[0][1].y * this.Height);                                
            drawPen.Dispose();
        }

        private void DrawTouchOutline(Graphics g, NWPoint[] outlinePts, int numPts)
        {
            Pen drawPen = new Pen(Color.Black, 2);
            
            //Draw lines between each of the points in our array.
            for (int index = 0; index < numPts - 1; index++)
            {
                g.DrawLine(drawPen,
                            outlinePts[index].x,
                            outlinePts[index].y,
                            outlinePts[index+1].x,
                            outlinePts[index+1].y);
            }

            //Now join back up to the first point.
            g.DrawLine (drawPen,
                        outlinePts[numPts-1].x,
                        outlinePts[numPts-1].y,
                        outlinePts[0].x,
                        outlinePts[0].y);

                        
            drawPen.Dispose();
        }

        private void DrawTouch(Graphics g, NWTouchPoint touchPt)
        {
            float width1 = touchPt.Width / 2;
            float width2 = touchPt.Height / 2;

            Color drawColour;
            if (touchPt.ConfidenceLevel < 75)
                drawColour = Color.FromArgb(255, 255, 192, 192);    //0xc0c0ff);
            else
                drawColour = Color.FromArgb(255, 0, 255, 0);        //0xff00ff00

            Brush drawBrush = new SolidBrush(drawColour);
            Pen drawPen = new Pen(Color.Black, 1);

            //Base the size of the touch on the confidence we have in that particular touch.
            width1 = 10 + (touchPt.ConfidenceLevel / 3);
            width2 = width1;

            g.FillEllipse(drawBrush,
                          touchPt.TouchPos.x - (width1 / 2),
                          touchPt.TouchPos.y - (width2 / 2),
                          width1,
                          width2);

            g.DrawEllipse (drawPen,
                          touchPt.TouchPos.x - (width1 / 2),
                          touchPt.TouchPos.y - (width2 / 2),
                          width1,
                          width2);

            drawPen.Dispose();
            drawBrush.Dispose();
        }

        private void DisplayTouchEventInfo(NWTouchPoint touchPt)
        {
            //Just display touch down and touch up events.
            if (touchPt.TouchEventType == (int)TouchEventType.TE_TOUCH_DOWN ||
                touchPt.TouchEventType == (int)TouchEventType.TE_TOUCH_UP)
            {
                string eventString = String.Empty;

                if (touchPt.TouchEventType == (int)TouchEventType.TE_TOUCH_DOWN)
                    eventString = "Down";
                else
                    eventString = "Up";

                string[] items = { touchPt.TouchID.ToString(), eventString };
                ListViewItem lvItem = new ListViewItem(items);
                lvItem.Text = touchPt.TouchID.ToString();
                lvTouchEvents.Items.Add(lvItem);
            }
        }

        private void DisplayConnectedDeviceInfo(NWDeviceInfo deviceInfo)
        {
            AddDeviceInformationItemToList("Serial Number", deviceInfo.SerialNumber.ToString());
            AddDeviceInformationItemToList("Model Number", deviceInfo.ModelNumber.ToString());
            AddDeviceInformationItemToList("Firmware Major Version", deviceInfo.VersionMajor.ToString());
            AddDeviceInformationItemToList("Firmware Minor Version", deviceInfo.VersionMinor.ToString());
            AddDeviceInformationItemToList("Product ID", deviceInfo.ProductID.ToString());
            AddDeviceInformationItemToList("Vendor ID", deviceInfo.VendorID.ToString());
        }

        private void AddDeviceInformationItemToList(string parameter, string value)
        {
            string[] items = { parameter, value };
            ListViewItem lvItem = new ListViewItem(items);
            lvItem.Text = parameter;
            lvDeviceInfo.Items.Add(lvItem);
        }
        */

        private NWDeviceInfo GetDeviceInfo()
        {
            NWDeviceInfo deviceInfo = new NWDeviceInfo();

            //Retrieve information about the touch device.
            GetTouchDeviceInfo(myDeviceID, out deviceInfo);

            return deviceInfo;
        }

        #endregion

        private void button1_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}