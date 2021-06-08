#pragma once

#include <vector>

#include "ofMain.h"
#include "ofxRapidLib.h"
#include "ofxMaxim.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		/* RapidLib */
		bool isRecording;
		double label;
		
		/* Video as input data */
		ofVideoGrabber vidGrabber;
		ofPixels frameData;
		ofTexture data1, data2, data3;
		int vidWidth;
		int vidHeight;
};
