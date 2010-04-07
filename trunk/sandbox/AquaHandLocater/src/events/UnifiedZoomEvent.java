package events;



public class UnifiedZoomEvent extends UnifiedEvent {
	
	private float _zoomScale;
	private float[] _zoomCenter = new float[3];
	
	protected UnifiedZoomEvent() {
	}
	
	public UnifiedZoomEvent(byte[] data) {
		super(data);
		int i = _name.length() + _description.length() + 2 + 17;
		
		int intBytes = (int)(((data[i] & 0xFF) << 24) + ((data[i+1] & 0xFF) << 16) +
				((data[i+2] & 0xFF) << 8) + (data[i+3]));
		_zoomScale = Float.intBitsToFloat(intBytes);
		i += 4;
		
		intBytes = (int)(((data[i] & 0xFF) << 24) + ((data[i+1] & 0xFF) << 16) +
				((data[i+2] & 0xFF) << 8) + (data[i+3]));
		
		_zoomCenter[0] = Float.intBitsToFloat(intBytes);
		i += 4;
		
		intBytes = (int)(((data[i] & 0xFF) << 24) + ((data[i+1] & 0xFF) << 16) +
				((data[i+2] & 0xFF) << 8) + (data[i+3]));
		_zoomCenter[1] = Float.intBitsToFloat(intBytes);
		i += 4;
		
		intBytes = (int)(((data[i] & 0xFF) << 24) + ((data[i+1] & 0xFF) << 16) +
				((data[i+2] & 0xFF) << 8) + (data[i+3]));
		_zoomCenter[2] = Float.intBitsToFloat(intBytes);
		i += 4;
	}

	public float getZoomScale() {
		return _zoomScale;
	}

	public float[] getZoomCenter() {
		return _zoomCenter;
	}

}
