package events;

public class HandIDTouchEvent extends Event {
	
	private int _handID;

	protected HandIDTouchEvent() {
	}
	
	public HandIDTouchEvent(byte[] data) {
		super(data);
		int i = _name.length() + _description.length() + 2 + 17;
		//System.out.println((data[i] & 0xff) + ", " + (data[i+1] & 0xff) + ", " + 
		//		(data[i+2] & 0xff) + ", " + (data[i+3] & 0xff));
		_handID = (int)(((data[i] & 0xFF) << 24) + ((data[i+1] & 0xFF) << 16) +
				((data[i+2] & 0xFF) << 8) + (data[i+3] & 0xFF));
		i += 4;
	}
	
	public int getHandID() {
		return _handID;
	}
}
