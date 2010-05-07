#include "FlickGesture.h"
#include "FlickEvent.h"
//#include "../../events/FlickEvent.h"
#include <time.h>
#include <stdio.h>
#include <math.h>

using namespace std;

#ifdef _WIN32
extern "C" {
    __declspec (dllexport) Gesture* createGesture(EventProcessor* 
            publisher, int regionID) {
                return new FlickGesture(publisher, regionID);
        }
}
#else
extern "C" {
    Gesture* createGesture(EventProcessor* publisher, int regionID) {
                return new FlickGesture(publisher, regionID);
        }
}
#endif

using namespace std;

FlickGesture::FlickGesture(EventProcessor* publisher, 
        int regionID) : UnifiedStandardDynamicGesture(publisher, regionID) {
}

bool FlickGesture::processDown(TouchData& touchData) {

    //set touch "down" points
	flickDownXYZ[0] = touchData.getX();
	flickDownXYZ[1] = touchData.getY();
	flickDownXYZ[2] = touchData.getZ();

	//get time for touch down
          
    GetSystemTime(&flickDownTime);
      
    return false;
}

bool FlickGesture::processMove(TouchData& touchData){
    

	//Nothing of relevance happens with a move
	//(with respect to flicks)
	//Flicks are triggered by down->move->up
	//Therefore, nothing happens for moves

    return false;
}

bool FlickGesture::processUp(TouchData& touchData){   
    
	//get XYZ coordinates for "up" movement
	flickUpXYZ[0] = touchData.getX();
	flickUpXYZ[1] = touchData.getY();
	flickUpXYZ[2] = touchData.getZ();

	//get time for UP move
    GetSystemTime(&flickUpTime);
	
	//find flick direction or if flick exists
	//determineIfFlick returns 0 if no flick found
	int flickDirection = determineIfFlick(touchData);
	
	//only publish a flick if there is one
	if (flickDirection != 0) {
		 
		//call function to publish the flick
		publishFlick(flickDirection);

	}
        return false;
}


int FlickGesture::determineIfFlick(TouchData& touchData){
	
	//logic to determine if there is a flick that has occured
	
	int flickDir = 0;
	double theta, dx, dy, dz;
    double dt = 0;
	double dthr = 0; //hours
	double dtmin = 0; //minutes
	double dtsec = 0; //seconds
	double dtms = 0; //milliseconds
	double flickTimeLengthMin = 0.1; //seconds
	double flickTimeLengthMax = 0.5; //seconds
	double angleVariance = 8;	//percent deviation allowed, [0 100]
	double flickLength = 3;  //percent of movement in a direction required to be flick, [0 100]

    double temp;

	//calculate dt
	dthr = (double)flickUpTime.wHour - (double)flickDownTime.wHour;
	dtmin = (double)flickUpTime.wMinute - (double)flickDownTime.wMinute;
    dtsec = (double)flickUpTime.wSecond - (double)flickDownTime.wSecond;
	dtms = (double)flickUpTime.wMilliseconds - (double)flickDownTime.wMilliseconds;
    
    dt = dtms/1000 + dtsec + dtmin*60 + dthr*3600;
    printf("%%%%%%%%\n downsec %d\n downms %d\n upsec %d\n upms %d\n dt %lf\n ",flickDownTime.wSecond,flickDownTime.wMilliseconds,flickUpTime.wSecond,flickUpTime.wMilliseconds,dt);
	//only do if flick length long enough
	if (dt > flickTimeLengthMin && dt < flickTimeLengthMax)
		{
			//find change in x and y
			dx = flickUpXYZ[0] - flickDownXYZ[0];
			dy = flickUpXYZ[1] - flickDownXYZ[1];
			//dz = p_up(2) - p_down(2);

			//find angle between them
			theta = atan(dy/dx);
			
			//convert angle to [0 2pi] interval

			//second/third quadrant
			if( dx < 0) {
				theta = theta + PI;
			}
			
            
            if (temp >= (PI/4) ) {
                temp = (double)(PI/2) - temp;
                
                }
			//check if within angle specification
			if ( ( temp ) <= (PI/2)*(angleVariance/100) ) {
				
				//check length of flick
				if ( (pow( (pow(dx,(double)2) + pow(dy,(double)2) ), (double)0.5)) >= (flickLength/100) ) {
					
						/*
					flickDir values
					1 = right
					2 = up
					3 = left
					4 = down
						*/

					//flick is to right
					if (theta >= (-PI/4) &&  theta <= (PI/4) ){
						flickDir = 1;
					}
					//flick is up
					if (theta >= (PI/4) &&  theta <= (3*PI/4) ){
						flickDir = 2;
					}
					//flick is to left
					if (theta >= (3*PI/4) &&  theta <= (5*PI/4) ){
						flickDir = 3;

					}//flick is down
					if (theta >= (5*PI/4) &&  theta <= (7*PI/4) ){
						flickDir = 4;
					}
					
				}

			}


		}

	return flickDir;
}

bool FlickGesture::publishFlick(int publishedDirection){
    
    string name("FlickEvent");
    string desc("FlickEvent");

    FlickEvent e(name, desc, EVENT_TYPE_OTHER, 0, _originalCentroid,
            publishedDirection);
        publishEvent(&e);

    return false;
}


	bool printGestureAnalysis(void);

bool FlickGesture::printGestureAnalysis(float dx, float dy, float dt, float theta) {

	////Prints out data regarding flick analysis for testing purposes
	//printf("Flick Analyzed.\n");
	//printf("\tX : %s : \tY : %s : \tZ : %s",

 //   printf("[PrintEventGesture] Received Event.\n");
 //   printf("\t%s : %s\n", event->getName().c_str(), event->getDesc().c_str());
 //   printf("\tType: %s ---", getType(event->getType()).c_str());
 //   printf(" ID: %d\n", event->getID());
 //   printf("\tX: %f\n\tY: %f\n\tZ: %f\n", event->getX(), event->getY(), 
 //           event->getZ());
 //   event->getLocation();

	return false;
}
