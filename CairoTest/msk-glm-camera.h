#pragma once
#include "msk-glm-ray.h"
#include "msk-core.h"

class MCamera : public MskCamera
{
private:
	glm::mat4 m_matProjection;
	glm::mat4 m_matView;
	glm::mat4 m_matModel;
	glm::mat4 m_matMVP;
	glm::vec3 m_vecUp;
	glm::vec3 m_vecCenter;
	glm::vec3 m_vecEye;
	glm::vec3 m_vecPos;
	glm::vec3 m_vecTarget;
	glm::vec3 m_vecRight;
	glm::vec3 m_vecDirection;
	glm::vec3 m_vecFront;

	float m_mouseSensitivity;
	float m_mouseLastX;
	float m_mouseLastY;
	float m_Yaw;
	float m_Pitch;
	float m_FOV;
	bool m_bMoveLeft;
	bool m_bMoveRight;
	bool m_bMoveForward;
	bool m_bMoveBackward;
	float m_fSpeed;
	bool m_bFirstMouse;
	GLfloat m_fZoom;
	GLuint mode;

	int m_nWidth;
	int m_nHeight;

	std::shared_ptr<MskRay> m_pRay;
public:
	MCamera();
	~MCamera();

	void SetPosition(glm::vec3 & pos);
	void MoveLeft(bool bMove);
	void MoveRight(bool bMove);
	void MoveBackward(bool bMove);
	void MoveForward(bool bMove);
	void Look(float mouseX, float mouseY, GLboolean mouseDown);
	void Update(GLfloat dt);
	void SetViewport(int x, int y, int width, int height);
	void SetMode(GLuint mode);
	void SetModelMatrix(glm::mat4 modelMatrix);

	glm::vec3& GetPosition();
	glm::mat4& GetModelViewProjectionMatrix();
	glm::mat4& GetModelMatrix();
	glm::mat4& GetViewMatrix();
	glm::mat4& GetProjectionMatrix();
	void Pan(glm::vec2 units);
	void Zoom(GLfloat amount);
	std::shared_ptr<MskRay> GetRay(int mouseX, int mouseY);
};

