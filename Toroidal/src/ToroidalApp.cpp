#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "ToroidalApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const int RES_AXIS = 16;
const int RES_HEIGHT = 8;
const float RES_S_RADIUS = 0.06f;
const int RES_S_SUBD = 16;
const vec2 RES_RADII(5.f,4.f);

void ToroidalApp::setup()
{
	setupCamera();
	setupScene();
}

void ToroidalApp::mouseDown( MouseEvent event )
{
}

void ToroidalApp::update()
{
}

void ToroidalApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::setMatrices(mCamera);
	
	gl::enableDepthRead();

	gl::setWireframeEnabled(true);
	gl::lineWidth(1.0f);
	mGlslMesh->uniform("uColor", ColorAf(1.f, 0.75f, 0.f, 1.f));
	mPreviewMeshBatch->draw();
	gl::setWireframeEnabled(false);
	
	mGlslInstance->uniform("uColor", ColorAf(0.5f, 0.5f, 0.5f, 1.0f));
	mGlslInstance->uniform("uEyePos", mCamera.getEyePoint());
	gl::pushMatrices();
	gl::scale(vec3(RES_RADII.y));
	mInstancedMeshesBatch->drawInstanced( (RES_AXIS+1)*(RES_HEIGHT+1) );
	gl::popMatrices();
}

void ToroidalApp::prepareSettings(App::Settings* settings)
{
	settings->setWindowSize(1280, 800);
}

void ToroidalApp::setupCamera()
{
	mCamera.setPerspective(60.0f, getWindowAspectRatio(), 0.01f, 1000.0f);
	mCamera.lookAt(vec3(0.0f, 1.0f, 10.0f), vec3(), vec3(0.0f, 1.0f, 0.0f));
	mCameraControl = CameraUi(&mCamera, getWindow());
}

void ToroidalApp::setupScene()
{
	auto previewMeshSrc = geom::Torus().radius(RES_RADII.x, RES_RADII.y).subdivisionsAxis(RES_AXIS).subdivisionsHeight(RES_HEIGHT);

	// Preview mesh
	mGlslMesh = gl::GlslProg::create(loadAsset("shaders/v_toroid.glsl"), loadAsset("shaders/f_toroid.glsl"));
	mPreviewMeshBatch = gl::Batch::create(previewMeshSrc, mGlslMesh);

	// Instances
	auto instanceSrc = geom::Sphere().center(vec3()).radius(RES_S_RADIUS).subdivisions(RES_S_SUBD);
	mGlslInstance = gl::GlslProg::create(loadAsset("shaders/v_instance.glsl"), loadAsset("shaders/f_instance.glsl"));

	gl::VboMeshRef instanceVboMesh = gl::VboMesh::create(instanceSrc);
	gl::VboMeshRef positionsSrc = mPreviewMeshBatch->getVboMesh();
	auto arrayVbos = positionsSrc->getVertexArrayVbos();

	geom::BufferLayout bufferLayout;
	bufferLayout.append(geom::Attrib::CUSTOM_0, 3, 0, 0, 1);

	instanceVboMesh->appendVbo(bufferLayout, arrayVbos[0]);
	mInstancedMeshesBatch = gl::Batch::create(instanceVboMesh, mGlslInstance, { {geom::Attrib::CUSTOM_0, "vInstancePosition"} });
}

CINDER_APP( ToroidalApp, RendererGl, *ToroidalApp::prepareSettings )
