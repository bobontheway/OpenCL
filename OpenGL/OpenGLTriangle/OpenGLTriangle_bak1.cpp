#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/resource.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <utils/Timers.h>

#include <EGLUtils.h>

#include <ui/PixelFormat.h>
#include <ui/Rect.h>
#include <ui/Region.h>
#include <ui/DisplayInfo.h>

#include <gui/ISurfaceComposer.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>

using namespace android;

// Index to bind the attributes to vertex shaders
#define VERTEX_ARRAY    0

#if 0
static void checkEglError(const char* op, EGLBoolean returnVal = EGL_TRUE) {
	if (returnVal != EGL_TRUE) {
		fprintf(stderr, "%s() returned %d\n", op, returnVal);
	}

	for (EGLint error = eglGetError(); error != EGL_SUCCESS;
		error = eglGetError()) {
		fprintf(stderr, "after %s() eglError %s (0x%x)\n",
			op, EGLUtils::strerror(error), error);
	}
}
#endif
static bool checkEglError(const char* op) {
	for (EGLint error = eglGetError(); error != EGL_SUCCESS;
		error = eglGetError()) {
		fprintf(stderr, "after %s() eglError %s (0x%x)\n",
			op, EGLUtils::strerror(error), error);
		return false;
	}
	return true;
}


#if 0
static void checkGlError(const char* op) {
	for (GLint error = glGetError(); error;
		error = glGetError()) {
		fprintf(stderr, "after %s() glError (0x%x)\n", op, error);
	}
}
#endif
static bool checkGlError(const char* op) {
	for (GLint error = glGetError(); error;
		error = glGetError()) {
		fprintf(stderr, "after %s() glError (0x%x)\n", op, error);
		return false;
	}
	return true;
}

//static EGLSurface surface;

	
EGLNativeWindowType my_window;
sp<Surface> sf;

static void initGLSurface(void)
{
	status_t err;

	sp<SurfaceComposerClient> session = new SurfaceComposerClient;

	// Get main display parameters.
	sp<IBinder> mainDpy = SurfaceComposerClient::getBuiltInDisplay(
		ISurfaceComposer::eDisplayIdMain);

	DisplayInfo dinfo;
	err = SurfaceComposerClient::getDisplayInfo(mainDpy, &dinfo);
	if (err != NO_ERROR) {
		fprintf(stderr, "ERROR: unable to get display characteristics\n");
		return;
	}

	sp<SurfaceControl> control =
		session->createSurface(String8("Triangle"), dinfo.w, dinfo.h,
			PIXEL_FORMAT_RGB_565);
			//PIXEL_FORMAT_RGBX_8888, ISurfaceComposerClient::eOpaque);
	
	if (control  == NULL || !control ->isValid()) {
		fprintf(stderr, "Failed to create SurfaceControl\n");
		return;
	}

	SurfaceComposerClient::openGlobalTransaction();
	//control->setLayer(0x7FFFFFFF);     // always on top
	control->setLayer(0x40000000);
	SurfaceComposerClient::closeGlobalTransaction();

	//sp<Surface> surface = control->getSurface();
	sp<Surface> sf = control->getSurface();
	// xbdong
	my_window = (EGLNativeWindowType) sf.get();
}

// EGL variables
EGLDisplay          eglDisplay;
EGLConfig	    eglConfig;
EGLContext          eglContext;
EGLSurface          eglSurface;


	// Handles for the two shaders used to draw the triangle, and the program handle which combines them.
	GLuint fragmentShader, vertexShader;
	GLuint shaderProgram;
	
	// Handles for the vertex buffer object
	GLuint              vertexBuffer;

	// Should the app still be animating?
	bool                isAnimating;
	
	// Is everything required initialised?
	bool				isInitialised;

	// Has an error occurred?
	bool				errorOccurred;

/*!*****************************************************************************************************************************************
 @Function		CreateEGLDisplay
 @Output		eglDisplay				    EGLDisplay created by the function
 @Return		Whether the function succeeded or not.
 @Description	Creates an EGLDisplay and initialises it.
*******************************************************************************************************************************************/
bool CreateEGLDisplay(EGLDisplay &eglDisplay) 
{
	/*	Get an EGL display.
		EGL uses the concept of a "display" which in most environments corresponds to a single physical screen. After creating a native
		display for a given windowing system, EGL can use this handle to get a corresponding EGLDisplay handle to it for use in rendering.
	*/
	eglDisplay = eglGetDisplay((EGLNativeDisplayType)0);
	if (eglDisplay == EGL_NO_DISPLAY)
	{
		//__android_log_print(ANDROID_LOG_ERROR, APPLICATION_NAME, "Failed to get an EGLDisplay");
		fprintf(stderr, "Failed to get an EGLDisplay\n");
		return false;
	}

	/*	Initialize EGL.
		EGL has to be initialized with the display obtained in the previous step. All EGL functions other than eglGetDisplay 
		and eglGetError need an initialised EGLDisplay. 
		If an application is not interested in the EGL version number it can just pass NULL for the second and third parameters, but they 
		are queried here for illustration purposes.
	*/
	EGLint eglMajorVersion, eglMinorVersion;
	if (!eglInitialize(eglDisplay, &eglMajorVersion, &eglMinorVersion))
	{
		//__android_log_print(ANDROID_LOG_ERROR, APPLICATION_NAME, "Failed to initialise the EGLDisplay");
		fprintf(stderr, "Failed to initialise the EGLDisplay\n");
		return false;
	}

	return true;
}

/*!*****************************************************************************************************************************************
 @Function		ChooseEGLConfig
 @Input			eglDisplay                  The EGLDisplay used by the application
 @Output		eglConfig                   The EGLConfig chosen by the function
 @Return		Whether the function succeeded or not.
 @Description	Chooses an appropriate EGLConfig and return it.
*******************************************************************************************************************************************/
bool ChooseEGLConfig(EGLDisplay eglDisplay, EGLConfig& eglConfig) 
{
	/*	Specify the required configuration attributes.
		An EGL "configuration" describes the capabilities an application requires and the type of surfaces that can be used for drawing.
		Each implementation exposes a number of different configurations, and an application needs to describe to EGL what capabilities it
		requires so that an appropriate one can be chosen. The first step in doing this is to create an attribute list, which is an array
		of key/value pairs which describe particular capabilities requested. In this application nothing special is required so we can query
		the minimum of needing it to render to a window, and being OpenGL ES 3.0 capable.
		Note: Initially there was no way to specify ES3 support at this point, and it has subsequently been added as an extension. However
		for simplicity this still declares the ES2 bit, and the application queries for specific ES 3.0 support when creating the context
		later.
	*/
	const EGLint configurationAttributes[] =
	{
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};

	/*	Find a suitable EGLConfig
		eglChooseConfig is provided by EGL to provide an easy way to select an appropriate configuration. It takes in the capabilities
		specified in the attribute list, and returns a list of available configurations that match or exceed the capabilities requested.
		Details of all the possible attributes and how they are selected for by this function are available in the EGL reference pages here:
		http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglChooseConfig.html
		It is also possible to simply get the entire list of configurations and use a custom algorithm to choose a suitable one, as many
		advanced applications choose to do. For this application however, taking the first EGLConfig that the function returns suits
		its needs perfectly, so we limit it to returning a single EGLConfig.
	*/
	EGLint configsReturned;
	if (!eglChooseConfig(eglDisplay, configurationAttributes, &eglConfig, 1, &configsReturned) || (configsReturned != 1))
	{
		//__android_log_print(ANDROID_LOG_ERROR, APPLICATION_NAME, "Failed to choose a suitable config.\n");
		fprintf(stderr, "Failed to choose a suitable config.\n");
		return false;
	}

	return true;
}

/*!*****************************************************************************************************************************************
 @Function		CreateEGLSurface
 @Input			nativeWindow                A native window that's been created
 @Input			eglDisplay                  The EGLDisplay used by the application
 @Input			eglConfig                   An EGLConfig chosen by the application
 @Output		eglSurface					The EGLSurface created from the native window.
 @Return		Whether the function succeeds or not.
 @Description	Creates an EGLSurface from a native window
*******************************************************************************************************************************************/
bool CreateEGLSurface( ANativeWindow* nativeWindow, EGLDisplay eglDisplay, EGLConfig eglConfig, EGLSurface& eglSurface) 
{
	/*	Create an EGLSurface for rendering.
		Using a native window created earlier and a suitable eglConfig, a surface is created that can be used to render OpenGL ES calls to.
		There are three main surface types in EGL, which can all be used in the same way once created but work slightly differently:
		 - Window Surfaces  - These are created from a native window and are drawn to the screen.
		 - Pixmap Surfaces  - These are created from a native windowing system as well, but are offscreen and are not displayed to the user.
		 - PBuffer Surfaces - These are created directly within EGL, and like Pixmap Surfaces are offscreen and thus not displayed.
		The offscreen surfaces are useful for non-rendering contexts and in certain other scenarios, but for most applications the main
		surface used will be a window surface as performed below.
	*/
	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)nativeWindow, NULL);
	if (!checkEglError("eglCreateWindowSurface"))
	{
		return false;
	}

	return true;
}

/*!*****************************************************************************************************************************************
 @Function		SetupEGLContext
 @Input			eglDisplay                  The EGLDisplay used by the application
 @Input			eglConfig                   An EGLConfig chosen by the application
 @Input			eglSurface					The EGLSurface created from the native window.
 @Output		eglContext                  The EGLContext created by this function
 @Input			nativeWindow                A native window, used to display error messages
 @Return		Whether the function succeeds or not.
 @Description	Sets up the EGLContext, creating it and then installing it to the current thread.
*******************************************************************************************************************************************/
bool SetupEGLContext( EGLDisplay eglDisplay, EGLConfig eglConfig, EGLSurface eglSurface, EGLContext& eglContext ) 
{
	/*	Make OpenGL ES the current API.
		EGL needs a way to know that any subsequent EGL calls are going to be affecting OpenGL ES,
		rather than any other API (such as OpenVG).
	*/
	eglBindAPI(EGL_OPENGL_ES_API);
	if (!checkEglError("eglBindAPI"))
	{
		return false;
	}

	/*	Create a context.
		EGL has to create what is known as a context for OpenGL ES. The concept of a context is OpenGL ES's way of encapsulating any
		resources and state. What appear to be "global" functions in OpenGL actually only operate on the current context. A context
		is required for any operations in OpenGL ES.
		Similar to an EGLConfig, a context takes in a list of attributes specifying some of its capabilities. However in most cases this
		is limited to just requiring the version of the OpenGL ES context required - In this case, OpenGL ES 3.0.
	*/
	EGLint contextAttributes[] = 
	{
		EGL_CONTEXT_CLIENT_VERSION, 3, 
		EGL_NONE
	};

	// Create the context with the context attributes supplied
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, contextAttributes);
	if (!checkEglError("eglCreateContext"))
	{
		return false;
	}

	/*	Bind the context to the current thread.
		Due to the way OpenGL uses global functions, contexts need to be made current so that any function call can operate on the correct
		context. Specifically, make current will bind the context to the thread it's called from, and unbind it from any others. To use
		multiple contexts at the same time, users should use multiple threads and synchronise between them.
	*/
	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	if (!checkEglError("eglMakeCurrent"))
	{
		return false;
	}

	return true;
}

/*!*****************************************************************************************************************************************
 @Function		InitialiseBuffer
 @Output		vertexBuffer                Handle to a vertex buffer object
 @Return		Whether the function succeeds or not.
 @Description	Initialises shaders, buffers and other state required to begin rendering with OpenGL ES
*******************************************************************************************************************************************/
bool InitialiseBuffer(GLuint &vertexBuffer) 
{
	/*	Concept: Vertices
		When rendering a polygon or model to screen, OpenGL ES has to be told where to draw the object, and more fundamentally what shape 
		it is. The data used to do this is referred to as vertices, points in 3D space which are usually collected into groups of three 
		to render as triangles. Fundamentally, any advanced 3D shape in OpenGL ES is constructed from a series of these vertices - each 
		vertex representing one corner of a polygon.
	*/

	/*	Concept: Buffer Objects
		To operate on any data, OpenGL first needs to be able to access it. The GPU maintains a separate pool of memory it uses independent
		of the CPU. Whilst on many embedded systems these are in the same physical memory, the distinction exists so that they can use and
		allocate memory without having to worry about synchronising with any other processors in the device.
		To this end, data needs to be uploaded into buffers, which are essentially a reserved bit of memory for the GPU to use. By creating
		a buffer and giving it some data we can tell the GPU how to render a triangle.
	*/

	// Vertex data containing the positions of each point of the triangle
	GLfloat vertexData[] = {-0.4f,-0.4f, 0.0f,  // Bottom Left
	                         0.4f,-0.4f, 0.0f,  // Bottom Right
	                         0.0f, 0.4f, 0.0f}; // Top Middle

	// Generate a buffer object
	glGenBuffers(1, &vertexBuffer);

	// Bind buffer as an vertex buffer so we can fill it with data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	
	/*	Set the buffer's size, data and usage
		Note the last argument - GL_STATIC_DRAW. This tells the driver that we intend to read from the buffer on the GPU, and don't intend
		to modify the data until we're done with it.		
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	if (!checkGlError("glBufferData"))
	{
		return false;
	}

	return true;
}


/*!*****************************************************************************************************************************************
 @Function		InitialiseShaders
 @Output		fragmentShader              Handle to a fragment shader
 @Output		vertexShader                Handle to a vertex shader
 @Output		shaderProgram               Handle to a shader program containing the fragment and vertex shader
 @Return		Whether the function succeeds or not.
 @Description	Initialises shaders, buffers and other state required to begin rendering with OpenGL ES
*******************************************************************************************************************************************/
bool InitialiseShaders( GLuint &fragmentShader, GLuint &vertexShader, GLuint &shaderProgram) 
{
	/*	Concept: Shaders
		OpenGL ES 3.0 uses what are known as shaders to determine how to draw objects on the screen. Instead of the fixed function
		pipeline in early OpenGL or OpenGL ES 1.x, users can now programmatically define how vertices are transformed on screen, what
		data is used where, and how each pixel on the screen is coloured.
		These shaders are written in GL Shading Language ES: http://www.khronos.org/registry/gles/specs/3.0/GLSL_ES_Specification_3.00.4.pdf
		which is usually abbreviated to simply "GLSL ES".
		Each shader is compiled on-device and then linked into a shader program, which combines a vertex and fragment shader into a form 
		that the OpenGL ES implementation can execute.
	*/

	/*	Concept: Fragment Shaders
		In a final buffer of image data, each individual point is referred to as a pixel. Fragment shaders are the part of the pipeline
		which determine how these final pixels are coloured when drawn to the framebuffer. When data is passed through here, the positions
		of these pixels is already set, all that's left to do is set the final colour based on any defined inputs.
		The reason these are called "fragment" shaders instead of "pixel" shaders is due to a small technical difference between the two
		concepts. When you colour a fragment, it may not be the final colour which ends up on screen. This is particularly true when 
		performing blending, where multiple fragments can contribute to the final pixel colour.
	*/
	const char* const fragmentShaderSource = "\
									  #version 300 es\n\
									  layout (location = 0) out lowp vec4 outColour;\
									  \
									  void main (void)\
									  {\
									      outColour = vec4(1.0, 1.0, 0.66 ,1.0);\
									  }";

	// Create a fragment shader object
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load the source code into it
	glShaderSource(fragmentShader, 1, (const char**)&fragmentShaderSource, NULL);

	// Compile the source code
	glCompileShader(fragmentShader);

	// Check that the shader compiled
	GLint isShaderCompiled;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiled);
	if (!isShaderCompiled)
	{
		// If an error happened, first retrieve the length of the log message
		int infoLogLength, charactersWritten;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		// Allocate enough space for the message and retrieve it
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(fragmentShader, infoLogLength, &charactersWritten, infoLog);

		// Display the error in a dialog box
		if (infoLogLength>1)
		{
			fprintf(stderr, "%s\n", infoLog);
			//__android_log_print(ANDROID_LOG_ERROR, APPLICATION_NAME, "%s", infoLog);
		}
		else
		{
			fprintf(stderr, "Failed to compile fragment shader.\n");
			//__android_log_print(ANDROID_LOG_ERROR, APPLICATION_NAME, "Failed to compile fragment shader.");
		}

		delete[] infoLog;
		return false;
	}
	
	/*	Concept: Vertex Shaders
		Vertex shaders primarily exist to allow a developer to express how to orient vertices in 3D space, through transformations like 
		Scaling, Translation or Rotation. Using the same basic layout and structure as a fragment shader, these take in vertex data and 
		output a fully transformed set of positions. Other inputs are also able to be used such as normals or texture coordinates, and can 
		also be transformed and output alongside the position data.
	*/
	// Vertex shader code
	const char* const vertexShaderSource = "\
									  #version 300 es\n\
									  layout (location = 0) in highp vec4	myVertex;\
									  uniform mediump mat4	transformationMatrix;\
									  \
									  void main(void)\
									  {\
									      gl_Position = transformationMatrix * myVertex;\
									  }";

	// Create a vertex shader object
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Load the source code into the shader
	glShaderSource(vertexShader, 1, (const char**)&vertexShaderSource, NULL);

	// Compile the shader
	glCompileShader(vertexShader);

	// Check the shader has compiled
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiled);
	if (!isShaderCompiled)
	{
		// If an error happened, first retrieve the length of the log message
		int infoLogLength, charactersWritten;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		// Allocate enough space for the message and retrieve it
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(vertexShader, infoLogLength, &charactersWritten, infoLog);

		// Display the error in a dialog box
		if (infoLogLength>1)
		{
			//__android_log_print(ANDROID_LOG_ERROR, APPLICATION_NAME, "%s", infoLog);
			fprintf(stderr, "%s\n", infoLog);
		}
		else
		{
			//__android_log_print(ANDROID_LOG_ERROR, APPLICATION_NAME, "Failed to compile vertex shader.");
			fprintf(stderr, "Failed to compile vertex shader.\n");
		}

		delete[] infoLog;
		return false;
	}

	// Create the shader program
	shaderProgram = glCreateProgram();

	// Attach the fragment and vertex shaders to it
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, vertexShader);

	// Bind the vertex attribute "myVertex" to location VERTEX_ARRAY (0)
	glBindAttribLocation(shaderProgram, VERTEX_ARRAY, "myVertex");

	// Link the program
	glLinkProgram(shaderProgram);

	// Check if linking succeeded in the same way we checked for compilation success
	GLint isLinked;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
	if (!isLinked)
	{
		// If an error happened, first retrieve the length of the log message
		int infoLogLength, charactersWritten;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		// Allocate enough space for the message and retrieve it
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(shaderProgram, infoLogLength, &charactersWritten, infoLog);

		// Display the error in a dialog box
		if (infoLogLength>1)
		{
			//__android_log_print(ANDROID_LOG_ERROR, APPLICATION_NAME, "%s", infoLog);
			fprintf(stderr, "%s\n", infoLog);
		}
		else
		{
			//__android_log_print(ANDROID_LOG_ERROR, APPLICATION_NAME, "Failed to link shader program.");
			fprintf(stderr, "Failed to link shader program.\n");
		}

		delete[] infoLog;
		return false;
	}
	
	/*	Use the Program
		Calling glUseProgram tells OpenGL ES that the application intends to use this program for rendering. Now that it's installed into
		the current state, any further glDraw* calls will use the shaders contained within it to process scene data. Only one program can
		be active at once, so in a multi-program application this function would be called in the render loop. Since this application only
		uses one program it can be installed in the current state and left there.
	*/
	glUseProgram(shaderProgram);

	if (!checkGlError("glUseProgram"))
	{
		return false;
	}
	
	return true;
}

/*!*****************************************************************************************************************************************
 @Function		RenderScene
 @Input			shaderProgram               The shader program used to render the scene
 @Input			eglDisplay                  The EGLDisplay used by the application
 @Input			eglSurface					The EGLSurface created from the native window.
 @Return		Whether the function succeeds or not.
 @Description	Renders the scene to the framebuffer. Usually called within a loop.
*******************************************************************************************************************************************/
bool RenderScene( GLuint shaderProgram, EGLDisplay eglDisplay, EGLSurface eglSurface ) 
{	
	/*	Set the clear color
		At the start of a frame, generally you clear the image to tell OpenGL ES that you're done with whatever was there before and want to
		draw a new frame. In order to do that however, OpenGL ES needs to know what colour to set in the image's place. glClearColor
		sets this value as 4 floating point values between 0.0 and 1.0, as the Red, Green, Blue and Alpha channels. Each value represents
		the intensity of the particular channel, with all 0.0 being transparent black, and all 1.0 being opaque white. Subsequent calls to
		glClear with the colour bit will clear the frame buffer to this value.
		The functions glClearDepth and glClearStencil allow an application to do the same with depth and stencil values respectively.
	*/
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

	/*	Clears the color buffer.
		glClear is used here with the Colour Buffer to clear the colour. It can also be used to clear the depth or stencil buffer using 
		GL_DEPTH_BUFFER_BIT or GL_STENCIL_BUFFER_BIT, respectively.
	*/
	glClear(GL_COLOR_BUFFER_BIT);

	// Get the location of the transformation matrix in the shader using its name
	int matrixLocation = glGetUniformLocation(shaderProgram, "transformationMatrix");

	// Matrix used to specify the orientation of the triangle on screen.
	const float transformationMatrix[] =
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	// Pass the transformationMatrix to the shader using its location
	glUniformMatrix4fv( matrixLocation, 1, GL_FALSE, transformationMatrix);
	if (!checkGlError("glUniformMatrix4fv"))
	{
		return false;
	}

	// Enable the user-defined vertex array
	glEnableVertexAttribArray(VERTEX_ARRAY);

	// Sets the vertex data to this attribute index, with the number of floats in each position
	glVertexAttribPointer(VERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, 0, 0);
	if (!checkGlError("glVertexAttribPointer"))
	{
		return false;
	}

	/*	Draw the triangle
		glDrawArrays is a draw call, and executes the shader program using the vertices and other state set by the user. Draw calls are the
		functions which tell OpenGL ES when to actually draw something to the framebuffer given the current state.
		glDrawArrays causes the vertices to be submitted sequentially from the position given by the "first" argument until it has processed
		"count" vertices. Other draw calls exist, notably glDrawElements which also accepts index data to allow the user to specify that
		some vertices are accessed multiple times, without copying the vertex multiple times.
		Others include versions of the above that allow the user to draw the same object multiple times with slightly different data, and
		a version of glDrawElements which allows a user to restrict the actual indices accessed.
	*/
	glDrawArrays(GL_TRIANGLES, 0, 3);
	if (!checkGlError("glDrawArrays"))
	{
		return false;
	}

	/*	Present the display data to the screen.
		When rendering to a Window surface, OpenGL ES is double buffered. This means that OpenGL ES renders directly to one frame buffer, 
		known as the back buffer, whilst the display reads from another - the front buffer. eglSwapBuffers signals to the windowing system
		that OpenGL ES 3.0 has finished rendering a scene, and that the display should now draw to the screen from the new data. At the same
		time, the front buffer is made available for OpenGL ES 3.0 to start rendering to. In effect, this call swaps the front and back 
		buffers.
	*/
	if (!eglSwapBuffers(eglDisplay, eglSurface) )
	{
		checkEglError("eglSwapBuffers");
		return false;
	}
	
	return true;
}


//int main(int argc, char** argv) {
int main(void) {
	if (!CreateEGLDisplay(eglDisplay))
	{
		errorOccurred = true;
		fprintf(stderr, "CreateEGLDisplay error\n");
	}

	if (!ChooseEGLConfig(eglDisplay, eglConfig))
	{
		errorOccurred = true;
		fprintf(stderr, "ChooseEGLConfig error\n");
	}

	// 需要获取 Window 窗口用于绘制
	//if (!CreateEGLSurface(sf.get, eglDisplay, 
	if (!CreateEGLSurface(my_window, eglDisplay, 
		eglConfig, eglSurface))
	{
		errorOccurred = true;
		fprintf(stderr, "CreateEGLSurface error\n");
	}

	if (!SetupEGLContext(eglDisplay, eglConfig, eglSurface, eglContext) )
	{
		errorOccurred = true;
		fprintf(stderr, "SetupEGLContext error\n");
	}

	if (!InitialiseBuffer(vertexBuffer) )
	{
		errorOccurred = true;
		fprintf(stderr, "InitialiseBuffer error\n");
	}

	if (errorOccurred != true)
	{
		if (!InitialiseShaders(fragmentShader, vertexShader, 
			shaderProgram) )
		{
			errorOccurred = true;
			fprintf(stderr, "InitialiseShaders error\n");
		}
		else
		{
			isInitialised = true;
			fprintf(stderr, "Success error\n");
		}
	}
	
	if (!RenderScene(shaderProgram, eglDisplay, eglSurface))
	{
		fprintf(stderr, "RenderScene error\n");
	}

	return 0;
}
