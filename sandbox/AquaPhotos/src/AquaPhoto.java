import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.RenderingHints;
import java.awt.geom.AffineTransform;
import java.awt.geom.NoninvertibleTransformException;
import java.awt.geom.Point2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Vector;

import javax.imageio.ImageIO;
import javax.swing.JComponent;

import events.Event;

/**
 * A Photo which can be dragged, scaled and rotated. Class loads an image from a
 * file and displays it to a multi-touch enabled environment which can then
 * drag, scale and modify it.
 * 
 * @author jroltgen
 * 
 */
public class AquaPhoto extends JComponent 
{
	
	/**
	 * TransformEvent is an event that maintains drag, scale, rotate,
	 * and anchor point data.  It is used by Photo to simplify graphics
	 * and contains processing.
	 * 
	 * @author jroltgen
	 *
	 */
	private class TransformEvent {
		private AffineTransform _transform;

		public TransformEvent(Location deltaPoint, Location rotationPoint,
				double rotationAngle, double scaleFactor) {

			double anchorx = rotationPoint.getX();
			double anchory = rotationPoint.getY();
			_transform = new AffineTransform();
			_transform.rotate(rotationAngle, anchorx, anchory);
			_transform.translate(deltaPoint.getX(), deltaPoint.getY());
			
			// Apply the scale transformation
			_transform.translate(-anchorx, -anchory);
			_transform.scale(scaleFactor, scaleFactor);
			_transform.translate(anchorx / scaleFactor / scaleFactor, anchory
					/ scaleFactor / scaleFactor);
		}

		public AffineTransform getTransform() {
			return _transform;
		}
		
	}
	
	private static final long serialVersionUID = -9033238853701636000L;
	private static final double MAX_IMAGE_WIDTH = 200;
	
	private static final double MAX_IMAGE_HEIGHT = 200;
	private static int _numPhotos = 0;
	
	private AffineTransform _transform = new AffineTransform();
	private BufferedImage _image;

	private int _id;
	private int _numTouches = 0;
	private int _lastX = -1;
	private int _lastY = -1;

	public AquaPhoto() throws Exception {
		this("resources/amnesia_WP.jpg");
	}

	/**
	 * Instantiates a Photo object from a File object
	 * 
	 * @param file
	 *            The file object contains the photo to be loaded
	 * @throws Exception
	 *             Throws and exception if the file is not found
	 */
	public AquaPhoto(File file) throws Exception {
		try {
			_image = ImageIO.read(file);

			int imageW = _image.getWidth(), imageH = _image.getHeight(), xPos, yPos;

			double scaleBy, wScale, hScale;

			wScale = MAX_IMAGE_WIDTH / imageW;
			hScale = MAX_IMAGE_HEIGHT / imageH;
			scaleBy = (wScale > 1 && hScale > 1 ? 1 : (wScale < hScale ? wScale
					: hScale));
			imageW = (int) (scaleBy * imageW);
			imageH = (int) (scaleBy * imageH);

			xPos = (int) (Math.random() * (AquaClient.SCREEN_SIZE.width - imageW));
			yPos = (int) (Math.random() * (AquaClient.SCREEN_SIZE.height - imageH));

			this.setBounds(xPos, yPos, imageW, imageH);

			// _image = _image.getScaledInstance(200, -1, Image.SCALE_SMOOTH);
		} catch (IOException e) {
			// _image = this.createImage(getWidth(), getHeight());
		}
		

		_id = ++_numPhotos;
	}

	/**
	 * Instantiates a Photo object from a full-qualified URL path name
	 * 
	 * @param url
	 *            The fully-qualified URL path to the file
	 * @throws Exception
	 *             Throws an exception if the file is not found
	 */
	public AquaPhoto(String url) throws Exception {
		this(new File(url));
	}

	@Override
	public boolean contains(int x, int y) {
		Point2D pt = new Point2D.Double(x, y);
		try {
			_transform.inverseTransform(pt, pt);
		} catch (NoninvertibleTransformException e) {
			e.printStackTrace();
		}
		return super.contains((int) pt.getX(), (int) pt.getY());
	}

	public int getID() {
		return _id;
	}
	
	@Override
	protected void paintComponent(Graphics g) 
	{
		int w = getWidth();
		int h = getHeight();
		//System.out.println("Painting photo at: " + getX() + ", " + getY());
		Graphics2D g2 = (Graphics2D) g;
		g2.addRenderingHints(new RenderingHints(
				RenderingHints.KEY_ANTIALIASING,
				RenderingHints.VALUE_ANTIALIAS_ON));
		AffineTransform transform = g2.getTransform();
		transform.concatenate(_transform);
		g2.setTransform(transform);
		g2.setClip(0, 0, w, h);
		g2.drawImage(_image, 0, 0, w, h, 0, 0, _image.getWidth(this), _image
				.getHeight(this), this);
		//super.paintComponent(g);
	}
	
	public Vector<String> getAllowedGestures() {
		Vector<String> allowedGestures = new Vector<String>();
		allowedGestures.add("UnifiedDragGesture\0");
		return allowedGestures;
	}

	/**
	 * Processes an event coming from Sparsh-UI for this Photo
	 * 
	 * @param e
	 *            The Sparsh-UI event
	 * @param point The location of the container in screen coordinates
	 */
	public void processEvent(Event e) {
		Location deltaPoint = new Location(0, 0);
		Location rotationPoint = new Location(0, 0);
		double rotationAngle = 0;
		double scaleFactor = 1;
		
		
		/*
		if (e instanceof TouchEvent) {
			TouchEvent te = (TouchEvent) e;
			if (te.getState() == TouchState.BIRTH) {
				_numTouches++;
			} else if (te.getState() == TouchState.DEATH) {
				_numTouches--;
				_lastX = -1;
				_lastY = -1;
				System.out.println("TOUCH");
			}
		} else if (e instanceof DragEvent) {
			DragEvent de = (DragEvent) e;
			int touchPointx = (int) (de.getAbsX() * PhotoOrganizer.SCREEN_SIZE.width);
			int touchPointy = (int) (de.getAbsY() * PhotoOrganizer.SCREEN_SIZE.height);
			if (_lastX < 0 || _lastY < 0) {
				_lastX = touchPointx;
				_lastY = touchPointy;
			}
			deltaPoint = new Location((int)(touchPointx - _lastX), (int)(touchPointy - _lastY));
			_lastX = touchPointx;
			_lastY = touchPointy;
		} else if (e instanceof RotateEvent) {
			RotateEvent re = (RotateEvent) e;
			float rotationPointx = (float)(re.getCenter().getX() * PhotoOrganizer.SCREEN_SIZE.width - containerScreenCoordinates.getX());
			float rotationPointy = (float)(re.getCenter().getY() * PhotoOrganizer.SCREEN_SIZE.height - containerScreenCoordinates.getY());
			rotationPoint = new Location(rotationPointx, rotationPointy);
			rotationAngle = re.getRotation();
			System.out.print("ROTATE ");
			System.out.print("Center: " + rotationPoint + ", ");
			System.out.println("Angle: " + rotationAngle);
		} else if (e instanceof ZoomEvent) {
			ZoomEvent ze = (ZoomEvent) e;
			scaleFactor = ze.getScale();
			int zoomCenterx = (int)(ze.getCenter().getX() * PhotoOrganizer.SCREEN_SIZE.width);
			int zoomCentery = (int)(ze.getCenter().getY() * PhotoOrganizer.SCREEN_SIZE.height);
			rotationPoint = new Location(zoomCenterx - containerScreenCoordinates.x, zoomCentery - containerScreenCoordinates.y);
		} else if(e instanceof DblClkEvent) {
			System.out.println("Received a double click event");
		}
		*/
		TransformEvent te = new TransformEvent(deltaPoint, rotationPoint,
				rotationAngle, scaleFactor);
		
		_transform.preConcatenate(te.getTransform());
		getParent().repaint();
	}

}
