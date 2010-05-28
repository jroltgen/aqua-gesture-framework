import java.awt.Color;
import java.awt.Dimension;
import java.awt.Image;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.MemoryImageSource;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Vector;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import events.Event;
import events.Unified2DRotateEvent;
import events.UnifiedDragEvent;
import events.UnifiedEvent;
import events.UnifiedZoomEvent;

public class AquaClient extends JPanel implements KeyListener, Runnable {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 8273709567701104122L;
	
	enum MessageType {
		REGION_ID,
		GLOBAL_INFO,
		REGION_INFO,
		PROCESS_GLOBAL_EVENT,
		PROCESS_REGION_EVENT,
		TRANSLATORS
	}
	
	public static Dimension SCREEN_SIZE = 
			Toolkit.getDefaultToolkit().getScreenSize();
	// 
	// FOR TABLE
	//public static Dimension SCREEN_SIZE = 
	//		new Dimension(1024, 768);
	
	private Socket _socket;
	private DataInputStream _input;
	private DataOutputStream _output;
	private JFrame _frame;
	private PointTracker _tracker;
	
	private Vector<AquaPhoto> _photos = new Vector<AquaPhoto>();
	
	public static void main(String[] args) throws UnknownHostException, IOException {
		AquaClient ac = new AquaClient();
		ac.connect();
		ac.initGUI();
		
		Thread t = new Thread(ac);
		t.start();
	}
	
	@Override
	public void run() {
		// Start the repaint thread.
		Thread t = new Thread(new Runnable() {
			@Override
			public void run() {
				while(true) {
					try {
						Thread.sleep(50);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					repaint();
				}
			}});
		t.start();
		
		while(true) {
			try {
				handleRequest();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	
	private void initGUI() {
		_frame = new JFrame();
        _frame.setSize(SCREEN_SIZE);
        _frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        _frame.setUndecorated(true);
        _frame.addKeyListener(this);
        _frame.setLayout(null);
        
        /*
        int[] pixels = new int[16 * 16];
        Image image = Toolkit.getDefaultToolkit().createImage(
                new MemoryImageSource(16, 16, pixels, 0, 16));
        Cursor transparentCursor =
                Toolkit.getDefaultToolkit().createCustomCursor
                     (image, new java.awt.Point(0, 0), "invisibleCursor");
        _frame.setCursor(transparentCursor);
        */
        _frame.add(this);
        _frame.setVisible(true);
        setBackground(Color.black);
        setLayout(null);
        setLocation(0, 0);
        setSize(SCREEN_SIZE.width, SCREEN_SIZE.height);
        

		
		_tracker = new PointTracker();
		_tracker.setLocation(0, 0);
		_tracker.setSize(_frame.getSize());
        add(_tracker);
        
        
        try {
			AquaPhoto p = new AquaPhoto(Color.cyan);
			_photos.add(p);
			p.setLocation(200, 200);
			p.setSize(320, 240);
			add(p);
			
			AquaPhoto p2 = new AquaPhoto(Color.magenta);
			_photos.add(p2);
			p2.setLocation(500, 100);
			p2.setSize(320, 240);
			add(p2);
			
			AquaPhoto p3 = new AquaPhoto(Color.green);
			_photos.add(p3);
			p3.setLocation(300, 500);
			p3.setSize(320, 240);
			add(p3);
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}


	/**
	 * Connects to Aqua.
	 * @throws IOException 
	 * @throws UnknownHostException 
	 */
	private void connect() throws UnknownHostException, IOException {
		_socket = new Socket("localhost", 3007);
		_input = new DataInputStream(_socket.getInputStream());
		_output = new DataOutputStream(_socket.getOutputStream());
		_output.write(3);
	}
	
	/**
	 * Handles an incoming request.
	 * @throws IOException 
	 */
	private void handleRequest() throws IOException {
		MessageType type = null;
		try {
			type = MessageType.values()[_input.read()];
		} catch (ArrayIndexOutOfBoundsException e) {
			System.out.println("Message not recognized...\n");
			return;
		}
		switch(type) {
		case REGION_ID:
			handleGetRegionID();
			break;
		case GLOBAL_INFO:
			handleGetGlobalInfo();
			break;
		case PROCESS_GLOBAL_EVENT:
			processGlobalEvent();
			break;
		case PROCESS_REGION_EVENT:
			processRegionEvent();
			break;
		case REGION_INFO:
			handleGetRegionInfo();
			break;
		case TRANSLATORS:
			handleGetTranslators();
			break;
		default:
			System.out.println("Error - message not recognized: " + type);
			break;
		}
	}
	
	/**
	 * Handles the getRegionID message.
	 */
	private void handleGetRegionID() throws IOException {
		float[] location = new float[3];
		location[0] = _input.readFloat();
		location[1] = _input.readFloat();
		location[2] = _input.readFloat();
		//System.out.println("Received Location - x:" + location[0] + ", y:" + 
		//		location[1] + ", z:" + location[2]);
		for (AquaPhoto photo : _photos) {
			Point pt = new Point((int)(location[0] * SCREEN_SIZE.width), 
					(int)(location[1] * SCREEN_SIZE.height));
			SwingUtilities.convertPointFromScreen(pt, photo);
			if (photo.contains(pt.x, pt.y)) {
				_output.writeInt(photo.getID());
				return;
			}
		}
		_output.writeInt(-1);
	}
	
	/**
	 * Handles the getGlobalInfo message.
	 */
	private void handleGetGlobalInfo() throws IOException {
		// Sent gestures as null-terminated strings.
		_output.writeInt(0);
		//_output.writeBytes("PrintEventGesture\0");
		
		// Send the events
		_output.writeInt(1);
		_output.writeBytes("UnifiedEvent\0");
		//_output.writeBytes("UnifiedZoomEvent\0");
	}
	
	/**
	 * Handles the getRegionInfo message.
	 */
	private void handleGetRegionInfo() throws IOException {
		int regionID = _input.readInt();
		for (AquaPhoto p : _photos) {
			if (p.getID() == regionID) {
				Vector<String> gestures = p.getAllowedGestures();
				_output.writeInt(gestures.size());
				for (String s : gestures) {
					_output.writeBytes(s);
				}
				_output.writeInt(3);
				// Want the zoom, drag & rotate events.
				_output.writeBytes("UnifiedDragEvent\0");
				_output.writeBytes("UnifiedZoomEvent\0");
				_output.writeBytes("Unified2DRotateEvent\0");
				return;
			}
		}

		// If we get to here, we don't want any gestures for this region.
		_output.writeInt(0);
		_output.writeInt(0);
	}
	
	/**
	 * Handles the processGlobalEvent message.
	 */
	private void processGlobalEvent() throws IOException {
		//System.out.println("Got global event.");
		short length = _input.readShort();
		byte[] data = new byte[length];
		
		_input.read(data, 0, length);
		String name = "";
		int index = 0;
		while (data[index] != '\0') {
			name += (char)data[index++];
		}
		//System.out.print("Data: ");
		//for (byte b : data) {
		//	System.out.print((int)b + " ");
		//}
		//System.out.println();
		Event e = null;
		
		if (name.equals("UnifiedEvent")) {
			e = new UnifiedEvent(data);
			//System.out.println("loc: " + e.getLocation()[0] + ", " + e.getLocation()[1] + ", " + e.getLocation()[2]);
		}
//		if (name.equals("UnifiedZoomEvent")) {
//			e = new UnifiedEvent(data);
//			System.out.println("Zoom loc: " + e.getLocation()[0] + ", " + e.getLocation()[1] + ", " + e.getLocation()[2]);
//		}
		_tracker.processEvent(e);
	}
	
	/**
	 * Handles the processRegionEvent message.
	 */
	private void processRegionEvent() throws IOException {
		
		int regionID = _input.readInt();
		
		short length = _input.readShort();
		byte[] data = new byte[length];
		
		_input.read(data, 0, length);
		String name = "";
		int index = 0;
		while (data[index] != '\0') {
			name += (char)data[index++];
		}
		
		Event e = null;
		
		if (name.equals("UnifiedDragEvent")) {
			e = new UnifiedDragEvent(data);
		} else if (name.equals("UnifiedZoomEvent")) {
			e = new UnifiedZoomEvent(data);
		} else if (name.equals("Unified2DRotateEvent")) {
			e = new Unified2DRotateEvent(data);
		}
		
		for (AquaPhoto p : _photos) {
			if (p.getID() == regionID) {
				p.processEvent(e);
			}
		}
	}
	/**
	 * Handles the getTranslators message.
	 */
	private void handleGetTranslators() throws IOException {
		// Send # of translators.
		_output.writeInt(0);
		
	}


	@Override
    public void keyPressed(KeyEvent e) {
		if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
			System.exit(0);
		}
    }


	@Override
	public void keyReleased(KeyEvent arg0) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void keyTyped(KeyEvent arg0) {
		// TODO Auto-generated method stub
		
	}
	
}
