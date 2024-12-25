#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/CameraUi.h"
#include "Osc.h"

using namespace ci;
using namespace cinder;
using namespace ci::app;
using namespace std;

class ToroidalApp : public App {
public:
	ToroidalApp();
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

	static void prepareSettings(App::Settings* settings);

	osc::UdpSocketRef	mUdpConnection;
	osc::ReceiverUdp	mUdpClient;
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

	/** UDP/Osc/Network ***************************************/


	// member functions
	void setupCamera();
	void setupScene();
	void setupColors(std::vector<vec3>& colorVector, size_t count);
};