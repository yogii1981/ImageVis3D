// Disable checked iterators on Windows.
#ifndef _DEBUG
# undef _SECURE_SCL
# define _SECURE_SCL 0
#endif

#include <StdTuvokDefines.h>
#include <tclap/CmdLine.h>
#include "Renderer/GL/GLRenderer.h"
#include "IO/IOManager.h"
#include "GLContext.h"
#include "SmallImage.h"

using namespace tuvok;


#define SHADER_PATH "Shaders"

bool SaveFBOToDisk(const std::string& filename) 
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

  SmallImage s(viewport[2], viewport[3], 4);
  boost::uint8_t* pixels = s.GetDataPtrRW();

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, viewport[2], viewport[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glReadBuffer(GL_BACK);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, viewport[2], viewport[3], 0); // faster than glReadPixels, so use this to copy from backbuffer
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);

  return s.SaveToBMPFile(filename);
}

int main(int argc, char * argv[])
{
	std::string filename;
	try 
	{
		TCLAP::CmdLine cmd("rendering test program");
		TCLAP::ValueArg<std::string> dset("d", "dataset", "Dataset to render.", true, "", "filename");
		cmd.add(dset);
		cmd.parse(argc, argv);
		filename = dset.getValue();
	} 
	catch (const TCLAP::ArgException & e)
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << "\n";
		return EXIT_FAILURE;
	}

	try
	{
		GLContext context(1920, 1200, 32, 24, 8, true, &std::wcerr);
		if (!context.isValid() || !context.set()) return EXIT_FAILURE;

		GLenum err = glewInit();
		if (err != GLEW_OK) 
		{
			std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << "\n";
			return EXIT_FAILURE;
		}
		
		Controller::Instance().DebugOut()->SetOutput(true, true, true, true);

		// Convert the data into a UVF if necessary 
    if ( SysTools::ToLowerCase(SysTools::GetExt(filename)) != "uvf" ) {
		  std::string uvf_file = SysTools::RemoveExt(filename) + ".uvf";
		  const std::string tmpdir = "/tmp/";
		  const bool quantize8 = false;
		  Controller::Instance().IOMan()->ConvertDataset(filename, uvf_file, tmpdir, true, 256, 4, quantize8);
      filename = uvf_file;
    }
		AbstrRenderer * renderer = Controller::Instance().RequestNewVolumeRenderer(MasterController::OPENGL_SBVR, 
																				false, false, false, false, false);
	
    renderer->LoadDataset(filename);
		renderer->AddShaderPath(SHADER_PATH);
		renderer->Resize(UINTVECTOR2(1920, 1200));
		renderer->Initialize();
		const std::vector<RenderRegion*> & rr = renderer->GetRenderRegions();
    renderer->SetRendererTarget(AbstrRenderer::RT_HEADLESS);

    FLOATMATRIX4 rm;
		rm.RotationX(45.0);
		renderer->SetRotation(rr[0], rm);
		renderer->Paint();
		SaveFBOToDisk("image.bmp");   

		renderer->Cleanup();
		Controller::Instance().ReleaseVolumeRenderer(renderer);
		context.restorePrevious();
	} 
	catch (const std::exception & e) 
	{
		std::cerr << "Exception: " << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
