import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.util.HashMap;
import java.util.List;
import java.util.Vector;


public class Organism extends SparshComponent {

	/**
	 * 
	 */
	private static final long serialVersionUID = -8143917353083648284L;

	private Color _aliveColor = new Color(220, 220, 0);
	//private static final Color _deadColor = new Color(140, 0, 0);
	private static final Color _deadColor = new Color(127, 127, 127);
	
	public static final int KILL = 0;
	public static final int GIVE_LIVE = 1;
	public static final int DO_NOTHING = 2;

	private Color _currentColor = _deadColor;
	private HashMap<Integer, Color> userIDToColorMap;
	private int currentUserID;
	
	private int _action = KILL;
	
	public Organism() {
		setSize(Game.ORGANISM_SIZE, Game.ORGANISM_SIZE);
		System.out.print("set sized");
		System.out.println(getWidth() + ", " + getHeight());

		// Setup color map
		userIDToColorMap = new HashMap<Integer, Color>();
		userIDToColorMap.put(10, Color.BLUE);
		userIDToColorMap.put(11, Color.RED);
		userIDToColorMap.put(12, Color.YELLOW);
		userIDToColorMap.put(13, Color.ORANGE);
	}
	
	public int getUserID() {
		return currentUserID;
	}
	
	@Override
	protected void paintComponent(Graphics g) {
		Graphics2D g2 = (Graphics2D) g;
		// Fill interior
		g2.setColor(_currentColor);
		g2.fillRect(0, 0, getWidth(), getHeight());
		
		// Draw border
		g2.setColor(Color.BLACK);
		g2.setStroke(new BasicStroke(2));
		g2.drawRect(0, 0, getWidth(), getHeight());
		//System.out.println("Done painting, w: " + getWidth() + ", H: " + getHeight());
	}
	
	@Override
	public List<String> getAllowedGestures(int GroupID) {
		Vector<String> gestures = new Vector<String>();
		return gestures;
	}
	
	public void giveLife() {
		_currentColor = _aliveColor;
	}
	
	public void kill() {
		_currentColor = _deadColor;
	}
	
	public int isAlive() {
		if (_currentColor == _aliveColor) {
			return 1;
		} else {
			return 0;
		}
	}
	
	public void setAction(int action) {
		_action = action;
	}
	
	public void step() {
		if (_action == KILL) {
			kill();
		} else if (_action == GIVE_LIVE) {
			giveLife();
		}
	}

	@Override
	public void processEvent(int groupID, events.Event arg1) {
	}

	public void setUserID(int userID) {
		currentUserID = userID;
		if (userIDToColorMap.containsKey(userID)) {
			_aliveColor = userIDToColorMap.get(userID);
		} else {
			_aliveColor = Color.WHITE;
		}
	}

}
