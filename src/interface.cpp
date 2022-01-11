#include "interface.h"

interface::interface()
{
	// get all subdefinitions of saft objects as pointers
	trans = recon.get_ptrans(); // pointer to transducer class
	sett = recon.get_psett(); // pointer to reconstruction settings
	inputDataVol = recon.get_ppreprocData(); // pointer to preprocessed volume
	reconDataVol = recon.get_preconData(); // pointer to reconstructed volume
}

// displays a small help marker next to the text
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	return;
}

void interface::InitWindow(int *argcp, char**argv)
{

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
	  printf("Error: %s\n", SDL_GetError());
		return;
	}
	// main_display_function goes somewhere here
	const char* glsl_version = "#version 140";
	// to find out which glsl version you are using, run glxinfo from terminal
	// and look for "OpenGL shading language version string"
	// https://en.wikipedia.org/wiki/OpenGL_Shading_Language

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow(windowTitle, 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1900, 1080, window_flags);
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	bool err = glewInit() != GLEW_OK;
	if (err)
	{
		printf("Failed to initialize OpenGL loader!");
	  throw "FailedOpenGLLoader";
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);

	ImGui_ImplOpenGL3_Init(glsl_version);
	bool done = false;
	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
		}
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		MainDisplayCode();
		// Rendering
		ImGui::Render();
		
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
 	SDL_DestroyWindow(window);
 	SDL_Quit();
	return;
}

// window to define all the transducer settings
void interface::TransducerWindow()
{
	ImGui::Begin("Transducer properties", &show_transducer_window);

	ImGui::Columns(1);
	ImGui::InputFloat("Focal distance [mm]", trans->get_pfocalDistance());
	ImGui::SameLine(); HelpMarker("Curvature radius of sensitive element, defines z level of focal plane");
	ImGui::InputFloat("Aperture radius [mm]", trans->get_prAperture());
	ImGui::SameLine(); HelpMarker("Used to define the size of the cone where we reconstruct");
	ImGui::InputFloat("Hole radius [mm]", trans->get_rHole());
	ImGui::SameLine(); HelpMarker("So far unused, lateron for sensnitivity field building.");
	ImGui::End();

	return;
}


// defining all the reconstruction settings
void interface::SettingsWindow()
{
	ImGui::Begin("Reconstruction settings", &show_settings_window);
	
	// cropping applied along x y and t
	float tCropMicro[2];
	tCropMicro[0] = sett->get_cropTMin() * 1e3; // stored as ms, sett as micros
	tCropMicro[1] = sett->get_cropTMax() * 1e3; // stored as ms, sett as micros
	ImGui::InputFloat2("t cropping [micros]", &tCropMicro[0]);
	sett->set_crop(0, tCropMicro[0] * 1e-3, tCropMicro[1] * 1e-3); // store as ms
	ImGui::InputFloat2("x cropping [mm]", sett->get_pcropX());
	ImGui::InputFloat2("y cropping [mm]", sett->get_pcropY());
	float rMinMm = sett->get_rMin() * 1e3;
	ImGui::InputFloat("min. r_recon [mm]", &rMinMm);
	sett->set_rMin(rMinMm * 1e-3);
	ImGui::SameLine(); HelpMarker("This defines the lower limit which for the radius of the used arcs. Usually this value should be close to the expected diffraction limit");
	ImGui::InputFloat("speed of sound [m/s]", sett->get_psos());
	ImGui::Checkbox("coherence factor weighting", sett->get_pflagCoherenceW());
	ImGui::SameLine(); HelpMarker("If this option is enabled, each reconstructed voxel will be weighted by the wave coherence. Can improve resolution and SNR.");
	ImGui::Checkbox("sensitivity field weighting", sett->get_pflagSensW());
	ImGui::Checkbox("pulse-echo mode", sett->get_pflagUs());
	ImGui::SameLine(); HelpMarker("While this program was originally developed to be used for OA image reconstruction it can also be applied to US pulse echo measurements");
	ImGui::Checkbox("GPU", sett->get_pflagGpu());
	ImGui::SameLine(); HelpMarker("Defines if we want to run the code on CPU or GPU"); 
	ImGui::End();
}

// interfacing window to conveniently load data from h5 files
void interface::DataLoaderWindow()
{
	ImGui::Begin("Data Loader", &show_data_loader);

	if (ImGui::Button("Load data"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", 
			"Choose File", ".h5\0.mat\0", ".");
	}

	if (ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey")) 
	{
		if (ImGuiFileDialog::Instance()->IsOk == true)
		{
			std::string inputFilePath = ImGuiFileDialog::Instance()->GetFilepathName();
			inputDataVol->readFromFile(inputFilePath);
			inputDataVol->calcMinMax();
			isDataSetDefined = 1; // set flag to data loaded
			
			// after data loading, update the cropping values 
			for (uint8_t iDim = 0; iDim < 3; iDim++)
			{
				sett->set_crop(iDim, 
					inputDataVol->get_minPos(iDim)*1e3,
					inputDataVol->get_maxPos(iDim)*1e3);
			}
			zCropRaw[0] = inputDataVol->get_minPos(0);
			zCropRaw[1] = inputDataVol->get_maxPos(0);
			xCropRaw[0] = inputDataVol->get_minPos(1);
			xCropRaw[1] = inputDataVol->get_maxPos(1);
			yCropRaw[0] = inputDataVol->get_minPos(2);
			yCropRaw[1] = inputDataVol->get_maxPos(2);
		}
		ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
	}

	if (isDataSetDefined)
	{
		if (ImGui::CollapsingHeader("Data information"))
		{
			ImGui::Columns(2);	
			// print information about dataset if defined
			ImGui::Text("Size"); ImGui::NextColumn();
			
			ImGui::Text("%i x %i x %i", 
				inputDataVol->get_dim(0), 
				inputDataVol->get_dim(1), 
				inputDataVol->get_dim(2)); 
			ImGui::NextColumn();

			ImGui::Text("Resolution [microm]"); ImGui::NextColumn();
			ImGui::Text("%.2f x %.2f", 
				inputDataVol->get_res(1) * 1e6, 
				inputDataVol->get_res(2) * 1e6);
			ImGui::NextColumn();
			
			ImGui::Text("Sampling freqeucny [MHz]"); ImGui::NextColumn();
			ImGui::Text("%.2f",	1 / inputDataVol->get_res(0) / 1e6);
			ImGui::NextColumn();

			ImGui::Text("t range [micros]"); ImGui::NextColumn();
			ImGui::Text("%.2f ... %.2f", 
				inputDataVol->get_minPos(0) * 1e6, 
				inputDataVol->get_maxPos(0) * 1e6);
			ImGui::NextColumn();

			ImGui::Text("x range [mm]"); ImGui::NextColumn();
			ImGui::Text("%.2f ... %.2f", 
				inputDataVol->get_minPos(1) * 1e3, 
				inputDataVol->get_maxPos(1) * 1e3);
			ImGui::NextColumn();

			ImGui::Text("y range [mm]"); ImGui::NextColumn();
			ImGui::Text("%.2f ... %.2f", 
				inputDataVol->get_minPos(2) * 1e3, 
				inputDataVol->get_maxPos(2) * 1e3);
			ImGui::NextColumn();

			ImGui::Text("Maximum value"); ImGui::NextColumn();
			ImGui::Text("%f", inputDataVol->getMinVal());
			ImGui::NextColumn();

			ImGui::Text("Minimum value"); ImGui::NextColumn();
			ImGui::Text("%f", inputDataVol->getMaxVal());
			ImGui::NextColumn();
			ImGui::Columns(1);
		}

		if (ImGui::CollapsingHeader("Raw Data slicer"))
		{
			ImGui::SliderInt("zLayer", &currSliceZ, 0, inputDataVol->get_dim(0) - 1);
			float tSlice = inputDataVol->get_pos(currSliceZ, 0);
			ImGui::Text("Time of current layer: %f micros", tSlice * 1e6);
			ImGui::Text("Approximated z layer: %f mm", tSlice * sett->get_sos());
			ImImagesc(inputDataVol->get_psliceZ((uint64_t) currSliceZ),
				inputDataVol->get_dim(1), inputDataVol->get_dim(2), &inDataTexture, inDataMapper);
			
			int width = 550;
			int height = (float) width / inputDataVol->get_length(1) * inputDataVol->get_length(2);
			ImGui::Image((void*)(intptr_t)inDataTexture, ImVec2(width, height));
			
			ImGui::SliderInt("yLayer", &currSliceY, 0, inputDataVol->get_dim(2) - 1);

			ImImagesc(inputDataVol->get_psliceY((uint64_t) currSliceY),
			 	inputDataVol->get_dim(1), inputDataVol->get_dim(0), &inDataTextureSlice, inDataMapper);
			height = (float) width / inputDataVol->get_length(1) * inputDataVol->get_length(0) * sett->get_sos();
			ImGui::Image((void*)(intptr_t)inDataTextureSlice, ImVec2(width, height));
			
			ImGui::SliderFloat("MinVal", inDataMapper.get_pminVal(), inputDataVol->get_minVal(), inputDataVol->get_maxVal(), "%.1f");
			ImGui::SliderFloat("MaxVal", inDataMapper.get_pmaxVal(), inputDataVol->get_minVal(), inputDataVol->get_maxVal(), "%.1f");
			ImGui::ColorEdit4("Min color", inDataMapper.get_pminCol(), ImGuiColorEditFlags_Float);
			ImGui::ColorEdit4("Max color", inDataMapper.get_pmaxCol(), ImGuiColorEditFlags_Float);
		}

		if (ImGui::CollapsingHeader("MIP preview"))
		{	

			zCropRawMm[0] = zCropRaw[0] * 1e6;
			zCropRawMm[1] = zCropRaw[1] * 1e6;
			xCropRawMm[0] = xCropRaw[0] * 1e3;
			xCropRawMm[1] = xCropRaw[1] * 1e3;
			yCropRawMm[0] = yCropRaw[0] * 1e3;
			yCropRawMm[1] = yCropRaw[1] * 1e3;

			// read in users idea of z/x/y cropping
			ImGui::SliderFloat2("t crop [micros]", 
				&zCropRawMm[0], inputDataVol->get_minPos(0) * 1e6, inputDataVol->get_maxPos(0) * 1e6);
			ImGui::SliderFloat2("x crop lower [mm]", 
				&xCropRawMm[0], inputDataVol->get_minPos(1) * 1e3, inputDataVol->get_maxPos(1) * 1e3);
			ImGui::SliderFloat2("y crop upper [mm]", 
				&yCropRawMm[0], inputDataVol->get_minPos(2) * 1e3, inputDataVol->get_maxPos(2) * 1e3);
			ImGui::SliderFloat("z stretch", &zStretchRaw, 0.5, 10);
			
			zCropRaw[0] = zCropRawMm[0] * 1e-6;
			zCropRaw[1] = zCropRawMm[1] * 1e-6;
			xCropRaw[0] = xCropRawMm[0] * 1e-3;
			xCropRaw[1] = xCropRawMm[1] * 1e-3;
			yCropRaw[0] = yCropRawMm[0] * 1e-3;
			yCropRaw[1] = yCropRawMm[1] * 1e-3;

			inputDataVol->set_cropRangeZ(&zCropRaw[0]);
			inputDataVol->set_cropRangeX(&xCropRaw[0]);
			inputDataVol->set_cropRangeY(&yCropRaw[0]);

			// update cropped mips if something changed
			if (inputDataVol->get_updatedCropRange())
				inputDataVol->calcCroppedMips();

			const float xStart = (xCropRaw[0] - inputDataVol->get_minPos(1)) / inputDataVol->get_length(1);
			const float xEnd = (xCropRaw[1] - inputDataVol->get_minPos(1)) / inputDataVol->get_length(1);
			const float yStart = (yCropRaw[0] - inputDataVol->get_minPos(2)) / inputDataVol->get_length(2);
			const float yEnd = (yCropRaw[1] - inputDataVol->get_minPos(2)) / inputDataVol->get_length(2); 

			const int width = 550;
			const int height = (float) (width) / (inputDataVol->get_length(1) * (xEnd - xStart)) * 
				(inputDataVol->get_length(2) * (yEnd - yStart));

			// plot MIP alonmg z
			ImImagesc(
				inputDataVol->get_croppedMipZ(), 
				inputDataVol->get_dim(1), 
				inputDataVol->get_dim(2), 
				&inDataMipZ, 
				mipRawMapper);
			ImGui::Image((void*)(intptr_t)inDataMipZ, 
				ImVec2(width, height),
				ImVec2(xStart, yStart), // lower corner to crop
				ImVec2(xEnd, yEnd)); // upper corner to crop

			ImImagesc(
				inputDataVol->get_croppedMipY(), 
				inputDataVol->get_dim(1), 
				inputDataVol->get_dim(0), 
				&inDataMipY, 
				mipRawMapper);

			// plot MIP along Y
			const float zStart = (zCropRaw[0] - inputDataVol->get_minPos(0)) / inputDataVol->get_length(0);
			const float zEnd = (zCropRaw[1] - inputDataVol->get_minPos(0)) / inputDataVol->get_length(0); 			
		
			const int height2 = ((float) width) / (inputDataVol->get_length(1) * (xEnd - xStart)) 
				* (inputDataVol->get_length(0) * sett->get_sos() * (zEnd - zStart)) * zStretchRaw;
			// printf("width: %d, height %d\n", width, height2);

			ImGui::Image((void*)(intptr_t)inDataMipY, 
				ImVec2(width, height2), 
				ImVec2(xStart, zStart), // lower corner to crop
				ImVec2(xEnd, zEnd)); // upper corner to crop

			// printf("min and max val crop: %f, %f\n", 
			//		inputDataVol->get_minValCrop(),
			//		inputDataVol->get_maxValCrop());

			ImGui::SliderFloat("MinVal", mipRawMapper.get_pminVal(), inputDataVol->get_minValCrop(), inputDataVol->get_maxValCrop(), "%.1f");
			ImGui::SliderFloat("MaxVal", mipRawMapper.get_pmaxVal(), inputDataVol->get_minValCrop(), inputDataVol->get_maxValCrop(), "%.1f");
			ImGui::ColorEdit4("Min color", mipRawMapper.get_pminCol(), ImGuiColorEditFlags_Float);
			ImGui::ColorEdit4("Max color", mipRawMapper.get_pmaxCol(), ImGuiColorEditFlags_Float);
		}
				
	}

	ImGui::End();

	return;
}

// helper function to display stuff
void interface::ImImagesc(
	const float* data, const uint64_t sizex, const uint64_t sizey, 
	GLuint* out_texture, const color_mapper myCMap)
{
	
	glDeleteTextures(1, out_texture);

	// Create an OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
	// use color transfer function to convert from float to rgba
	unsigned char* data_conv = new unsigned char[4 * sizex * sizey];
	myCMap.convert_to_map(data, sizex * sizey, data_conv);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizex, sizey, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_conv);

	// give pointer back to main program
	*out_texture = image_texture;
	delete[] data_conv; // free memory for temporary array
	return;
}

// starts the reconstruction and takes care of previews of reconstructed volumes
void interface::ReconWindow()
{
	ImGui::Begin("Reconstruction", &show_recon_window);

	if (!isDataSetDefined || recon.get_isRunning())
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}
	
	if (!recon.get_isRunning())
	{
		if (isReconRunning)
		{
			reconThread.join();
			// if this is the first reconstruction we have done, update the cropping
			if (isReconDone == 0)
			{
				zCropRecon[0] = reconDataVol->get_minPos(0);
				zCropRecon[1] = reconDataVol->get_maxPos(0);
				xCropRecon[0] = reconDataVol->get_minPos(1);
				xCropRecon[1] = reconDataVol->get_maxPos(1);
				yCropRecon[0] = reconDataVol->get_minPos(2);
				yCropRecon[1] = reconDataVol->get_maxPos(2);
			}

			isReconDone = 1;
			isReconRunning = 0;

			// recalculate cropped mips and update value limits
			reconDataVol->calcCroppedMips();
			reconMipMapper.set_minVal(reconDataVol->get_minValCrop());
			reconMipMapper.set_maxVal(reconDataVol->get_maxValCrop());
			printf("Finished reconstruction procedure!\n");
		}

		// show reconstruct button if dataset was defined
		if (!isDataSetDefined)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::Button("Reconstruct"))
		{
			printf("Starting reconstruction procedure...\n");
			// recon.recon();
			isReconRunning = 1;
			saft* reconPtr = &recon; // get pointer to our reconstruction procedure
			reconThread = reconPtr->recon2thread();
			isReconRunning = 1;
		}

		if (!isDataSetDefined)
		{
			ImGui::PopItemFlag();
	    ImGui::PopStyleVar();
		}

	}
	else // here we simply show a disabled button and a progress bar
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		if (ImGui::Button("Reconstruct"))
		{
			// nothing to be done
		}
		ImGui::PopItemFlag();
	  ImGui::PopStyleVar();

		ImGui::Text("Reconstruction in progress...");
		time_t tt;
		tt = system_clock::to_time_t(recon.get_tStart());

		ImGui::Text("Started reconstruction at: %s\n", ctime(&tt));
		ImGui::Text("ETA: %.1f seconds", recon.get_tRemain());
		ImGui::ProgressBar(recon.get_percDone() / 100);
	}

	

	if (isReconDone) // this means that the reconstruction was performed at least once
	{
		ImGui::Text("Last reconstruction took %.1f seconds.", recon.get_reconTime());
		
		// slice preview through reconstruction
		if (ImGui::CollapsingHeader("Slice preview recon"))
		{
			// show some information about current depth
			ImGui::SliderInt("zLayer recon slice [ID]", &currSliceZRecon, 0, reconDataVol->get_dim(0) - 1);
			ImGui::SliderInt("yLayer recon slice [ID]", &currSliceYRecon, 0, reconDataVol->get_dim(2) - 1);
			const float zSlice = reconDataVol->get_pos(currSliceZRecon, 0);
			ImGui::Text("Depth of recon layer: %f mm", zSlice);

			// generate the actual image along z normal plane
			ImImagesc(
				reconDataVol->get_psliceZ((uint64_t) currSliceZRecon),
				reconDataVol->get_dim(1), 
				reconDataVol->get_dim(2), 
				&reconSliceZ, 
				reconDataMapper);
			
			const int width = 550;
			const int height = (float) width / reconDataVol->get_length(1) * 
				reconDataVol->get_length(2);
			ImGui::Image((void*)(intptr_t)reconSliceZ, ImVec2(width, height));
			
			// generate the actual image along y normal plane
			ImImagesc(
				reconDataVol->get_psliceY((uint64_t) currSliceYRecon),
			 	reconDataVol->get_dim(1), 
			 	reconDataVol->get_dim(0), 
			 	&reconSliceY, 
			 	reconDataMapper);

			const int height2 = (float) width / reconDataVol->get_length(1) * reconDataVol->get_length(0);
			ImGui::Image((void*)(intptr_t)reconSliceY, ImVec2(width, height2));
			
			ImGui::SliderFloat("Min val recon slice", reconDataMapper.get_pminVal(), 
				reconDataVol->get_minVal(), reconDataVol->get_maxVal(), "%.1f");
			ImGui::SliderFloat("Max val recon slice", reconDataMapper.get_pmaxVal(), 
				reconDataVol->get_minVal(), reconDataVol->get_maxVal(), "%.1f");
			ImGui::ColorEdit4("Min color recon slice", reconDataMapper.get_pminCol(), 
				ImGuiColorEditFlags_Float);
			ImGui::ColorEdit4("Max color recon slice", reconDataMapper.get_pmaxCol(), 
				ImGuiColorEditFlags_Float);
		}

		if (ImGui::CollapsingHeader("MIP preview recon"))
		{

			zCropReconMm[0] = zCropRecon[0] * 1e3;
			zCropReconMm[1] = zCropRecon[1] * 1e3;
			xCropReconMm[0] = xCropRecon[0] * 1e3;
			xCropReconMm[1] = xCropRecon[1] * 1e3;
			yCropReconMm[0] = yCropRecon[0] * 1e3;
			yCropReconMm[1] = yCropRecon[1] * 1e3;

			// read in users idea of z/x/y cropping
			ImGui::SliderFloat2("z crop recon [mm]", 
				&zCropReconMm[0], reconDataVol->get_minPos(0) * 1e3, reconDataVol->get_maxPos(0) * 1e3);
			ImGui::SliderFloat2("x crop recpm [mm]", 
				&xCropReconMm[0], reconDataVol->get_minPos(1) * 1e3, reconDataVol->get_maxPos(1) * 1e3);
			ImGui::SliderFloat2("y crop recon [mm]", 
				&yCropReconMm[0], reconDataVol->get_minPos(2) * 1e3, reconDataVol->get_maxPos(2) * 1e3);
			ImGui::SliderFloat("z stretch recon", &zStretchRecon, 0.5, 10);
			
			zCropRecon[0] = zCropReconMm[0] * 1e-3;
			zCropRecon[1] = zCropReconMm[1] * 1e-3;
			xCropRecon[0] = xCropReconMm[0] * 1e-3;
			xCropRecon[1] = xCropReconMm[1] * 1e-3;
			yCropRecon[0] = yCropReconMm[0] * 1e-3;
			yCropRecon[1] = yCropReconMm[1] * 1e-3;

			// check if all are in a good valid order, otherwise make sure that they are sorted
			if (zCropRecon[0] > zCropRecon[1])
			{
				const float midValZ = (zCropRecon[0] + zCropRecon[1]) / 2;
				zCropRecon[0] = midValZ; zCropRecon[1] = midValZ;
			}

			if (xCropRecon[0] > xCropRecon[1])
			{
				const float midValX = (xCropRecon[0] + xCropRecon[1]) / 2;
				xCropRecon[0] = midValX; xCropRecon[1] = midValX;
			}

			if (yCropRecon[0] > yCropRecon[1])
			{
				const float midValY = (yCropRecon[0] + yCropRecon[1]) / 2;
				yCropRecon[0] = midValY; yCropRecon[1] = midValY;
			}

			reconDataVol->set_cropRangeZ(&zCropRecon[0]);
			reconDataVol->set_cropRangeX(&xCropRecon[0]);
			reconDataVol->set_cropRangeY(&yCropRecon[0]);

			// update cropped mips if something changed
			if (reconDataVol->get_updatedCropRange())
				reconDataVol->calcCroppedMips();

			const float xStart = (xCropRecon[0] - reconDataVol->get_minPos(1)) / reconDataVol->get_length(1);
			const float xEnd = (xCropRecon[1] - reconDataVol->get_minPos(1)) / reconDataVol->get_length(1);
			const float yStart = (yCropRecon[0] - reconDataVol->get_minPos(2)) / reconDataVol->get_length(2);
			const float yEnd = (yCropRecon[1] - reconDataVol->get_minPos(2)) / reconDataVol->get_length(2); 

			const int width = 550;
			const int height = (float) (width) / (reconDataVol->get_length(1) * (xEnd - xStart)) * 
				(reconDataVol->get_length(2) * (yEnd - yStart));

			// plot MIP with normal axis along z
			ImImagesc(
				reconDataVol->get_croppedMipZ(), 
				reconDataVol->get_dim(1), 
				reconDataVol->get_dim(2), 
				&reconMipZ, 
				reconMipMapper);

			ImGui::Image((void*)(intptr_t)reconMipZ, 
				ImVec2(width, height),
				ImVec2(xStart, yStart), // lower corner to crop
				ImVec2(xEnd, yEnd)); // upper corner to crop

			ImImagesc(
				reconDataVol->get_croppedMipY(), 
				reconDataVol->get_dim(1), 
				reconDataVol->get_dim(0), 
				&reconMipY, 
				reconMipMapper);

			// plot MIP along Y
			const float zStart = (zCropRecon[0] - reconDataVol->get_minPos(0)) / reconDataVol->get_length(0);
			const float zEnd = (zCropRecon[1] - reconDataVol->get_minPos(0)) / reconDataVol->get_length(0); 			
		
			const int height2 = ((float) width) / (reconDataVol->get_length(1) * (xEnd - xStart)) 
				* (reconDataVol->get_length(0) * (zEnd - zStart)) * zStretchRecon;
			// printf("width: %d, height %d\n", width, height2);

			ImGui::Image((void*)(intptr_t)reconMipY, 
				ImVec2(width, height2), 
				ImVec2(xStart, zStart), // lower corner to crop
				ImVec2(xEnd, zEnd)); // upper corner to crop

			ImGui::SliderFloat("MinVal MIP", reconMipMapper.get_pminVal(), reconDataVol->get_minValCrop(), 
				reconDataVol->get_maxValCrop(), "%.1f");
			ImGui::SliderFloat("MaxVal MIP", reconMipMapper.get_pmaxVal(), reconDataVol->get_minValCrop(), 
				reconDataVol->get_maxValCrop(), "%.1f");
			ImGui::ColorEdit4("Min color MIP", reconMipMapper.get_pminCol(), ImGuiColorEditFlags_Float);
			ImGui::ColorEdit4("Max color MIP", reconMipMapper.get_pmaxCol(), ImGuiColorEditFlags_Float);
		}
		
		// this is not implemented yet but at some point we want to have an option here to export the previews
		if (ImGui::CollapsingHeader("Preview export functions"))
		{
			static char filePath [64];
			ImGui::InputText("Image path", filePath, 64);
			string filePathString = filePath;

			if (ImGui::Button("Export as png"))
			{
				
			}
		}

		if (ImGui::CollapsingHeader("Volumetric export functions"))
		{
			static char filePath [64];
			ImGui::InputText("Volume path", filePath, 64);
			string filePathString = filePath;

			ImGui::Columns(2);
			if (ImGui::Button("Export as vtk"))
			{
				reconDataVol->exportVtk(filePathString);
			}
			ImGui::NextColumn();
			if (ImGui::Button("Export as h5"))
			{
				reconDataVol->saveToFile(filePath);	
			}

		}
	}

	ImGui::End();
	return;
}


// main loop which we run through to update ImGui
void interface::MainDisplayCode()
{
	TransducerWindow();
	SettingsWindow();
	DataLoaderWindow();
	ReconWindow();
	
	return;
}
