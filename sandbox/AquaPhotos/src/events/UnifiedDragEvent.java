package events;

public class UnifiedDragEvent extends UnifiedEvent {
	private float _dx;
	private float _dy;
	private float _dz;
	
	protected UnifiedDragEvent() {
	}
	
	public UnifiedDragEvent(byte[] data) {
		super(data);
		int i = _name.length() + _description.length() + 2 + 17;
		System.out.println("Name : " + _name.length() + ", Desc: " + _description.length());
		int intBytes = (int)(((data[i] & 0xFF) << 24) + ((data[i+1] & 0xFF) << 16) +
				((data[i+2] & 0xFF) << 8) + (data[i+3]));
		_dx = Float.intBitsToFloat(intBytes);
		System.out.println("dx: " + _dx);
		i += 4;
		
		intBytes = (int)(((data[i] & 0xFF) << 24) + ((data[i+1] & 0xFF) << 16) +
				((data[i+2] & 0xFF) << 8) + (data[i+3]));
		_dy = Float.intBitsToFloat(intBytes);
		System.out.println("dy: " + _dy);
		i += 4;
		
		intBytes = (int)(((data[i] & 0xFF) << 24) + ((data[i+1] & 0xFF) << 16) +
				((data[i+2] & 0xFF) << 8) + (data[i+3]));
		_dz = Float.intBitsToFloat(intBytes);
		i += 4;
	}

	public float get_dx() {
		return _dx;
	}

	public float get_dy() {
		return _dy;
	}

	public float get_dz() {
		return _dz;
	}
	
	
}
