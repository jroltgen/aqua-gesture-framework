import java.awt.Component;
import java.awt.Dimension;
import java.awt.Point;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.HashMap;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import events.HandIDTouchEvent;

import events.*;

public class Game extends JPanel {

	/**
	 * 
	 */
	private static final long serialVersionUID = -6528912752656031335L;
	private static final int GAME_HEIGHT = 768;
	private static final int GAME_WIDTH = 1024;
	public static final int ORGANISM_SIZE = 16;

	private static final int ORGANISMS_COLS = GAME_WIDTH / ORGANISM_SIZE;
	private static final int ORGANISMS_ROWS = GAME_HEIGHT / ORGANISM_SIZE;
	

	private Organism[][] _organisms;
	
	enum MessageType {
		REGION_ID,
		GLOBAL_INFO,
		REGION_INFO,
		PROCESS_GLOBAL_EVENT,
		PROCESS_REGION_EVENT,
		TRANSLATORS
	}
	
	private Socket _socket;
	private DataInputStream _input;
	private DataOutputStream _output;

	public static void main(String[] args) {
		new Game().run();
	}

	private void run() {
		// Setup gesture server.
		try {
			connect();
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		

		// Setup frame
		JFrame frame = new JFrame("AQUA-G Game of Life");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		// Setup myself
		setPreferredSize(new Dimension(GAME_WIDTH, GAME_HEIGHT));
		setLayout(null);
		initOrganisms();
		
		TimerTask updater = new TimerTask() {
			public void run() {
				step();
			}
		};
		Timer t = new Timer();
		t.schedule(updater, 1000, 500);

		// Finish frame setup
		frame.add(this);
		frame.pack();
		frame.setVisible(true);
		
		// Start listenining for messages
		readMessages();
	}
	
	/**
	 * Connects to Aqua.
	 * @throws IOException 
	 * @throws UnknownHostException 
	 */
	private void connect() throws UnknownHostException, IOException {
		_socket = new Socket("localhost", 5945);
		_input = new DataInputStream(_socket.getInputStream());
		_output = new DataOutputStream(_socket.getOutputStream());
		_output.write(3);
	}


	protected void step() {
		
		System.out.println("Stepping...");
		for (int i = 0; i < ORGANISMS_ROWS; i++) {
			for (int j = 0; j < ORGANISMS_COLS; j++) {
				
				int sum = 0;
				HashMap<Integer, Integer> userIDCounts = new HashMap<Integer, Integer>();
				userIDCounts.put(10, 0);
				userIDCounts.put(11, 0);
				userIDCounts.put(12, 0);
				userIDCounts.put(13, 0);
				
				// Previous row
				if (i > 0) {
					if (j > 0) sum += countOrganism(userIDCounts, _organisms[i - 1][j - 1]);
					sum += countOrganism(userIDCounts, _organisms[i - 1][j]);
					if (j < ORGANISMS_COLS - 1) sum += countOrganism(userIDCounts, _organisms[i - 1][j + 1]);
				}
				// Current row
				if (j > 0) sum += countOrganism(userIDCounts, _organisms[i][j - 1]);
				if (j < ORGANISMS_COLS - 1) sum += countOrganism(userIDCounts, _organisms[i][j + 1]);
				// Next row
				if (i < ORGANISMS_ROWS - 1) {
					if (j > 0) sum += countOrganism(userIDCounts, _organisms[i + 1][j - 1]);
					sum += countOrganism(userIDCounts, _organisms[i + 1][j]);
					if (j < ORGANISMS_COLS - 1) sum += countOrganism(userIDCounts, _organisms[i + 1][j + 1]);
				}
				//if (sum > 0) 
				//System.out.println("Sum: " + sum);
			
				if (_organisms[i][j].isAlive() > 0) {
					if (sum > 1 && sum < 4) {
						_organisms[i][j].setAction(Organism.DO_NOTHING);
					} else {
						_organisms[i][j].setAction(Organism.KILL);
					}
				} else {
					if (sum == 3) {
						// Find most popular user id
						int maxNum = -1;
						int maxUserID = -1;
						for (int key : userIDCounts.keySet()) {
							System.out.println("KEY: " + key);
							if (userIDCounts.get(key) > maxNum) {
								//System.out.println("Value: " + )
								maxUserID = key;
								maxNum = userIDCounts.get(key);
							}
						}
						_organisms[i][j].setAction(Organism.GIVE_LIVE);
						_organisms[i][j].setUserID(maxUserID);
						System.out.println("Got 3, giving life to: " + maxUserID);
					} else {
						_organisms[i][j].setAction(Organism.DO_NOTHING);
					}
				}
			}
		}
		for (int i = 0; i < ORGANISMS_ROWS; i++) {
			for (int j = 0; j < ORGANISMS_COLS; j++) {
				_organisms[i][j].step();
			}
		}
		repaint();
	}
	
	private int countOrganism(HashMap<Integer, Integer> userIDs, Organism o) {
		if (userIDs.containsKey(o.getUserID()) && o.isAlive() == 1) {
			userIDs.put(o.getUserID(), userIDs.get(o.getUserID()) + 1);
		}
		return o.isAlive();
		
	}

	private void initOrganisms() {
		_organisms = new Organism[ORGANISMS_ROWS][ORGANISMS_COLS];
		for (int i = 0; i < ORGANISMS_ROWS; i++) {
			for (int j = 0; j < ORGANISMS_COLS; j++) {
				_organisms[i][j] = new Organism();
				_organisms[i][j].setLocation((j * ORGANISM_SIZE),
						(i * ORGANISM_SIZE));
				System.out.println("Added " + i + ", " + j + ", size: "
						+ ORGANISM_SIZE);
				add(_organisms[i][j]);
			}
		}
		repaint();
	}

	
	private void readMessages() {
		while (true) {
			try {
				handleRequest();
			} catch (IOException e) {
				e.printStackTrace();
				System.exit(0);
			}
		}
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
	
	private void handleGetRegionInfo() throws IOException {
		
		int regionID = _input.readInt();
		
		List<String> gestures = SparshRegistry.getAllowedGestures(regionID);
		_output.writeInt(gestures.size());
		for (String s : gestures) {
			_output.writeBytes(s);
		}
		_output.writeInt(0);
	}

	
	private void handleGetRegionID() throws IOException {
		float[] location = new float[3];
		location[0] = _input.readFloat();
		location[1] = _input.readFloat();
		location[2] = _input.readFloat();
		
		// TODO this could be made more efficient because we already know
		// the location of each component.
		for (Component c : getComponents()) {
			Point p = new Point((int)(location[0] * GAME_WIDTH), (int)(location[1] * GAME_HEIGHT));
			SwingUtilities.convertPointFromScreen(p, c);
			if (c.contains(p)) {
				_output.writeInt(((SparshComponent)c).getGroupID());
				return;
			}
		}
		_output.writeInt(-1);
	}
	
	private void handleGetGlobalInfo() throws IOException {
		// Sent gestures as null-terminated strings.
		_output.writeInt(0);
		
		// Send the events
		_output.writeInt(1);
		_output.writeBytes("HandIDTouchEvent\0");
	}
	
	private void handleGetTranslators() throws IOException {
		// Send # of translators.
		_output.writeInt(1);
		_output.writeBytes("GetHandIDGesture\0");
		
	}
	
	private void processGlobalEvent() throws IOException {
		short length = _input.readShort();
		byte[] data = new byte[length];
		
		_input.read(data, 0, length);
		String name = "";
		int index = 0;
		while (data[index] != '\0') {
			name += (char)data[index++];
		}
		
		Event e = null;
		
		if (name.equals("HandIDTouchEvent")) {
			e = new HandIDTouchEvent(data);
		} else {
			System.out.println("Error - region event not constructed properly.");
			System.exit(0);
		}
		
		//SparshRegistry.processEvent(groupID, event);
		if (e instanceof HandIDTouchEvent) {
			HandIDTouchEvent te = (HandIDTouchEvent) e;
			int handID = te.getHandID() % 100000;
			System.out.println("Hand ID: " + handID);
			Point p = new Point((int)(te.getLocation()[0] * GAME_WIDTH), (int)(te.getLocation()[1] * GAME_HEIGHT));
			SwingUtilities.convertPointFromScreen(p, this);
			//System.out.println("(" + p.x + ", " + p.y + ")");
			if (p.y / ORGANISM_SIZE < ORGANISMS_ROWS && 
					p.x / ORGANISM_SIZE < ORGANISMS_COLS && p.y > 0 && p.x > 0) {
				_organisms[p.y / ORGANISM_SIZE][p.x / ORGANISM_SIZE].setUserID(handID);
				_organisms[p.y / ORGANISM_SIZE][p.x / ORGANISM_SIZE].giveLife();
				_organisms[p.y / ORGANISM_SIZE][p.x / ORGANISM_SIZE].repaint();
			}
		}
	}

	
	private void processRegionEvent() throws IOException {
		
		// Get region ID
		_input.readInt();
		short length = _input.readShort();
		byte[] data = new byte[length];
		
		_input.read(data, 0, length);
		
	}
	
	

}
