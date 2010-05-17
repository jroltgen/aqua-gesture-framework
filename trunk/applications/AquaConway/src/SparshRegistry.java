

import java.util.List;
import java.util.TreeMap;
import java.util.Vector;
import events.Event;

public class SparshRegistry {

	private static TreeMap<Integer, SparshComponent> components = new TreeMap<Integer, SparshComponent>();

	public static void addComponent(SparshComponent c) {
		components.put(c.getGroupID(), c);
	}

	public static List<String> getAllowedGestures(int groupID) {
		SparshComponent c = components.get(groupID);
		if (c != null) {
			return c.getAllowedGestures(groupID);
		} else {
			System.out.println("[SparshRegistry] getAllowedGestures() : GroupID not found: " + groupID);
			return new Vector<String>(0);
		}
	}

	public static void processEvent(int groupID, Event event) {
		SparshComponent c = components.get(groupID);
		if (c != null) {
			c.processEvent(groupID, event);
		} else {
			System.out.println("[SparshRegistry] processEvent() : GroupID not found: " + groupID);
		}
	}

}
