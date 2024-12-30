#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "ToroidalApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;



ToroidalApp::ToroidalApp() : mUdpClient(10001)
{
}

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
	mGlslMesh->uniform("uColor", ColorAf(1.f, 1.f, 1.f, 1.f));
	mPreviewMeshBatch->draw();
	gl::setWireframeEnabled(false);
	
	mGlslInstance->uniform("uColor", ColorAf(.25f, .25f, .25f, 1.0f));
	mGlslInstance->uniform("uEyePos", mCamera.getEyePoint());
	gl::pushMatrices();
	gl::scale(vec3(RES_RADII.y));
	mInstancedMeshesBatch->drawInstanced( (RES_AXIS+1)*(RES_HEIGHT+1) );
	gl::popMatrices();
}

void ToroidalApp::prepareSettings(App::Settings* settings)
{
	settings->setWindowSize(900,900);
}

void ToroidalApp::setupCamera()
{
	mCamera.setPerspective(60.0f, getWindowAspectRatio(), 0.01f, 1000.0f);
	mCamera.lookAt(vec3(0.0f, 2.5f, 15.0f), vec3(), vec3(0.0f, 1.0f, 0.0f));
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

	// Positions
	gl::VboMeshRef positionsSrc = mPreviewMeshBatch->getVboMesh();
	auto arrayVbos = positionsSrc->getVertexArrayVbos();
	geom::BufferLayout layoutPosition;
	layoutPosition.append(geom::Attrib::CUSTOM_0, 3, 0, 0, 1);
	instanceVboMesh->appendVbo(layoutPosition, arrayVbos[0]);

	// Colors
	setupColors(mColorSrc, (RES_AXIS+1)*(RES_HEIGHT+1));
	gl::VboRef colorVbo = gl::Vbo::create(GL_ARRAY_BUFFER, mColorSrc.size() * sizeof(vec3), mColorSrc.data(), GL_DYNAMIC_DRAW);
	geom::BufferLayout layoutColor;
	layoutColor.append(geom::Attrib::CUSTOM_1, 3, 0, 0, 1);
	instanceVboMesh->appendVbo(layoutColor, colorVbo);

	mInstancedMeshesBatch = gl::Batch::create(instanceVboMesh, mGlslInstance, { {geom::Attrib::CUSTOM_0, "vInstancePosition"}, {geom::Attrib::CUSTOM_1, "vInstanceColor"} });
}

void ToroidalApp::setupColors(std::vector<vec3> &colorVector, size_t count)
{
	colorVector.clear();
	for (size_t i = 0; i < count; i++)
	{
		if (i >= 0 && i <= 11)
			colorVector.push_back(vec3(1.0f));
		else
		{
			if (i % 2 == 0 && i > 11)
				colorVector.push_back(vec3(1.f, 0.f, 0.f));
			else
				colorVector.push_back(vec3(0.f, 1.f, 0.f));
		}
	}
}

void ToroidalApp::patternChessboard()
{
}

void ToroidalApp::patternRings()
{
}

void ToroidalApp::patternCircles()
{
}

void ToroidalApp::patternSparkle()
{
}

CINDER_APP( ToroidalApp, RendererGl, ToroidalApp::prepareSettings )
