

/**
 * GroupIDFactory generates group IDs for SparshComponents and
 * SparshPanels.
 * 
 * @author Jay Roltgen (jroltgen@gmail.com)
 *
 */
public class GroupIDFactory {

	/**
	 * The next ID to return.
	 */
	private static int _nextID = 1;
	
	/**
	 * Generates the next groupID.  This method will return a
	 * unique ID for use as a component's groupID.
	 * 
	 * @return
	 * 		The next group ID.
	 */
	public static int getNextGroupID() {
		return _nextID++;
	}
	
}
