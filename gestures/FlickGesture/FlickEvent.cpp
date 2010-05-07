
#include <string>
#include "FlickEvent.h"

#ifdef _WIN32
	extern "C" {
		__declspec (dllexport) Event* createEvent(char* data) {
			return new FlickEvent(data);
		}
	}
#else
	extern "C" {
		Event* createEvent(char* data) {
			return new FlickEvent(data);
		}
	}
#endif

//Length of flick event data - simply contains direction
//#define FLICKEVENT_DATA_LENGTH 4

//Not sure how to find/know this length - probably really easy and obvious to others ***


FlickEvent::FlickEvent(char *data) : Event(data) {
		
	//Find current data position
	int dataPos = (_name.length() + _description.length() + 2 + 17);
	    
	//is this hard coded 4 to represent the FLICKEVENT_DATA_LENGTH?
	memcpy(&_flickDirection, &data[dataPos], 4);
		
	    
	// Handle endianness.
	if (EndianConverter::isLittleEndian()) {
		_flickDirection = EndianConverter::swapIntEndian(_flickDirection);
			
	}
};



FlickEvent::FlickEvent(std::string& name, std::string& desc, char type, int id, 
					   float* location, int publishedDirection) : Event(name, desc, type, id, location) {

	_flickDirection = publishedDirection;
}

char* FlickEvent::serializeData(short& outLength) {
	    
	//what is this used for?
	//outLength = FLICKEVENT_DATA_LENGTH;

	int tempDirection;
	        
	tempDirection = _flickDirection;
	    
	//Handle endianness
	if (EndianConverter::isLittleEndian()) {
		tempDirection = EndianConverter::swapIntEndian(tempDirection);
	}
	    
	//is this hard coded 4 to represent the FLICKEVENT_DATA_LENGTH?
	memcpy(_dataBuffer + 0, &tempDirection, 4);
	    
	return _dataBuffer;

}
FlickEvent::~FlickEvent() {
}

//class FlickEvent : public Event {
//
//// Attributes
//private:
//	//direction of Flick
//    int _flickDirection;
//
//    char  _dataBuffer[FLICKEVENT_DATA_LENGTH];
//
//	//Serialize data
//	char* FlickEvent::serializeData(short& outLength) {
//	    
//		//what is this used for?
//		//outLength = FLICKEVENT_DATA_LENGTH;
//
//		int tempDirection;
//	        
//		tempDirection = _flickDirection;
//	    
//		//Handle endianness
//		if (EndianConverter::isLittleEndian()) {
//			tempDirection = EndianConverter::swapIntEndian(tempDirection);
//		}
//	    
//		//is this hard coded 4 to represent the FLICKEVENT_DATA_LENGTH?
//		memcpy(_dataBuffer + 0, &tempDirection, 4);
//	    
//		return _dataBuffer;
//
//	}
//
//	FlickEvent::FlickEvent(char *data) : Event(data) {
//		
//		//Find current data position
//		int dataPos = (_name.length() + _description.length() + 2 + 17);
//	    
//		//is this hard coded 4 to represent the FLICKEVENT_DATA_LENGTH?
//		memcpy(&_flickDirection, &data[dataPos], 4);
//		
//	    
//		// Handle endianness.
//		if (EndianConverter::isLittleEndian()) {
//			_flickDirection = EndianConverter::swapFloatEndian(_flickDirection);
//			
//		}
//	}
//};