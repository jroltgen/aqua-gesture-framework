import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.util.HashMap;

import javax.swing.JComponent;

import events.Event;
import events.EventType;

public class PointTracker extends JComponent {

	/**
	 * 
	 */
	private static final long serialVersionUID = 3328559222726640297L;
	private HashMap<Integer, PointLocation> _downPoints = new HashMap<Integer, PointLocation>();
	private HashMap<Integer, PointLocation> _hoverPoints = new HashMap<Integer, PointLocation>();
	
	private final int SIZE = 20;

	public void processEvent(Event e) {
		// TODO Auto-generated method stub
		//System.out.println("Processing evetn: " + e.getLocation()[0] + ", " + e.getLocation()[1]);
		PointLocation l = new PointLocation(
				(int) (e.getLocation()[0] * AquaClient.SCREEN_SIZE.width),
				(int) (e.getLocation()[1] * AquaClient.SCREEN_SIZE.height));
		if (e.getType() == EventType.DOWN.ordinal()
				|| e.getType() == EventType.MOVE.ordinal()) {
			_downPoints.put(e.getId(), l);
			_hoverPoints.remove(e.getId());
		} else if (e.getType() == EventType.UP.ordinal()) {
			_downPoints.remove(e.getId());
		} else if (e.getType() == EventType.HOVER.ordinal()) {
			_hoverPoints.put(e.getId(), l);
		}
		//repaint();
	}
	
	@Override
	protected void paintComponent(Graphics g) {
		//System.out.println("-----");
		Graphics2D g2 = (Graphics2D) g;
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		for (PointLocation l : _downPoints.values()) {
			//System.out.println("Painting point: " + l.getX() + ", " + l.getY());
			g2.setColor(new Color(255, 255, 255, 127));
			g2.fillOval(l.getX() - SIZE / 2, l.getY() - SIZE / 2, SIZE, SIZE);
			
			g2.setStroke(new BasicStroke(3));
			g2.setColor(Color.GREEN);
			g.drawOval(l.getX() - SIZE / 2, l.getY() - SIZE / 2, SIZE, SIZE);
		}
		
		for (PointLocation l : _hoverPoints.values()) {
			//System.out.println("Painting point: " + l.getX() + ", " + l.getY());
			g2.setColor(new Color(255, 255, 255, 127));
			g2.fillOval(l.getX() - SIZE / 2, l.getY() - SIZE / 2, SIZE, SIZE);
			
			g2.setStroke(new BasicStroke(3));
			g2.setColor(Color.WHITE);
			g.drawOval(l.getX() - SIZE / 2, l.getY() - SIZE / 2, SIZE, SIZE);
		}
	}

	class PointLocation {
		private int x;
		private int y;

		public PointLocation(int x, int y) {
			this.x = x;
			this.y = y;
		}

		public int getX() {
			return x;
		}

		public int getY() {
			return y;
		}
	}
}
