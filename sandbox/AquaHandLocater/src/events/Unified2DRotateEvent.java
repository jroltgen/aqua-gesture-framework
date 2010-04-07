package events;



public class Unified2DRotateEvent extends UnifiedEvent {
	private float _rotateAngle;
	private float[] _rotateCenter = new float[2];
	
	protected Unified2DRotateEvent() {
	}
	
	public Unified2DRotateEvent(byte[] data) {
		super(data);
		int i = _name.length() + _description.length() + 2 + 17;
		
		int intBytes = (int)(((data[i] & 0xFF) << 24) + ((data[i+1] & 0xFF) << 16) +
				((data[i+2] & 0xFF) << 8) + (data[i+3]));
		_rotateAngle = Float.intBitsToFloat(intBytes);
		i += 4;
		
		intBytes = (int)(((data[i] & 0xFF) << 24) + ((data[i+1] & 0xFF) << 16) +
				((data[i+2] & 0xFF) << 8) + (data[i+3]));
		
		_rotateCenter[0] = Float.intBitsToFloat(intBytes);
		i += 4;
		
		intBytes = (int)(((data[i] & 0xFF) << 24) + ((data[i+1] & 0xFF) << 16) +
				((data[i+2] & 0xFF) << 8) + (data[i+3]));
		_rotateCenter[1] = Float.intBitsToFloat(intBytes);
		i += 4;
	}

	public float getRotateAngle() {
		return _rotateAngle;
	}

	public float[] getRotateCenter() {
		return _rotateCenter;
	}

}
