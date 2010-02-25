package events;

public class Event {
	protected String _name;
	protected String _description;
	protected int _type;
	protected int _id;
	protected float _location[];
	
	public Event() {
		_location = new float[3];
	}
	
	public Event(byte[] data) {
		int i = 0;
		
		_location = new float[3];
		
		// Get the name.
		_name = "";
		while (data[i] != '\0') {
			_name += (char)data[i++];
		}
		i++;
		
		// Get the description.
		_description = "";
		while (data[i] != '\0') {
			_description += (char)data[i++];
		}
		i++;
		
		// Get the type.
		_type = (int)(data[i] & 0xFF);
		
		// Get the ID.
		_id = (int)(((data[i] & 0xFF) << 24) + ((data[i+1] & 0xFF) << 16) +
				((data[i+2] & 0xFF) << 8) + (data[i+3]));
		i += 4;
		
		// Get the location.
		for (int j = 0; j < 3; j++) {
			int intBytes = (int)(((data[i] & 0xFF) << 24) + ((data[i+1] & 0xFF) << 16) +
					((data[i+2] & 0xFF) << 8) + (data[i+3]));
			_location[j] = Float.intBitsToFloat(intBytes);
			i += 4;
		}
	}

	public String getName() {
		return _name;
	}

	public String getDescription() {
		return _description;
	}

	public int getType() {
		return _type;
	}

	public int getId() {
		return _id;
	}

	public float[] getLocation() {
		return _location;
	}
	
	
}
