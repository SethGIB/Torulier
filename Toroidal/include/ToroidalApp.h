#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/CameraUi.h"

using namespace ci;
using namespace cinder;
using namespace ci::app;
using namespace std;

const int RES_AXIS = 8;
const int RES_HEIGHT = 8;
const float RES_S_RADIUS = .175f;
const int RES_S_SUBD = 16;
const vec2 RES_RADII(6.f, 4.f);

class ToroidalApp : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

	static void prepareSettings(App::Settings* settings);

private:
	/** Scene/GL **********************************************/
	// View
	CameraPersp			mCamera;
	CameraUi			mCameraControl; //Mouse camera driver

	// Scene
	gl::BatchRef		mPreviewMeshBatch;
	gl::BatchRef		mInstancedMeshesBatch;

	gl::GlslProgRef		mGlslMesh;
	gl::GlslProgRef		mGlslInstance;

	// Instance Setup
	geom::BufferLayout	mPositionsBuffer;
	gl::VboMeshRef		mInstanceMesh;
	gl::VboRef			mInstanceData;
	std::vector<vec3>	mColorSrc;

	// Control Matrix
	vec3 mCtrlMatrix[RES_AXIS+1][RES_HEIGHT+1];

	/** UDP/Osc/Network ***************************************/


	// member functions
	void setupCamera();
	void setupScene();
	void getPositions(gl::VboRef sourceBuffer, std::vector<vec3>& positionVector, size_t count);
	void setupColors(std::vector<vec3>& colorVector, size_t count);

	// patterns
	void patternChessboard();
	void patternRings();
	void patternCircles();
	void patternSparkle();
};