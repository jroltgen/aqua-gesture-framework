

import java.util.List;
import javax.swing.JComponent;
import events.Event;

/**
 * SparshComponent is the base class for any Swing components wishing to be
 * compatible with Sparsh-UI. All custom Sparsh components should extend this
 * class rather than JComponent.
 * 
 * @author Jay Roltgen (jroltgen@gmail.com)
 */
public abstract class SparshComponent extends JComponent {
	/**
	 * Serial version ID for SparshComponent.
	 */
	private static final long serialVersionUID = 6167672416833534358L;

	/**
	 * The Group ID for this component.
	 */
	protected int _groupID;

	/**
	 * Will return the allowed gestures for this component in the form of a
	 * list. See allowed gestures in GestureType.java in the Gesture Server.
	 * 
	 * @return 
	 *		A list of gesture IDs.
	 */
	public abstract List<String> getAllowedGestures(int GroupID);

	/**
	 * Constructs a Sparsh Component. This automatically gets a groupID from the
	 * groupID factory.
	 */
	public SparshComponent() {
		_groupID = GroupIDFactory.getNextGroupID();
		SparshRegistry.addComponent(this);
	}

	/**
	 * Returns the groupID of this Sparsh component.
	 * @return
	 * 		The groupID for this Sparsh component.
	 */
	
	
	public int getGroupID() {
		return _groupID;
	}

	/**
	 * Process an event which is generated for this component
	 * @param arg1
	 * 		The event the component should process.
	 */
	public abstract void processEvent(int groupID, Event arg1);
}