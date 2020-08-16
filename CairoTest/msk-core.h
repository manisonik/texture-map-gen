#pragma once
#include "stdafx.h"
#include "msk-event.h"

using VertexVariant = std::variant<GLfloat, glm::vec2, glm::vec3, glm::vec4>;

enum DockType {
	DockLeft,
	DockRight,
	DockTop,
	DockBottom
};

enum AnchorType {
	AnchorLeft,
	AnchorRight,
	AnchorTop,
	AnchorBottom
};

enum WindowType {
	Overlapped		= 0x00000000L,
	Popup			= 0x80000000L,
	Child			= 0x40000000L,
	ClipSiblings	= 0x04000000L,
	ClipChildren	= 0x02000000L
};

enum MskShaderType 
{
	Vertex		= GL_VERTEX_SHADER,
	Fragment	= GL_FRAGMENT_SHADER,
	Geometry	= GL_GEOMETRY_SHADER
};

enum MskTextureFilter {
	Linear					= GL_LINEAR,
	MipMap					= GL_MIPMAP,
	MipMapLinearLinear		= GL_LINEAR_MIPMAP_LINEAR,
	MipMapLinearNearest		= GL_LINEAR_MIPMAP_NEAREST,
	MipMapNearestLinear		= GL_NEAREST_MIPMAP_LINEAR,
	MipMapNearestNearest	= GL_NEAREST_MIPMAP_NEAREST,
	Nearest					= GL_NEAREST
};

enum MskTextureWrap {
	ClampToEdge		= GL_CLAMP_TO_EDGE,
	MirroredRepeat	= GL_MIRRORED_REPEAT,
	Repeat			= GL_REPEAT
};

enum MskTextureFormat {
	Red				= GL_RED,
	RG				= GL_RG,
	RGB				= GL_RGB,
	BGR				= GL_BGR,
	RGBA			= GL_RGBA,
	BGRA			= GL_BGRA,
	RedInteger		= GL_RED_INTEGER,
	RGInteger		= GL_RG_INTEGER,
	RGBInteger		= GL_RGB_INTEGER,
	BGRInteger		= GL_BGR_INTEGER,
	RGBAInteger		= GL_RGBA_INTEGER,
	BGRAInteger		= GL_BGRA_INTEGER,
	Stencil			= GL_STENCIL_INDEX,
	DepthComponent	= GL_DEPTH_COMPONENT,
	DepthStencil	= GL_DEPTH_STENCIL
};

enum MskTextureTarget {
	OneDimension		= GL_TEXTURE_1D,
	TwoDimension		= GL_TEXTURE_2D,
	ThreeDimension		= GL_TEXTURE_3D,
	Recta				= GL_TEXTURE_RECTANGLE,
	Buffer				= GL_TEXTURE_BUFFER,
	CubeMap				= GL_TEXTURE_CUBE_MAP,
	OneDimensionArray	= GL_TEXTURE_1D_ARRAY,
	TwoDimensionArray	= GL_TEXTURE_2D_ARRAY,
	CubeMapArray		= GL_TEXTURE_CUBE_MAP_ARRAY,
	Multisample			= GL_TEXTURE_2D_MULTISAMPLE,
	MultisampleArray	= GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};

enum MskMeshes {
	Cube,
	Pyramid,
	Cone,
	Circle,
	Plane
};

enum class MskVertexUsage {
	Normal,
	BiNormal,
	Position,
	Tangent,
	TexCoords,
	Color
};

struct MskFace {
public:
	std::vector<GLshort> indices;
};

struct MskMaterial {
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

struct MskRay : public MskObject
{
public:
	virtual glm::vec3 GetDirection() = 0;
	virtual GLvoid SetOrigin(glm::vec3 origin) = 0;
	virtual GLvoid SetDirection(glm::vec3 direction) = 0;
	virtual GLboolean Intersect(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec3& bary) = 0;
};

struct MskCairo : public MskObject {
public:
	virtual GLsizei GetWidth() = 0;
	virtual GLsizei GetHeight() = 0;
	virtual GLvoid Clear(double r, double g, double b, double a) = 0;
	virtual GLvoid Rectangle(double x, double y, double width, double height) = 0;
	virtual GLvoid Circle(double x, double y, double width, double height) = 0;
	virtual GLvoid Triangle(double x, double y, double width, double height) = 0;
	virtual GLvoid FillRectangle(double x, double y, double width, double height) = 0;
	virtual unsigned char* GetSurfaceData() = 0;
};

struct MskVertexAttribute : public MskObject {
public:
	std::string alias;
	GLuint type;
	GLuint componentCount;
	MskVertexUsage usage;
	GLuint offset;

	static MskVertexAttribute Normal()		{ 
		return { MskObject(), "vertexNormal",	GL_FLOAT, 3, MskVertexUsage::Normal}; };
	static MskVertexAttribute BiNormal()	{ 
		return { MskObject(), "vertexBiNormal", GL_FLOAT, 3, MskVertexUsage::BiNormal }; };
	static MskVertexAttribute Position()	{ 
		return { MskObject(), "vertexPosition", GL_FLOAT, 4, MskVertexUsage::Position }; };
	static MskVertexAttribute Tangent()		{ 
		return { MskObject(), "vertexTangent",	GL_FLOAT, 3, MskVertexUsage::Tangent }; };
	static MskVertexAttribute TexCoords()	{ 
		return { MskObject(), "vertexTexCoords",GL_FLOAT, 2, MskVertexUsage::TexCoords }; };
	static MskVertexAttribute Color()		{ 
		return { MskObject(), "vertexColor",	GL_FLOAT, 4, MskVertexUsage::Color }; };
};

struct MskVertexAttributes : public MskObject {
public:
	virtual void Add(std::unique_ptr<MskVertexAttribute>& vertexAttribute) = 0;
	virtual void Begin() = 0;
	virtual void End() = 0;
};

struct MskTextureData : public MskObject {
	GLuint		m_nWidth;
	GLuint		m_nHeight;
	GLubyte		m_nColorType;
	GLubyte		m_nBitDepth;
	GLubyte		m_nFormat;
	GLuint		m_nNumOfPasses;
	GLubyte*	m_pBytes;
};

struct MskFontCharacter {
	GLfloat ax;
	GLfloat ay;
	GLfloat bw;
	GLfloat bh;
	GLfloat bl;
	GLfloat bt;
	GLfloat tx;
};

struct MskFont : public MskObject {
	virtual std::vector<MskFontCharacter> GetCharacters() = 0;
	virtual GLuint GetTextureWidth() = 0;
	virtual GLuint GetTextureHeight() = 0;
	virtual GLuint GetTextureId() = 0;
};

struct MskMesh : public MskObject {
	virtual GLvoid SetColor(glm::vec4 color) = 0;
	virtual GLvoid AddPlane() = 0;
	virtual GLvoid AddCube() = 0;
	virtual GLvoid AddGrid(GLfloat width, GLfloat height, GLfloat count) = 0;
	virtual GLvoid SetVertices(std::vector<glm::vec3>& vertices) = 0;
	virtual GLvoid SetNormals(std::vector<glm::vec3>& normals) = 0;
	virtual GLvoid SetIndices(std::vector<GLshort>& indices) = 0;
	virtual GLvoid SetTextureCoords(std::vector<glm::vec2>& texCoords) = 0;
	virtual GLvoid Rotate(GLfloat angle) = 0;
	virtual GLvoid Translate(GLfloat dx, GLfloat dy, GLfloat dz) = 0;
	virtual GLvoid Translate(GLuint subset, GLfloat dx, GLfloat dy, GLfloat dz) = 0;
	virtual GLvoid Scale(GLfloat x, GLfloat y, GLfloat z) = 0;
	virtual GLvoid SetVertexAttributes(std::vector<MskVertexAttribute>& vertexAttributes) = 0;
	virtual GLvoid Update() = 0;
	virtual std::vector<MskFace>& GetFaces() = 0;
	virtual std::vector<glm::vec3>& GetTangents() = 0;
	virtual std::vector<glm::vec3>& GetBitangents() = 0;
	virtual std::vector<glm::vec3>& GetNormals() = 0;
	virtual std::vector<glm::vec3>& GetVertices() = 0;
	virtual std::vector<GLshort>& GetIndices() = 0;
	virtual std::vector<glm::vec2>& GetTextureCoords() = 0;
	virtual glm::mat4& GetModelMatrix() = 0;
	virtual glm::vec4& GetColor() = 0;
};

struct MskTexture : public MskObject {
	virtual GLvoid Bind(GLuint unit) = 0;
	virtual GLvoid Bind() = 0;
	virtual GLvoid SetInternalFormat(MskTextureFormat internalFormat) = 0;
	virtual GLvoid SetFormat(MskTextureFormat format) = 0;
	virtual GLvoid SetFilter(MskTextureFilter minFilter, MskTextureFilter magFilter) = 0;
	virtual GLvoid SetWrap(MskTextureWrap u, MskTextureWrap v) = 0;
	virtual GLuint GetTextureId() = 0;
	virtual GLboolean LoadFromFile(std::string filename) = 0;
	virtual GLboolean LoadFromMemory(std::shared_ptr<MskTextureData> textureData) = 0;
	virtual MskTextureTarget GetTextureTarget() = 0;
	virtual GLvoid EnableMultisampling(GLuint samples) = 0;
};

struct MskTextureShape : public MskObject {

};

struct MskTextRenderer : public MskObject 
{
	virtual GLvoid Begin(std::shared_ptr<MskFont>& mskFont) = 0;
	virtual GLvoid SetProjectionMatrix(glm::mat4 & matProj) = 0;
	virtual GLvoid End() = 0;
	virtual GLvoid Flush() = 0;
	virtual GLvoid RenderText(std::string text, float x, float y, float scale) = 0;
};

struct MskShapeRenderer : public MskObject
{
	virtual void Begin() = 0;
	virtual void End() = 0;
	virtual void Flush() = 0;
	virtual void Draw(std::shared_ptr<MskMesh>& mesh) = 0;
	virtual void SetProjectionMatrix(glm::mat4& matProj) = 0;
};

struct MskText : public MskObject {};

struct MskControl : public MskObject 
{
public:
	Event<EventArgs> Render;
	Event<UpdateEventArgs> Update;
	Event<ResizeEventArgs> Resize;
};

struct MskScene : public MskObject
{
	Event<EventArgs> Render;
	Event<UpdateEventArgs> Update;
	Event<ResizeEventArgs> Resize;
	Event<MouseEventArgs> MouseMove;
	Event<KeyEventArgs> KeyDown;
	Event<KeyEventArgs> KeyPress;
	Event<KeyEventArgs> KeyUp;
};

struct MskCamera : public MskObject 
{
public:
	virtual void SetPosition(glm::vec3 & pos) = 0;
	virtual void Zoom(GLfloat amount) = 0;
	virtual void Pan(glm::vec2 units) = 0;
	virtual void MoveLeft(bool bMove) = 0;
	virtual void MoveRight(bool bMove) = 0;
	virtual void MoveBackward(bool bMove) = 0;
	virtual void MoveForward(bool bMove) = 0;
	virtual void Look(float mouseX, float mouseY, GLboolean mouseDown) = 0;
	virtual void Update(GLfloat dt) = 0;
	virtual void SetViewport(int x, int y, int width, int height) = 0;
	virtual void SetMode(GLuint mode) = 0;
	virtual void SetModelMatrix(glm::mat4 modelMatrix) = 0;
	virtual glm::mat4& GetModelViewProjectionMatrix() = 0;
	virtual glm::mat4& GetModelMatrix() = 0;
	virtual glm::mat4& GetViewMatrix() = 0;
	virtual glm::mat4& GetProjectionMatrix() = 0;
	virtual glm::vec3& GetPosition() = 0;
	virtual std::shared_ptr<MskRay> GetRay(int mouseX, int mouseY) = 0;
};

struct MskLineRenderer : public MskObject
{
	virtual GLvoid Begin() = 0;
	virtual GLvoid End() = 0;
	virtual GLvoid SetProjectionMatrix(glm::mat4& matProj) = 0;
	virtual GLvoid Flush() = 0;
	virtual GLvoid Draw(std::shared_ptr<MskMesh>& mesh) = 0;
};

struct MskView : public MskObject
{
public:
	Event<PaintArgs> Paint;
	Event<UpdateEventArgs> Update;
	Event<EventArgs> Render;
	Event<EventArgs> Destroy;
	Event<ResizeEventArgs> Resize;
	Event<EventArgs> ParentChanged;
	Event<EventArgs> MouseClick;
	Event<EventArgs> MouseDown;
	Event<EventArgs> MouseOver;
	Event<EventArgs> MouseDoubleClick;
	Event<EventArgs> MouseHover;
	Event<EventArgs> MouseLeave;
	Event<EventArgs> MouseEnter;
	Event<MouseEventArgs> MouseMove;
	Event<EventArgs> Move;
	Event<EventArgs> Load;
	Event<EventArgs> Click;
	Event<EventArgs> DockChanged;
	Event<EventArgs> DragEnter;
	Event<EventArgs> DragDrop;
	Event<EventArgs> DragLeave;
	Event<EventArgs> DragOver;
	Event<KeyEventArgs> KeyDown;
	Event<KeyEventArgs> KeyPress;
	Event<KeyEventArgs> KeyUp;

	virtual int AddControl(std::shared_ptr<MskControl>& control) = 0;
	virtual GLvoid AddScene(std::shared_ptr<MskScene>& scene) = 0;
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual void ClearScreen() = 0;
	virtual void SwapBuffers() = 0;
	virtual void MakeCurrent() = 0;
	virtual HDC GetDeviceContext() = 0;
};

struct MskOglView : public MskView 
{
public:
	Event<EventArgs> RenderScene;
};

struct MskShader : public MskObject
{
	virtual GLuint GetId() = 0;
	virtual std::string GetSource() = 0;
	virtual void LoadFromString(const std::string & source) = 0;
	virtual void LoadFromFile(const std::string & filename) = 0;
	virtual void Compile() = 0;
	virtual void GetError() = 0;
};

struct MskShaderProgram : public MskObject 
{
	virtual void AttachShader(std::shared_ptr<MskShader>& shader) = 0;
	virtual void Link() = 0;
	virtual void Use() = 0;
	virtual void Disable() = 0;
	virtual int AddAttribute(const std::string name) = 0;
	virtual int AddUniform(const std::string name) = 0;
	virtual void SetUniform(const std::string name, float x, float y, float z) = 0;
	virtual void SetUniform(const std::string name, const glm::vec3& v) = 0;
	virtual void SetUniform(const std::string name, const glm::vec4& v) = 0;
	virtual void SetUniform(const std::string name, const glm::mat3& m) = 0;
	virtual void SetUniform(const std::string name, const glm::mat4& m) = 0;
	virtual void SetUniform(const std::string name, float value) = 0;
	virtual void SetUniform(const std::string name, int value) = 0;
	virtual void SetUniform(const std::string name, bool value) = 0;
};

struct MskTextureRenderer : public MskObject 
{
	virtual GLvoid SetCamera(std::shared_ptr<MskCamera>& camera) = 0;
	virtual GLvoid SetCamera(glm::mat4& matProj) = 0;
	virtual GLvoid EnableLighting(GLboolean enable) = 0;
	virtual GLvoid EnablePOM(GLboolean enable) = 0;
	virtual GLvoid Draw(std::shared_ptr<MskMesh>& shape) = 0;
	virtual GLvoid Begin() = 0;
	virtual GLvoid Begin(std::shared_ptr<MskTexture>& texture) = 0;
	virtual GLvoid Begin(
		std::shared_ptr<MskTexture>& diffuse, 
		std::shared_ptr<MskTexture>& normal, 
		std::shared_ptr<MskTexture>& height
	) = 0;
	virtual GLvoid End() = 0;
	virtual GLvoid Flush() = 0;
};

struct MskImage : public MskObject {
	virtual HRESULT __stdcall Load(
		std::string filename
	) = 0;

	virtual HRESULT __stdcall GetImageData(
		std::shared_ptr<unsigned char>& data
	) = 0;
};

struct MskFrameRenderer : public MskObject
{
	virtual GLvoid Begin() = 0;
	virtual GLvoid End() = 0;
	virtual std::shared_ptr<MskTexture> GetTexture() = 0;
};

struct MskMultisampleFrameRenderer : public MskObject
{
	virtual GLvoid Begin() = 0;
	virtual GLvoid End() = 0;
	virtual GLvoid Resize(GLsizei width, GLsizei height) = 0;
	virtual std::shared_ptr<MskTexture> GetTexture() = 0;
};

struct MskTextureRegion : public MskObject 
{

};

struct MskTextureAtlas : public MskObject
{

};

struct MskVertexArray : public MskObject {
	virtual void Bind(std::shared_ptr<MskShaderProgram>& shader) = 0;
	virtual void End() = 0;
	virtual void SetVertices(std::vector<GLfloat>& vertices, std::vector<MskVertexAttribute>& vertexAttributes) = 0;
};

struct MskApp : public MskObject
{
public:
	// Events
	Event<EventArgs> Idle;
	Event<EventArgs> Activated;
	Event<EventArgs> Closed;
	Event<EventArgs> Deactivate;
	Event<EventArgs> ControlAdded;
	Event<EventArgs> ControlRemoved;

	virtual GLvoid CreateControl(std::shared_ptr<MskControl>& control) = 0;
	virtual GLvoid CreateView(std::shared_ptr<MskView>& view) = 0;
	virtual GLvoid CreateImage(std::shared_ptr<MskImage>& image) = 0;
	virtual GLvoid CreateShader(const MskShaderType & type, std::shared_ptr<MskShader>& shader) = 0;
	virtual GLvoid CreateShaderProgram(std::shared_ptr<MskShaderProgram>& shaderProgram) = 0;
	virtual GLvoid CreateFTFont(std::string filepath, std::shared_ptr<MskFont>& font) = 0;
	virtual GLvoid CreateTextRenderer(std::shared_ptr<MskTextRenderer>& textRenderer) = 0;
	virtual GLvoid CreateShapeRenderer(std::shared_ptr<MskShapeRenderer>& shapeRenderer) = 0;
	virtual GLvoid CreateCamera(std::shared_ptr<MskCamera>& mskCamera) = 0;
	virtual GLvoid CreateTexture(std::shared_ptr<MskTexture>& mskTexture) = 0;
	virtual GLvoid CreateTextureRenderer(std::shared_ptr<MskTextureRenderer>& mskTextureRenderer) = 0;
	virtual GLvoid CreateFrameRenderer(std::shared_ptr<MskFrameRenderer>& mskFrameRenderer) = 0;
	virtual GLvoid CreateMultisampleFrameRenderer(std::shared_ptr<MskMultisampleFrameRenderer>& mskFrameRenderer) = 0;
	virtual GLvoid CreateMesh(std::shared_ptr<MskMesh>& mskMesh) = 0;
	virtual GLvoid CreateCairo(std::shared_ptr<MskCairo>& cairo) = 0;
	virtual GLvoid CreateLineRenderer(std::shared_ptr<MskLineRenderer>& mskLineRenderer) = 0;
	virtual GLint  Run() = 0;
	virtual GLuint GetWidth() = 0;
	virtual GLuint GetHeight() = 0;
	virtual std::shared_ptr<MskCamera> GetCamera() = 0;
};

class MskFactory 
{
public:
	static GLvoid CreateApplication(std::shared_ptr<MskApp>& mskApp);
	static GLvoid CreateControl(std::shared_ptr<MskControl>& control);
	static GLvoid CreateView(std::shared_ptr<MskView>& view);
	static GLvoid CreateImage(std::shared_ptr<MskImage>& image);
	static GLvoid CreateShader(const MskShaderType & type, std::shared_ptr<MskShader>& shader);
	static GLvoid CreateShaderProgram(std::shared_ptr<MskShaderProgram>& shaderProgram);
	static GLvoid CreateFTFont(std::string filepath, std::shared_ptr<MskFont>& font);
	static GLvoid CreateTextRenderer(std::shared_ptr<MskTextRenderer>& textRenderer);
	static GLvoid CreateShapeRenderer(std::shared_ptr<MskShapeRenderer>& shapeRenderer);
	static GLvoid CreateCamera(std::shared_ptr<MskCamera>& mskCamera);
	static GLvoid CreateTexture(std::string filepath, std::shared_ptr<MskTexture>& mskTexture);
	static GLvoid CreateTextureRenderer(std::shared_ptr<MskTextureRenderer>& mskTextureRenderer);
	static GLvoid CreateMesh(MskMeshes shapes, std::shared_ptr<MskMesh>& mskMesh);
};



