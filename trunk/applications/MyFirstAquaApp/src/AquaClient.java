import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;


public class AquaClient {
	
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
	
	public static void main(String[] args) throws UnknownHostException, IOException {
		AquaClient ac = new AquaClient();
		ac.connect();
		while(true) ac.handleRequest();
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
		_output.write(1);
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
		if (location[0] < 0.5) {
			_output.writeInt(1);
		} else {
			_output.writeInt(0);
		}
	}
	
	/**
	 * Handles the getGlobalInfo message.
	 */
	private void handleGetGlobalInfo() throws IOException {
		_output.writeInt(0);

		// Sent gestures as null-terminated strings.
		//_output.writeBytes("HelloWorldGesture\0");
		
		// Send the events
		_output.writeInt(0);
	}
	
	/**
	 * Handles the getRegionInfo message.
	 */
	private void handleGetRegionInfo() throws IOException {
		int regionID = _input.readInt();
		if (regionID == 1) {
			_output.writeInt(1);
			_output.writeBytes("HelloWorldGesture\0");
			_output.writeInt(0);
		} else {
			_output.writeInt(0);
			_output.writeInt(0);
		}
	}
	
	/**
	 * Handles the processGlobalEvent message.
	 */
	private void processGlobalEvent() throws IOException {
		System.out.println("Got global event.\n");
	}
	
	/**
	 * Handles the processRegionEvent message.
	 */
	private void processRegionEvent() throws IOException {
		System.out.println("Got region event.\n");
	}
	/**
	 * Handles the getTranslators message.
	 */
	private void handleGetTranslators() throws IOException {
		// Send # of translators.
		_output.writeInt(0);
		
	}
	
}
